// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);

var defaultLoc = 'NSW_PT131';
var useLoc = false;
var cachedLoc = null;
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
  var url = 'http://fj3k.com/pebble/';
  if (pos && !(pos.coords.latitude === 0 && pos.coords.longitude === 0)) {
    url += '?lat=' + pos.coords.latitude + '&lng=' + pos.coords.longitude;
  } else if (defaultLoc.length > 0) {
    url += '?acc=' + defaultLoc;
  }

  var keys = require('message_keys');
  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);

      for (var i = 0; i < json.weather.forecast.length && i < 8; i++) {
        var dictionary = {};
        var tempMax = json.weather.forecast[i].max;
//        var tempMin = parseInt(json.weather.forecast[i].min, 10);
        var icon = parseInt(json.weather.forecast[i].icon, 10);
  
        var icokey = keys.Forecast + i;
        var tempkey = keys.Temperature + i;
        dictionary[icokey] = icon;
        dictionary[tempkey] = (tempMax.length > 0) ? parseInt(tempMax, 10) : -40;
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
          console.log('D:' + parseInt(data[di], 10) + ' M:' + parseInt(max[di], 10) + ' T:' + parseInt(tick[di], 10));
          dictionary3[datakey] = parseInt(data[di], 10);
          dictionary3[maxkey] = parseInt(max[di], 10);
          dictionary3[tickey] = parseInt(tick[di], 10);
          dictionary3[negkey] = neg[di];
          sendData(dictionary3);
        }
      } 
    }
  );
}

function sendData(dict) {
  if (am_sending) {
    am_buffer.push(dict);
    console.log('Queued. Buffer size: ' + am_buffer.length);
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
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWeather();
  }
);
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getWeather();
  }
);
