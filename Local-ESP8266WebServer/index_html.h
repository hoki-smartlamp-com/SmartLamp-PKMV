const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Hoki SmartLamp</title>
    <link rel="manifest" href="manifest.json" />
    <link rel="shortcut icon" href="/favicon.ico" type="image/x-icon" />
    <link rel="icon" href="/favicon.ico" type="image/x-icon" />
    <style>
      :root {
        --bg: #212121;
        --disabled: #4d4d4d;
      }

      * {
        padding: 0;
        margin: 0;
      }

      body {
        height: 100%;
        width: 100%;
        overflow: hidden !important;
        background-color: var(--bg);
        font-weight: 100;
        color: #f0f0f0;
        font-family: Verdana, Geneva, Tahoma, sans-serif;
      }

      header {
        padding: 20px 30px;
      }

      header h2 {
        font-weight: 100;
        font-size: 1.7em;
        text-transform: uppercase;
      }

      header svg {
        margin-top: 3.5px;
      }

      header.flex {
        justify-content: space-between;
      }

      button:hover {
        filter: brightness(0.9);
        cursor: pointer;
      }

      button:focus {
        /* filter: bright(0.8); */
        outline: none;
      }

      button:disabled {
        background-color: var(--disabled);
      }

      .btn {
        font-weight: 100 !important;
        width: 100%;
        padding: 15px 20px;
        border: 0;
        border-radius: 2px;
        color: white;
        font-weight: bold;
        text-transform: uppercase;
        font-size: 1em;
      }

      .apply {
        flex: 1;
        background-color: #0f8db3;
        /* flex: 2; */
      }

      .power {
        padding: 15px 0;
        border: 0;
        width: 100%;
        height: 60px;
        /* border-radius: 100%; */
        background-color: #e62a2a;
        color: white;
        font-weight: bold;
      }

      .power p {
        font-size: 1rem;
        font-weight: 400;
        padding: 0 10px;
        margin: auto;
      }

      .power.off {
        background-color: var(--disabled);
      }

      .container {
        padding: 8vw;
      }

      .body {
        height: 65vh;
        position: relative;
        display: flex;
      }

      .flex {
        display: flex;
      }

      .inline-flex {
        display: inline-flex;
      }

      .text-center {
        text-align: center;
      }

      .hide {
        display: none !important;
      }

      .hidden {
        opacity: 0 !important;
        visibility: hidden !important;
        transition: 0.7s all;
      }

      .modal,
      .loading {
        opacity: 1;
        position: fixed;
        z-index: 10;
        visibility: visible;
        top: 0;
        left: 0;
        width: 100vw;
        height: 100vh;
        background-color: rgba(0, 0, 0, 0.5);
        backdrop-filter: blur(3px);
        display: flex;
        transition: 0.35s all;
      }

      input[type="text"],
      input[type="password"] {
        outline: none;
        background-color: rgba(60, 60, 60, 0.5);
        width: 100%;
        padding: 10px 15px;
        border: 1px solid #555;
        border-radius: 2px;
        color: #fff;
        box-sizing: border-box;
      }

      .modal .container {
        background-color: var(--bg);
        border-radius: 2px;
        width: 84vw;
        margin: auto 6vw;
        box-shadow: 2px 2px 15px rgba(0, 0, 0, 0.7);
        /* transition: 1s display; */
      }

      .modal h3 {
        font-weight: 100;
      }

      .ml-1 {
        margin-left: 10px;
      }

      .ml-2 {
        margin-left: 20px;
      }

      .ml-3 {
        margin-left: 30px;
      }

      .mt-1 {
        margin-top: 10px;
      }

      .mt-2 {
        margin-top: 20px;
      }

      .mt-3 {
        margin-top: 30px;
      }

      .my-auto {
        margin-top: auto;
        margin-bottom: auto;
      }

      .loader {
        margin: auto;
        border: 5px solid #ffffffa6;
        border-radius: 50%;
        width: 10vw;
        height: 10vw;
        -webkit-animation: spin 2s linear infinite;
        /* Safari */
        animation: spin 2s linear infinite;
        border-top: 5px solid #ffffff;
        border-bottom: 5px solid #ffffff;
      }

      @keyframes spin {
        0% {
          transform: rotate(0deg);
        }

        100% {
          transform: rotate(360deg);
        }
      }
    </style>
  </head>

  <body>
    <header class="flex">
      <h2>Hoki SmartLamp</h2>
    </header>
    <div class="loading">
      <div class="loader"></div>
    </div>
    <div class="container body">

      <div class="modal hidden" id="wifi_connect">
        <div class="container">
          <p id="wifi_status"></p>
        </div>
      </div>

      <form action="/wifi/set" method="post" id="submitForm" class="my-auto text-center">
        <img src="/img/icons-512.png" height="160px" width="160px">
        <h4 class="mt-2">Sambungkan dengan Wi-Fi terlebih dahulu untuk dapat diakses secara online</h4>
        <input type="text" name="ssid" id="ssid" placeholder="SSID" class="mt-2">
        <input type="password" name="password" id="wifi_password" placeholder="Password" class="mt-2">
        <div class="flex mt-2">
          <button type="submit" class="btn apply" onclick="connectWifi()">Connect</button>
        </div>
      </form>
    </div>
    <script src="app.js"></script>
    <script>
      window.addEventListener("load", () => {
        registerServiceWorker();
      });
      function registerServiceWorker() {
        if ("serviceWorker" in navigator) {
          navigator.serviceWorker
            .register("/sw.js")
            .then((reg) => {
              console.log("Service Worker registered successfully");
            })
            .catch((e) =>
              console.error("Error during service worker registration:", e)
            );
        } else {
          console.warn("Service Worker is not supported");
        }
      }
    </script>
  </body>

</html>
)=====";

const char app_js[] PROGMEM = R"=====(
  let ssid = document.getElementById("ssid");
  let wifiPassword = document.getElementById("wifi_password");

  let wifi = JSON.parse(localStorage.getItem("wifi"));
  if (wifi) {
    ssid.value = wifi.ssid;
    wifiPassword.value = wifi.password;
  }

  function connectWifi() {
    localStorage.setItem(
      "wifi",
      JSON.stringify({
        ssid: ssid.value,
        password: wifiPassword.value,
      })
    );
  }

  let ip_local = "http://192.168.4.1"; // ganti sama IP baru kalau udah dibuat mode AP

  let modalWifi = document.getElementById("wifi_connect");
  fetch(ip_local + "/wifi")
    .then((response) => response.json())
    .then((json) => {
      if (json.status == "connected" || json.status == "connecting") {
        modalWifi.classList.remove("hidden");

        let wifi_connected = "Wi-Fi sudah terkoneksi, silahkan kunjungi <a href='https://hoki-smartlamp.com'>www.hoki-smartlamp.com</a> dengan memakai Wi-Fi rumah anda atau menggunakan Internet Data"
        let wifi_connecting = "Sedang menghubungkan Wi-Fi..."

        setTimeout(() => {
          if (json.status == "connected") 
            window.close('','_parent','');
          else location.reload();
        }, 15000);

        document.getElementById("wifi_status").innerHTML = (json.status == "connected") ? wifi_connected : wifi_connecting
      } else {
        modalWifi.classList.add("hidden");
      }
      loadingState("off");
    });

  function loadingState(status) {
    let newLoading = document.createElement("div");
    let loader = document.createElement("div");
    newLoading.classList.add("loading");
    loader.classList.add("loader");
    newLoading.appendChild(loader);
    if (status) {
      document.body.removeChild(document.querySelector(".loading"));
    } else document.body.appendChild(newLoading);
  }
)=====";
