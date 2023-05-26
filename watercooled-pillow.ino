#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <NTC_Thermistor.h>


// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "pass";

// Replace with your web server address
const char* serverAddress = "192.168.0.108";
const int port = 8000;

// Replace with your sensor pin
const int sensorPin = A0;

const int testPin = 0;

// Set the size of JSON buffer
const int bufferSize = JSON_OBJECT_SIZE(1);

int ThermistorPin = A0;
float R0 = 10000;

NTC_Thermistor thermistor(ThermistorPin, R0, 9500, 20, 3950);

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  pinMode(0, HIGH);

  // Connect to Wi-Fi
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, password);
  }

  Serial.println("Connected to WiFi");

}

void loop() {

  double celsius = thermistor.readCelsius() + 27;

  Serial.print("Temperature: "); 
  Serial.print(celsius);
  Serial.println(" C"); 

  // Read sensor value
  //int sensorValue = analogRead(sensorPin);
  double sensorValue = celsius;

  // Create JSON object
  StaticJsonDocument<bufferSize> jsonDoc;
  jsonDoc["sensorValue"] = sensorValue;

  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send HTTP POST request
  String contentType = ("Content-Type", "application/json");

  client.post("/api/send", contentType, jsonString);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.print(statusCode);
  Serial.print(", Response: ");
  Serial.println(response);

  client.get("/api/receive");
  int receiveStatusCode = client.responseStatusCode();
  String receiveResponse = client.responseBody();

  Serial.println(receiveStatusCode); 
  Serial.println(receiveResponse); 

  DynamicJsonDocument receiveJsonDoc(1024);
  DeserializationError error = deserializeJson(receiveJsonDoc, receiveResponse);

  if (error) {
    Serial.print("JSON parsing error: ");
    Serial.println(error.c_str());
    return;
  }

  // Retrieve value from JSON
  String pinValue = receiveJsonDoc["pinValue"];

  Serial.print("Writing ");
  Serial.print(pinValue);
  Serial.print(" to pin ");
  Serial.println(testPin);

  // Update pin value
  if (pinValue == "HIGH") {
    digitalWrite(testPin, HIGH);
  } 
  else if (pinValue == "LOW") {
    digitalWrite(testPin, LOW);
  }

  // Wait for a moment before sending the next request
  delay(100);
}