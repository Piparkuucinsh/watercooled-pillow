import express, { Express, Request, Response } from 'express';
import dotenv from 'dotenv';
import path from 'path';

dotenv.config();

const app: Express = express();
const port = process.env.PORT;

app.use(express.json());

let pinValue = "LOW";
let temp = 0;

app.post('/api/button', (req: Request, res: Response) => {
    console.log("kidala")
    if (!req.body.pinValue) {
        res.send("kidala")
    }
    pinValue = req.body.pinValue;
    res.send('set value');
});

app.get('/api/temp', (req: Request, res: Response) => {
    const data = {temp: temp.toFixed(1)}
    console.log("test")
    res.send(JSON.stringify(data))
});

app.get('/', (req: Request, res: Response) => {
    console.log("prikol")
    console.log(path.join(__dirname, '..', '/index.html'))
    res.sendFile(path.join(__dirname, '..', '/index.html'));
});

app.post('/api/send', (req: Request, res: Response) => {
    console.log(req.body);
    temp = req.body.sensorValue;
    res.send("success")
});

app.get('/api/receive', (req: Request, res: Response) => {
    console.log("fujaks")
    const data = {pinValue: pinValue}
    res.send(JSON.stringify(data))
});

app.listen(port, () => {
    console.log(`⚡️[server]: Server is running at http://localhost:${port}`);
});