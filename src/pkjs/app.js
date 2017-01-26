// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);

var ready = false;

var maxSteps = localStorage.getItem('maxSteps');

var defaultLoc = 'NSW_PT131';
var useLoc = false;
var cachedLoc = null;
var cachedData = localStorage.getItem('dataCache');

var am_buffer = [];
var am_sending = false;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function weatherService(pos) {
  var url = 'http://fj3k.com/pebble/?n=' + Math.floor(Math.random() * 1000) + '&stamp=' + maxSteps;
  if (pos && !(pos.coords.latitude === 0 && pos.coords.longitude === 0)) {
    url += '&lat=' + pos.coords.latitude + '&lng=' + pos.coords.longitude;
  } else if (defaultLoc.length > 0) {
    url += '&acc=' + defaultLoc;
  }

  xhrRequest(url, 'GET', handleResponse);
}

function initBatt(battery) {
  if (battery) {
    battery.addEventListener('chargingchange', function () {
      battDate(battery.level * 100, battery.charging);
    });
    battery.addEventListener('levelchange', function () {
      battDate(battery.level * 100, battery.charging);
    });
    battDate(battery.level * 100, battery.charging);
  }
}

function battDate(phoneBatt, charging) {
  var d = new Date();
  if (!ready || (arguments.callee.lastSend && (arguments.callee.lastSend + 5 * 60 * 1000 > d.getTime()))) return;
  var keys = require('message_keys');
  var dictionary4 = {};
  var batterKey = keys.PhoneBattery;
  dictionary4[batterKey] = phoneBatt;
  var stepsKey = keys.StepCount;
  dictionary4[stepsKey] = maxSteps;
  sendData(dictionary4);
  arguments.callee.lastSend = d.getTime();    
}

function handleResponse(responseText) {
  cachedData = responseText;
  localStorage.setItem('dataCache', cachedData);
  parseResponse(responseText);
}

function parseResponse(jsonText) {
  var keys = require('message_keys');
  var json = JSON.parse(jsonText);

  for (var i = 0; i < json.weather.forecast.length && i < 8; i++) {
    var dictionary = {};
    var tempMax = json.weather.forecast[i].max;
    var tempMin = json.weather.forecast[i].min;
    var icon = parseInt(json.weather.forecast[i].icon, 10);

    var icokey = keys.Forecast + i;
    var tempkey = keys.Temperature + i;
    var mintempkey = keys.MinTemp + i;
    dictionary[icokey] = icon;
    dictionary[tempkey] = (tempMax.length > 0) ? parseInt(tempMax, 10) : -40;
    dictionary[mintempkey] = (tempMin.length > 0) ? parseInt(tempMin, 10) : -40;
    sendData(dictionary);
  }

  var dictionary2 = {};
  var statskey = keys.ShowStreams;
  dictionary2[statskey] = json.stats.active;
  sendData(dictionary2);
  var NOST = -2147483648;
  if (json.stats.active) {
    var data = [NOST, NOST, NOST, NOST];
    var max = [NOST, NOST, NOST, NOST];
    var tick = [NOST, NOST, NOST, NOST];
    var neg = [0, 0, 0, 0];
    for (var d = 0; d < json.stats.data.length && d < 4; d++) {
      if (typeof json.stats.data[d] == 'undefined') {
        //Send NOST
      } else if (Array.isArray(json.stats.data[d])) {
        for (var sd = 0; sd < 2; sd++) {
          data[d * 2 + sd] = json.stats.data[d][sd];
          if (json.stats.max[sd]) max[d * 2 + sd] = json.stats.max[sd];
          if (json.stats.tick[sd]) tick[d * 2 + sd] = json.stats.tick[sd];
          if (json.stats.neg[sd]) neg[d * 2 + sd] = json.stats.neg[sd];
        }
      } else {
        data[d] = json.stats.data[d];
        if (json.stats.max[d]) max[d] = json.stats.max[d];
        if (json.stats.tick[d]) tick[d] = json.stats.tick[d];
        if (json.stats.neg[d]) neg[d] = 1;
      }
    }

    for (var di = 0; di < 4; di++) {
      var dictionary3 = {};
      var datakey = keys.StatsData + di;
      var maxkey = keys.StatsMax + di;
      var tickey = keys.StatsTick + di;
      var negkey = keys.StatsNeg + di;
      dictionary3[datakey] = parseInt(data[di], 10);
      dictionary3[maxkey] = parseInt(max[di], 10);
      dictionary3[tickey] = parseInt(tick[di], 10);
      dictionary3[negkey] = neg[di];
      sendData(dictionary3);
    }
  } 
}


function sendData(dict) {
  if (am_sending) {
    am_buffer.push(dict);
  } else {
    am_sending = 1;
    Pebble.sendAppMessage(dict, sendDataAppSuccess, sendDataAppError);
    console.log('Sending. Buffer size: ' + am_buffer.length);
  }
}

function sendDataAppSuccess(e) {
  console.log('Success! Buffer size: ' + am_buffer.length);
  if (am_buffer.length > 0) {
    Pebble.sendAppMessage(am_buffer.shift(), sendDataAppSuccess, sendDataAppError);
  } else {
    am_sending = 0;
  }
}

function sendDataAppError(e) {
  console.log('Error! Buffer size: ' + am_buffer.length);
  if (am_buffer.length > 0) {
    Pebble.sendAppMessage(am_buffer.shift(), sendDataAppSuccess, sendDataAppError);
  } else {
    am_sending = 0;
  }
}

function locationSuccess(pos) {
  cachedLoc = pos;
  weatherService(pos);
}

function locationError(err) {
  weatherService(cachedLoc);
}

function getWeather() {
  if (cachedData) {
    parseResponse(cachedData);
  }
  if (useLoc) {
    //navigator.geolocation.getCurrentPosition( //one time
    navigator.geolocation.watchPosition( //whenever it changes
      locationSuccess,
      locationError,
      {timeout: 15000, maximumAge: 60000, enableHighAccuracy: false}
    );
  } else {
    weatherService();
  }
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    // Get the initial weather
    ready = true;

    getWeather();
    if (navigator.getBattery) {
      navigator.getBattery().then(function(battery) {
        initBatt(battery);
      });
    } else if (navigator.battery || navigator.mozBattery) { // deprecated battery object
      initBatt(navigator.battery || navigator.mozBattery);
    }
  }
);
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    cachedData = null;
    getWeather();

    console.log('Received message');
    var dict = e.payload;
    var steps = dict['StepCount'];
    if (steps > maxSteps) {
      maxSteps = steps;
      localStorage.setItem('maxSteps', maxSteps);
    }
  }
);
