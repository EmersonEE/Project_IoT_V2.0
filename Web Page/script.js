let bot = {
  TOKEN: "6335002576:AAHZLHuTtUlXVMue8DQXW0PmndSIdx1LbhU",
  ID: "1527971462",
};

var firebaseConfig = {
  apiKey: "AIzaSyApgj_Z88Xrp9RpeNWS7QekfQO7UCzoLBQ",
  authDomain: "iot-project-322be.firebaseapp.com",
  projectId: "iot-project-322be",
  storageBucket: "iot-project-322be.appspot.com",
  messagingSenderId: "304517409136",
  appId: "1:304517409136:web:466aa6f8fa6575264607b5",
  measurementId: "G-957Z9JW6LB",
};
// document.addEventListener('DOMContentLoaded', function () {

function Carga1Encendida() {
  let message = "Luz de la Pc Encendida!!! ";

  fetch(
    `https://api.telegram.org/bot${bot.TOKEN}/sendMessage?chat_id=${
      bot.ID
    }&text=${encodeURIComponent(message)}`,
    {
      method: "POST",
    }
  );
}

function Carga1Apagada() {
  let messageOff1 = "Luz de la Pc Apagada!!! ";

  fetch(
    `https://api.telegram.org/bot${bot.TOKEN}/sendMessage?chat_id=${
      bot.ID
    }&text=${encodeURIComponent(messageOff1)}`,
    {
      method: "POST",
    }
  );
}

// Initialize Firebase
firebase.initializeApp(firebaseConfig);
var database = firebase.database();
var Led1Status;
var Led2Status;
var Led3Status;
var Led4Status;

database.ref().on("value", function (snap) {
  Led1Status = snap.val().Led1Status;
  console.log("Cambio en el estado:", Led1Status);

  if (Led1Status === false) {
    console.log("Carga 1 Apagada");
  } else {
    console.log("Carga 1 Encendida");
  }
  toggleSwitch();
});
document.getElementById("miBoton").addEventListener("click", function () {
  var firebaseRef = firebase.database().ref().child("Led1Status");

  if (Led1Status === false) {
    firebaseRef.set(true);
    Led1Status = true;
  } else {
    firebaseRef.set(false);
    Led1Status = false;
  }
  toggleSwitch();
});

database.ref().on("value", function (snap) {
  Led2Status = snap.val().Led2Status;
  console.log("Cambio en el estado:", Led2Status);

  if (Led2Status === false) {
    console.log("Carga 2 Apagada");
    Carga1Apagada();
  } else {
    console.log("Carga 2 Encendida");
    Carga1Encendida();
  }
  toggleSwitch2();
});

document.getElementById("miBoton2").addEventListener("click", function () {
  var firebaseRef = firebase.database().ref().child("Led2Status");

  if (Led2Status === false) {
    firebaseRef.set(true);
    Led2Status = true;
  } else {
    firebaseRef.set(false);
    Led2Status = false;
  }
  toggleSwitch2();
});

database.ref().on("value", function (snap) {
  Led3Status = snap.val().Led3Status;
  console.log("Cambio en el estado:", Led3Status);

  if (Led3Status === false) {
    console.log("Carga 3 Apagada");
  } else {
    console.log("Carga 3 Encendida");
  }
  toggleSwitch3();
});

document.getElementById("miBoton3").addEventListener("click", function () {
  var firebaseRef = firebase.database().ref().child("Led3Status");

  if (Led3Status === false) {
    firebaseRef.set(true);
    Led3Status = true;
  } else {
    firebaseRef.set(false);
    Led3Status = false;
  }
  toggleSwitch3();
});

database.ref().on("value", function (snap) {
  Led4Status = snap.val().Led4Status;
  console.log("Cambio en el estado:", Led4Status);

  if (Led4Status === false) {
    console.log("Carga 4 Apagada");
  } else {
    console.log("Carga 4 Encendida");
  }
  toggleSwitch4();
});

document.getElementById("miBoton4").addEventListener("click", function () {
  var firebaseRef = firebase.database().ref().child("Led4Status");

  if (Led4Status === false) {
    firebaseRef.set(true);
    Led4Status = true;
  } else {
    firebaseRef.set(false);
    Led4Status = false;
  }
  toggleSwitch4();
});

function toggleSwitch() {
  var button = document.querySelector(".switch-button");

  if (Led1Status === true) {
    button.classList.add("on");
  } else {
    button.classList.remove("on");
  }
}

function toggleSwitch2() {
  var button = document.querySelector(".switch-button2");
  if (Led2Status === true) {
    button.classList.add("on");
  } else {
    button.classList.remove("on");
  }
}

function toggleSwitch3() {
  var button = document.querySelector(".switch-button3");
  if (Led3Status === true) {
    button.classList.add("on");
  } else {
    button.classList.remove("on");
  }
}
function toggleSwitch4() {
  var button = document.querySelector(".switch-button4");
  if (Led4Status === true) {
    button.classList.add("on");
  } else {
    button.classList.remove("on");
  }
}
