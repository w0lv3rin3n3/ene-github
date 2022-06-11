const express = require("express");
const SerialPort = require("serialport");
// var fs = require("fs");
const mysql = require("mysql");
const app = express();
const cors = require("cors");
const { json } = require("express");
app.use(cors());

const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
  delimiter: "\r\n",
});

var port = new SerialPort("COM10", {
  baudRate: 115200,
  dataBits: 8,
  parity: "none",
  stopBits: 1,
  flowControl: false,
});

var dataFromSensor = {};
// var dataFromSensor = new Map();

port.pipe(parser);
/*sensorData = [] array of objects
sensorData.push(data) inside parser function
example: sensorData[0].temperature (results in number)
*/

parser.on("data", function (data) {
  // let sensor_params = data.split("@");
  // const sensor_id = sensor_params[0];
  // console.log('debug',sensor_params);
  const obj = JSON.parse(data);
  // if (sensor_id == "0") {
    // console.log(obj.temperature);

  //   console.log(obj.humidity);
  // } else if (sensor_id == "1") {
  //   console.log(obj.primul);

  //   console.log(obj.al_doilea);
  // }

  console.log("Received data from port: " + data);
  app.emit("data", data); //this should send the object to index.html
  dataFromSensor = obj;
});

app.get("/", (req, res) => {
  res.send("Server running on express");
});

app.get("/temperature-sensor", (req, res) => {
  res.send(dataFromSensor);
});

app.get("/2nd-sensor", (req, res) => {
  res.send(dataFromSensor);
});

app.get("/slider-input/send/:value", (req, res) => {
  const ledState = req.params.value;
  port.write(`{"ledState":${ledState}}`);
  // port.write(ledState);
  res.send(`Led state changed to ${ledState}`);
});
/*
tables: sensor1 [1], sensor2 [2]
columns: [1]: temperature, humidity, dateTime; [2]:
obj = {comanada1 : 'dwdwdadw', comanda 2} 
ruta/:comanda {fa ceva pe obj.comanda}
apelare: ruta/comanda1
*/

app.listen(3000, () => console.log("Example app listening on port 3000!"));