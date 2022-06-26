const express = require("express");
const SerialPort = require("serialport");
const mysql = require("mysql");
const app = express();
const cors = require("cors");
const { json } = require("express");
const moment = require("moment");
app.use(cors());

var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: false}));
app.use(bodyParser.json());

const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
  delimiter: "\r\n",
});

// Create port
var port = new SerialPort("COM10", {
  baudRate: 115200,
  dataBits: 8,
  parity: "none",
  stopBits: 1,
  flowControl: false,
});
port.pipe(parser);


// Create data buffer
var dataFromSensor = {};

// Parse JSON incoming data
parser.on("data", function (data) {
  const obj = JSON.parse(data);

  console.log("Received data from port: " + data);
  app.emit("data", data); //this should send the object to index.html
  dataFromSensor = obj;
  // console.log(Object.keys(dataFromSensor).length);
  if(dataFromSensor.sensor_name == "ir")
  {
    addIRSensors(dataFromSensor)
    console.log("Inserted sensor " + dataFromSensor.sensor_name);
  }
    
  else if(Object.keys(dataFromSensor).length > 1 && dataFromSensor.sensor_name != "ir")
    addSensorsToDb(dataFromSensor)
});

// Create connection
const db = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: 'Mucidegaina.99',
  database: 'esp_sensors_db'
  // port: 3306
}) 

// Connect to MySql
db.connect(err => {
  if(err) {
    throw err
  }
  console.log('MySQL Connected');
})

// Create Database
app.get('/createdb', (req, res) => {
  let sql = 'CREATE DATABASE esp_sensors_db'
  db.query(sql, err => {
    if(err) {
      throw err;
    }
    res.send('Database Created');
  })
})

// Create Table
app.get('/create-sensors-table', (req, res) => {
  let sql = 'CREATE TABLE sensors(id int AUTO_INCREMENT, sensor_name VARCHAR(255), arg1 VARCHAR(255), arg2 VARCHAR(255), PRIMARY KEY(id))'
  db.query(sql, err => {
    if(err) {
      throw err;
    }
    res.send('Sensors table Created');
  })
})

// Insert sensor
app.get('/insert-sensor', (req, res) => {
var today = moment().toDate()
today = moment(today).format('YYYY-MM-DD HH:mm:ss')
  let post = {sensor_name: `temp`, arg1: '25', arg2: ``, date: `${today}`};
  let sql = 'INSERT INTO sensors SET ?'
  let query = db.query(sql, post, err => {
    if(err) {
      throw err
    }
    res.send('Sensor added');
  })
})

function addSensorsToDb(dataFromSensor) {
  var today = moment().toDate()
  today = moment(today).format('YYYY-MM-DD HH:mm:ss')
  let post = {
    sensor_name: dataFromSensor.sensor_name,
    value1: dataFromSensor.value1,
    value2: dataFromSensor.value2,
    date: today
  };
  let sql = 'INSERT INTO sensors SET ?'
  let query = db.query(sql, post, err => {
    if(err) {
      throw err
    }
  })
}

function addIRSensors(dataFromSensor) {
  var today = moment().toDate()
  today = moment(today).format('YYYY-MM-DD HH:mm:ss')
  let post = {
    sensor_name: dataFromSensor.sensor_name,
    command_name: document.getElementById("btn1"),
    protocol: dataFromSensor.protocol,
    address: dataFromSensor.address,
    command_code: dataFromSensor.command_code
  };
  let sql = 'INSERT INTO ir_data SET ?'
  let query = db.query(sql, post, err => {
    if(err) {
      throw err
    }
  })
}

// Select sensor
app.get('/get-sensor', (req, res) => {
  let sql = 'SELECT * from sensors'
  let query = db.query(sql, (err, results) => {
    if(err) {
      throw err
    }
    console.log(results);
    res.send('Sensor details fetched');
  })
})

// Update sensor
app.get('/update-sensor/:id', (req, res) => {
  let newName = 'Updated name';
  let sql = `UPDATE sensors SET sensor_name = '${newName}' WHERE id = ${req.params.id}`
  let query = db.query(sql, err => {
    if(err) {
      throw err
    }
    res.send('Sensor updated');
  })
})

// Delete sensor
app.get('/delete-sensor/:id', (req, res) => {
  let newName = 'Updated name';
  let sql = `DELETE FROM sensors WHERE id = ${req.params.id}`
  let query = db.query(sql, err => {
    if(err) {
      throw err
    }
    res.send('Sensor deleted');
  })
})

// Root
app.get("/", (req, res) => {
  res.send("Server running on express");
});

// Get dht11 data
app.get("/dht11-sensor", (req, res) => {
  let sql = `SELECT * from sensors WHERE sensor_name = 'dht11_sensor'`
  let query = db.query(sql, (err, results) => {
    if(err) {
      throw err
    }
    for(let element of results) {
      element.date = moment(element.date).format('YYYY-MM-DD HH:mm:ss')
    }
    res.send(results);
  })
});

// Get gas sensor data
app.get("/gas-sensor", (req, res) => {
  let sql = `SELECT * from sensors WHERE sensor_name = 'gas_sensor'`
  let query = db.query(sql, (err, results) => {
    if(err) {
      throw err
    }
    for(let element of results) {
      element.date = moment(element.date).format('YYYY-MM-DD HH:mm:ss')
    }
    res.send(results);
  })
});

// Get LDR data
app.get("/LDR-sensor", (req, res) => {
  let sql = `SELECT * from sensors WHERE sensor_name = 'LDR_sensor'`
  let query = db.query(sql, (err, results) => {
    if(err) {
      throw err
    }
    for(let element of results) {
      element.date = moment(element.date).format('YYYY-MM-DD HH:mm:ss')
    }
    res.send(results);
  })
});

// Get PIR data
app.get("/PIR-sensor", (req, res) => {
  let sql = `SELECT * from sensors WHERE sensor_name = 'PIR_sensor'`
  let query = db.query(sql, (err, results) => {
    if(err) {
      throw err
    }
    for(let element of results) {
      element.date = moment(element.date).format('YYYY-MM-DD HH:mm:ss')
    }
    res.send(results);
  })
});

// Set ledState
app.get("/slider-input/send/:value", (req, res) => {
  const ledState = req.params.value;
  port.write(`{"ledState":${ledState}}`);
  // port.write(ledState);
  res.send(`Led state changed to ${ledState}`);
});

// Set commandIR
// app.get("/commandIR/send/:value", (req, res) => {
//   const command_name = req.params.value;
//   let sql = `SELECT * from ir_data WHERE command_name = '${command_name}'`
//   let query = db.query(sql, (err, results) => {
//     if(err) {
//       throw err
//     }
//     // for(let element of results) {
//       // element.date = moment(element.date).format('YYYY-MM-DD HH:mm:ss')
//     // }
//     res.send(results);
//   })
//   // port.write(`{"protocol":${commandIR}, "address":${commandIR}, "command_code":${commandIR}}`);
//   // port.write(ledState);
//   // res.send(`Command ${command_name} is sent to TV`);
// });


// Set stepperState
app.get("/stepper-input/send/:value", (req, res) => {
  const stepperState = req.params.value;
  port.write(`{"stepperState":${stepperState}}`);
  // port.write(ledState);
  res.send(`Stepper state changed to ${stepperState}`);
});

// Send commandIR
app.post('/commandIR/send', function(req, res) {
  console.log(req.body);
  const protocol = req.body.protocol;
  const command_code = req.body.command_code;
  const address = req.body.address;

  port.write(`{"protocol": "${protocol}", "address": ${address}, "command_code": ${command_code}}`);
  // port.write(ledState);
  return res.send(`Command ${command_code} is sent to TV`);
});

// Receive commandIR
app.get('/commandIR/get', function(req, res) {
  port.write(`{"receive": "commandIR"}`);
  return res.send(`Command requested`);
});

app.listen(3000, () => console.log("Example app listening on port 3000!"));