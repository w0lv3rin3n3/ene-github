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
// var port = new SerialPort("COM5", {
//   baudRate: 115200,
//   dataBits: 8,
//   parity: "none",
//   stopBits: 1,
//   flowControl: false,
// });
// port.pipe(parser);


// Create data buffer
var dataFromSensor = {};
// var dataFromSensor = new Map();


/*sensorData = [] array of objects
sensorData.push(data) inside parser function
example: sensorData[0].temperature (results in number)
*/

// Parse JSON incoming data
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
  if(dataFromSensor.sensor_name == "ir")
  {
    addIRSensors(dataFromSensor)
    console.log("Inserted sensor " + dataFromSensor.sensor_name);
  }
    
  else
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
    arg1: dataFromSensor.arg1,
    arg2: dataFromSensor.arg2,
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
    command_name: dataFromSensor.command_name,
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

// Get temperature data
app.get("/dht11-sensor", (req, res) => {
  let sql = `SELECT * from sensors WHERE sensor_name = 'temp'`
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

// Get 2nd-sensor data
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

// Set ledState
app.get("/slider-input/send/:value", (req, res) => {
  const ledState = req.params.value;
  port.write(`{"ledState":${ledState}}`);
  // port.write(ledState);
  res.send(`Led state changed to ${ledState}`);
});

// Set commandIR
app.get("/commandIR/send/:value", (req, res) => {
  const command_name = req.params.value;
  let sql = `SELECT * from ir_data WHERE command_name = '${command_name}'`
  let query = db.query(sql, (err, results) => {
    if(err) {
      throw err
    }
    // for(let element of results) {
      // element.date = moment(element.date).format('YYYY-MM-DD HH:mm:ss')
    // }
    res.send(results);
  })
  // port.write(`{"protocol":${commandIR}, "address":${commandIR}, "command_code":${commandIR}}`);
  // port.write(ledState);
  // res.send(`Command ${command_name} is sent to TV`);
});


app.post('/commandIR/send', function(req, res) {
  console.log(req.body);
  const protocol = req.body.protocol;
  const command_code = req.body.command_code;
  const address = req.body.address;

  port.write(`{"protocol": "${protocol}", "address": ${address}, "command_code": ${command_code}}`);
  // port.write(ledState);
  return res.send(`Command ${command_code} is sent to TV`);
});


/*
tables: sensor1 [1], sensor2 [2]
columns: [1]: temperature, humidity, dateTime; [2]:
obj = {comanada1 : 'dwdwdadw', comanda 2} 
ruta/:comanda {fa ceva pe obj.comanda}
apelare: ruta/comanda1
*/

app.listen(3000, () => console.log("Example app listening on port 3000!"));