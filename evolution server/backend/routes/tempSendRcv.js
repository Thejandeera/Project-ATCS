const express = require('express');
const router = express.Router();
const WebSocket = require('ws'); // Import WebSocket module
const Temperature = require('../models/temperature');

// POST route to receive temperature data
router.post('/temperature', async (req, res) => {
  try {
    const { temperature, motion } = req.body; // Get temperature and motion from request body
    console.log('V',temperature);
    const newTemperature = new Temperature({ temperature, motion }); // Save temperature and motion to MongoDB
    await newTemperature.save();
    console.log('V',temperature);
    // Emit temperature and motion update to WebSocket clients
    req.app.locals.wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify({ temperature, motion }));
      }
    });

    res.status(201).json({ message: 'Temperature data received and saved successfully' });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
});


module.exports = router;