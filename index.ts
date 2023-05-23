import express, { Express, Request, Response } from 'express';
import dotenv from 'dotenv';

dotenv.config();

const app: Express = express();
const port = process.env.PORT;

app.use(express.json());

app.get('/', (req: Request, res: Response) => {
    console.log("prikol")
    res.send('Express + TypeScript Server');
});

app.post('/api/send', (req: Request, res: Response) => {
    console.log(req.body);
    res.send("success")
});

app.listen(port, () => {
    console.log(`⚡️[server]: Server is running at http://localhost:${port}`);
});