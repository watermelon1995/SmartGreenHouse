var express = require("express");
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var path = require("path");

var serialport = require("serialport");
var util = require("util");

app.use(express.static(path.join(__dirname, "/")));

var portnName = "/dev/ttyUSB0";
var serialport = require("serialport");
var util = require("util");

var portnName = "/dev/ttyUSB1";

var arduino = new serialport.SerialPort(portnName, {
  baudrate: 9600,
  parser: serialport.parsers.readline("\n")
});

var dataBegin;
var line = "";

arduino.on('open', function (err) {
    if (err) {
        console.log(err);
        return;
    }
    console.log('Serial Port Opened');
    arduino.on('data', function (data) {
        line = util.inspect(JSON.parse(data));
        console.log(line);
        io.emit("dataRdy", line.replace("\"", ""));
    });
});

io.on("connection", function(socket) {
    console.log("User connected");

/*    socket.on("changeHumidity", function(data){
        console.log("humidity    " + data);
        io.emit("dataRdy", "{ Humidity : " + data + " }");
    });

    socket.on("changeLight", function(data){
        console.log("light    " + data);
       io.emit("dataRdy", data);
    });

    socket.on("changeSoil", function(data){
        console.log("soil    " + data);
        io.emit("dataRdy", data);
    });*/
});

app.get('/', function(req, res){
    res.sendfile('graph.html');
});

io.on('connection', function(socket){
    console.log('a user connected');
});

http.listen(8000, function(){
    console.log('listening on 8000');
});
