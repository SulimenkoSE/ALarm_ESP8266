'use strict';

//Добавим всем функциям в прототип метод defer(ms), который вызывает функции через ms миллисекунд.
//аргументы должны корректно передаваться оригинальной функции.
Function.prototype.delay = function(ms) {
  let f = this;
  return function(...args) {
    setTimeout(() => f.apply(this, args), ms);
  }
};

 
function myAsyncFunction(loadFileJson) {
  return new Promise((resolve, reject) => {
    const xhr = new XMLHttpRequest();
    xhr.addEventListener("progress", updateProgress);
    xhr.addEventListener("load", transferComplete);
    xhr.addEventListener("error", transferFailed);
    xhr.addEventListener("abort", transferCanceled);
    xhr.addEventListener("loadend", loadEnd);
    
    xhr.open("GET", loadFileJson);
    xhr.responseType = "json";
    
    // When the request loads, check whether it was successful
    
    xhr.onload = function() {
        if (this.readyState == 4 && xhr.status == 200) {
            if (xhr.response === null){
              reject(Error('Didn\'t load successfully; error xhr.response: null'));
            } else {
              resolve(xhr.response);
            }
        } else {
            // If it fails, reject the promise with a error message
            reject(Error('Didn\'t load successfully; error code:' + xhr.statusText));
        }
    };
    
    // progress on transfers from the server to the client (downloads)
    function updateProgress (oEvent) {
      if (oEvent.lengthComputable) {
        var percentComplete = oEvent.loaded / oEvent.total * 100;
        // ...
      } else {
        // Unable to compute progress information since the total size is unknown
      }
    }
    
    function transferComplete(evt) {
      console.log("The transfer is complete - " + loadFileJson);
    }
    
    function transferFailed(evt) {
      console.log("An error occurred while transferring the file- " + loadFileJson);
    }
    
    function transferCanceled(evt) {
      console.log("The transfer has been canceled by the user - " + loadFileJson);
    }
    
    function loadEnd(e) {
      console.log("The transfer finished (although we don't know if it succeeded or not).");
    } 
        
        xhr.onerror = () => reject(Error('Error code:' + xhr.statusText));
        xhr.send();
      });
    }

function load() {
  
  let dataJSON;
  let dataBody = document.getElementsByTagName('body')[0].innerHTML;
  let new_string;
  myAsyncFunction('/configq.json')
    .then(
      function(result) {
          let dataJSON = result;
          for (var key in dataJSON) {
              new_string = dataBody.replace(new RegExp('{{' + key + '}}', 'g'), dataJSON[key]);
              dataBody = new_string;
          }
          document.getElementsByTagName('body')[0].innerHTML = new_string;
      }, 
      function(err) {
          alert(err); // Error: "It broke"
      })
    .catch(error => {
      console.log(error); // Error: Not Found
    });
}

function loadSensors() {
    let x = "";
    myAsyncFunction('/sensors.json').then(
        function(result) {
            let dataJSON = result;
            for (let key = 0; key < dataJSON.length; key++) {
                x += "<h1>" + dataJSON[key].name + "</h1>";
                for (let j in dataJSON[key].sensors) {
                    x += "<h2>" + dataJSON[key].sensors[j].name + "</h2>";
                }
            }
            document.getElementById("demo_3").innerHTML = x;
        }, 
        function(err) {
            console.log(err); // Error: "It broke"
        });
}

function loadNameZons() {
  
    myAsyncFunction('/sensors.json').then(
        function(result) {
            let dataJSON = result;
            let objsel = document.getElementById("zona_name");
            for (let key = 0; key < dataJSON.length; key++) {
                objsel.options[objsel.options.length] = new Option(dataJSON[key].name, "" + key + "");
            }
        }, 
        function(err) {
            console.log(err); // Error: "It broke"
        });  
}

function loadZons(sel) {
  
  let txt = "", k = 0;

  if (sel === undefined) sel = null;
  
  myAsyncFunction('/sensors.json').then(function(result) {
    let dataJSON = result;
    txt += "<table border='1'>";
    for (let key = 0; key < dataJSON.length; key++) {
        for (let j in dataJSON[key].sensors) {
        if (key === sel) {
            if (k === 0) {
                txt += " <caption>Состояние датчиков зоны  " + dataJSON[key].name + "</caption>";
                txt += "<tr><th>id датчика</th><th>Название</th><th>Сосотояние On/Off</th></tr>";
                k = 1;
            }
            txt += "<tr><td>" + dataJSON[key].sensors[j].id_sensor + "</td><td>" + dataJSON[key].sensors[j].name + "</td><td>" + dataJSON[key].sensors[j].AlarmStatusAlarms + "</td></tr>";
        }
        }
    }
    txt += "</table>";
    document.getElementById("table_main").innerHTML = txt;
  }, function(err) {
    console.log(err); // Error: "It broke"
  });  
}

function loadConfigs() {

    document.getElementById("demo_1").innerHTML = "<h2> 3 </h2>";
    myAsyncFunction('/config.json').then(
        function(result) {
            //console.log(result); // "Stuff worked!"
            let myJSON = JSON.stringify(result, null, 3);
            document.getElementById("demo_4").innerHTML = myJSON;
            getStatusPin("NasosaStatus", 4);
        }, 
        function(err) {
            console.log(err); // Error: "It broke"
        });
}


function val(id) {
    let v = document.getElementById(id).value;
    return v;
}

function send_request(submit, server) {
    let request = new XMLHttpRequest();
    request.open("GET", server, true);
    request.send();
    save_status(submit, request);
}

function save_status(submit, request) {
    let old_submit = submit.value;
    request.onreadystatechange = function () {
        if (request.readyState != 4) return;
        submit.value = request.responseText;
        setTimeout(function () {
            submit.value = old_submit;
            submit_disabled(false);
        }, 1000);
    };
    submit.value = 'Подождите...';
    submit_disabled(true);
}

function submit_disabled(request) {
    let inputs = document.getElementsByTagName("input");
    for (let i = 0; i < inputs.length; i++) {
        if (inputs[i].type === 'submit') { inputs[i].disabled = request; }
    }
}

function toggle(target) {
    let curVal = document.getElementById(target).className;
    document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}

function set_ssdp(submit) {
    let server = "/ssdp?ssdp=" + val('ssdp');
    send_request(submit, server);
    document.getElementById('ssdp_t').innerHTML = val('ssdp');
}

function sendData(led) {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 200) {
            //document.getElementById("LEDState").innerHTML =
            //this.responseText;
        }
    };
    xhttp.open("GET", "setLED?LEDstate=" + led, true);
    xhttp.send();
}

function getDataADC() {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 200) {
            //document.getElementById("ADCValue").innerHTML =
            //this.responseText;
        }
    };
    xhttp.open("GET", "readADC", true);
    xhttp.send();
}

function getDataTemp(idElementTemp, number) {
    var xhttp = new XMLHttpRequest();
    
    xhttp.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 200) {
            //document.getElementById(idElementTemp).innerHTML =
            //this.responseText;
        }
    };
    xhttp.open("GET", "readTemperatura?Num=" + number, true);
    xhttp.send();
}

function getStatusPin(idElementPin, pin) {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState === 4 && this.status === 200) {
        //document.getElementById(idElementPin).innerHTML =
        //this.responseText;
    }
  };
  xhttp.open("GET", "statusPin?statusPin=" + pin, true);
  xhttp.send();
}

function funonload() {
    getDataADC();
    sendData(3);
    getStatusPin("NasosaStatus", 4);
    getStatusPin("Activation_Sensor_Status", 5);
    getDataTemp("Temp_0", 0);
    getDataTemp("Temp_1", 1);
    load();
}

function foo() {
    document.getElementById('btn').value = 'Ваш текст:' + document.getElementById("txt").value + '.';
}

function change_myselect(sel) {

    if (sel === "") loadZons(null);

    if (sel === "0") loadZons(0);

    if (sel === "1") loadZons(1);

    if (sel === "2") loadZons(2);
}

let promiseCount = 0;
function testPromise() {
  let thisPromiseCount = ++promiseCount;
  let log = document.getElementById('log');
  
  if ("Promise" in window) {
    let btn = document.getElementById("btn_2");
    btn.addEventListener("click",testPromise);
  } else {
    log.innerHTML = "Демонстрация невозможна, поскольку ваш браузер не поддерживает интерфейс <code>Promise<code>.";
  }
  
  log.insertAdjacentHTML('beforeend', thisPromiseCount + ') Запуск синхронного кода)\r\n');

  // Создаём промис, возвращающее 'result' (по истечении 3-х секунд)
  var p1 = new Promise(
    // Функция разрешения позволяет завершить успешно или 
    // отклонить промис
    function(resolve, reject) {       
      log.insertAdjacentHTML('beforeend', thisPromiseCount + ') Запуск промиса )');
      // Это всего лишь пример асинхронности
      window.setTimeout(
        function() {
          // Обещание исполнено!
          resolve(thisPromiseCount);
        }, Math.random() * 1000 );
    });

  // Указываем, что сделать с исполненным промисм
  p1.then(
    // Записываем в протокол
    function(val) {
      log.insertAdjacentHTML('beforeend', val + ') Обещание исполнено (асинхронный код завершён)');
    });

  log.insertAdjacentHTML('beforeend', thisPromiseCount + ') Обещание создано (синхронный код завершён)');
}

/*function imgLoad(url) {
    // Create new promise with the Promise() constructor;
    // This has as its argument a function
    // with two parameters, resolve and reject
    return new Promise(function(resolve, reject) {
      // Standard XHR to load an image
      var request = new XMLHttpRequest();
      request.open('GET', url);
      request.responseType = 'blob';
      // When the request loads, check whether it was successful
      request.onload = function() {
        if (request.status === 200) {
        // If successful, resolve the promise by passing back the request response
          resolve(request.response);
        } else {
        // If it fails, reject the promise with a error message
          reject(Error('Image didn\'t load successfully; error code:' + request.statusText));
        }
      };
      request.onerror = function() {
      // Also deal with the case when the entire request fails to begin with
      // This is probably a network error, so reject the promise with an appropriate message
          reject(Error('There was a network error.'));
      };
      // Send the request
      request.send();
    });
  }
  // Get a reference to the body element, and create a new image object
  var body = document.querySelector('body');
  var myImage = new Image();
  // Call the function with the URL we want to load, but then chain the
  // promise then() method on to the end of it. This contains two callbacks
  imgLoad('myLittleVader.jpg').then(function(response) {
    // The first runs when the promise resolves, with the request.response
    // specified within the resolve() method.
    var imageURL = window.URL.createObjectURL(response);
    myImage.src = imageURL;
    body.appendChild(myImage);
    // The second runs when the promise
    // is rejected, and logs the Error specified with the reject() method.
  }, function(Error) {
    console.log(Error);
  });
  
  httpGet('/page-not-exists')
    .then(response => JSON.parse(response))
    .then(user => httpGet(`https://api.github.com/users/${user.name}`))
    .then(githubUser => {
      githubUser = JSON.parse(githubUser);
  
      let img = new Image();
      img.src = githubUser.avatar_url;
      img.className = "promise-avatar-example";
      document.body.appendChild(img);
  
      return new Promise((resolve, reject) => {
        setTimeout(() => {
          img.remove();
          resolve();
        }, 3000);
      });
    })
    .catch(error => {
      alert(error); // Error: Not Found
    });
    
    
    //Добавим всем функциям в прототип метод defer(ms), который вызывает функции через ms миллисекунд.
    Function.prototype.defer = function(ms) {
      setTimeout(this, ms);
    };

    function f() {
      alert("Hello!");
    }

    f.defer(1000); // выведет "Hello!" через 1 секунду
  
  //Добавим всем функциям в прототип метод defer(ms), который вызывает функции через ms миллисекунд.
  //аргументы должны корректно передаваться оригинальной функции.
  Function.prototype.defer = function(ms) {
    let f = this;
    return function(...args) {
      setTimeout(() => f.apply(this, args), ms);
    }
  };

  // check it
  function f(a, b) {
    alert( a + b );
  }

  f.defer(1000)(1, 2); // выведет 3 через 1 секунду.
  
  <script>
            class Clock {
              constructor({ template }) {
                this.template = template;
              }
            
              render() {
                let date = new Date();
            
                let hours = date.getHours();
                if (hours < 10) hours = '0' + hours;
            
                let mins = date.getMinutes();
                if (mins < 10) mins = '0' + mins;
            
                let secs = date.getSeconds();
                if (secs < 10) secs = '0' + secs;
            
                let output = this.template
                  .replace('h', hours)
                  .replace('m', mins)
                  .replace('s', secs);
            
                console.log(output);
              }
            
              stop() {
                clearInterval(this.timer);
              }
            
              start() {
                this.render();
                this.timer = setInterval(() => this.render(), 1000);
              }
            }
            
            
            let clock = new Clock({template: 'h:m:s'});
            clock.start();
          </script>*/
          
    