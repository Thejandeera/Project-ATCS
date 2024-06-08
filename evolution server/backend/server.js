// server.js
const express = require('express');
const mongoose = require('mongoose');

const http = require("http"); // Import the HTTP module for creating an HTTP server
const WebSocket = require("ws"); // Import the WebSocket module
const cors = require("cors");

const tempSendRcv = require ("./routes/tempSendRcv");
const app = express();
const PORT = process.env.PORT || 5000; // Set your desired port

// Connect to MongoDB
mongoose.connect("mongodb+srv://thejan1010:oqcAVwREL3UHBXXW@atcs.xtgu8ti.mongodb.net/?retryWrites=true&w=majority&appName=ATCS")
.then(() => {
  console.log('MongoDB Connected');
})
.catch(err => console.error(err)); // Change console.log to console.error for better error handling

app.use(cors()); //Enable CORS for all routes

app.use(express.json());

app.use("/api/tempSendRcv", tempSendRcv);

// Define routes
app.get('/', (req, res) => {
  res.send('Hello from Express server!');
});

const server = http.createServer(app); // Create an HTTP server using Express app

const wss = new WebSocket.Server({ port: 3030 }); // Create a WebSocket server attached to the HTTP server

// Set wss as a local variable accessible from request object
app.locals.wss = wss;

wss.on("connection", function connection(ws) {
  console.log("Client connected");


  ws.on("message", function incoming(message) {
    console.log("Received: %s", message);
    ws.send("Hello, client!"); // Send a message to the client
  });

  ws.on("close", function () {
    console.log("Client disconnected");
  });
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`); // Use backticks for string interpolation
});
