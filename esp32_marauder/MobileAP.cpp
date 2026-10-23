#include "MobileAP.h"
#include "CommandLine.h"

extern CommandLine cli_obj;

#define LOG_BUFFER_SIZE 4096
static char logBuffer[LOG_BUFFER_SIZE];
static size_t logHead = 0;
static bool logWrapped = false;

void mobileLogWrite(uint8_t c) {
  logBuffer[logHead] = (char)c;
  logHead = (logHead + 1) % LOG_BUFFER_SIZE;
  if (logHead == 0) logWrapped = true;
}

String getMobileLogs() {
  String out = "";
  if (logWrapped) {
    out.reserve(LOG_BUFFER_SIZE);
    for (size_t i = logHead; i < LOG_BUFFER_SIZE; i++) {
      out += logBuffer[i];
    }
  } else {
    out.reserve(logHead);
  }
  for (size_t i = 0; i < logHead; i++) {
    out += logBuffer[i];
  }
  return out;
}

const char MOBILE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Flipper Killer S3 · Marauder</title>
<style>
:root {
  --bg: #0d1117;
  --panel: #161b22;
  --border: #30363d;
  --accent: #58a6ff;
  --accent-hover: #1f6feb;
  --danger: #f85149;
  --success: #3fb950;
  --text: #c9d1d9;
  --text-muted: #8b949e;
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
body { background: var(--bg); color: var(--text); padding: 12px; }
header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 12px; }
h1 { font-size: 1.15rem; font-weight: 700; color: #fff; }
.badge { background: #238636; color: #fff; font-size: 0.7rem; padding: 3px 8px; border-radius: 12px; font-weight: 600; }
.grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 8px; margin-bottom: 12px; }
button {
  background: var(--panel); border: 1px solid var(--border); color: var(--text);
  padding: 10px 8px; border-radius: 8px; font-size: 0.82rem; font-weight: 600;
  display: flex; align-items: center; justify-content: center; gap: 6px; cursor: pointer;
  transition: all 0.15s ease;
}
button:active { transform: scale(0.97); }
button.danger { background: rgba(248,81,73,0.15); border-color: var(--danger); color: #ff7b72; grid-column: span 2; }
button.primary { background: rgba(88,166,255,0.15); border-color: var(--accent); color: var(--accent); }
#term {
  background: #000; color: #39ff14; font-family: monospace; font-size: 0.78rem;
  padding: 10px; border-radius: 8px; border: 1px solid var(--border);
  height: 260px; overflow-y: auto; white-space: pre-wrap; word-break: break-all;
  margin-bottom: 10px;
}
.cli-box { display: flex; gap: 6px; }
input[type="text"] {
  flex: 1; background: var(--panel); border: 1px solid var(--border);
  color: #fff; padding: 10px 12px; border-radius: 8px; font-size: 0.9rem;
}
input[type="text"]:focus { outline: none; border-color: var(--accent); }
.send-btn { background: var(--accent); color: #000; border: none; padding: 0 16px; border-radius: 8px; font-weight: 700; }
</style>
</head>
<body>
<header>
  <h1>⚡ Flipper Killer S3</h1>
  <span class="badge">STANDALONE AP</span>
</header>

<div class="grid">
  <button class="danger" onclick="cmd('stopscan')">🛑 Detener Todo (stopscan)</button>
  <button class="primary" onclick="cmd('scanap')">📡 Escanear Wi-Fi</button>
  <button class="primary" onclick="cmd('sniffbeacon')">📻 Sniff Beacons</button>
  <button onclick="cmd('sniffpmkid')">🔐 Capturar PMKID</button>
  <button onclick="cmd('sniffpwn')">😈 Sniff Pwnagotchi</button>
  <button onclick="cmd('attack -t apple')">🍎 BLE Apple Spam</button>
  <button onclick="cmd('attack -t android')">🤖 BLE Android Spam</button>
  <button onclick="cmd('attack -t windows')">🪟 BLE Windows Spam</button>
  <button onclick="cmd('list -a')">📋 Ver APs Detectados</button>
</div>

<pre id="term">Conectado a Flipper Killer S3. Esperando datos...</pre>

<div class="cli-box">
  <input type="text" id="cmdInput" placeholder="Ej: help, channel 6, reboot..." onkeydown="if(event.key==='Enter')send();">
  <button class="send-btn" onclick="send()">Enviar</button>
</div>

<script>
let term = document.getElementById('term');
let lastLen = 0;

function cmd(val) {
  fetch('/api/cmd?c=' + encodeURIComponent(val));
}

function send() {
  let el = document.getElementById('cmdInput');
  let val = el.value.trim();
  if (val) {
    cmd(val);
    el.value = '';
  }
}

function updateLog() {
  fetch('/api/log')
    .then(r => r.text())
    .then(txt => {
      if (txt.length !== lastLen) {
        lastLen = txt.length;
        term.textContent = txt;
        term.scrollTop = term.scrollHeight;
      }
    })
    .catch(() => {});
}

setInterval(updateLog, 1000);
updateLog();
</script>
</body>
</html>
)rawliteral";

MobileAPClass MobileAP;

void MobileAPClass::begin() {
  pinMode(0, INPUT_PULLUP);
}

void MobileAPClass::startAP() {
  if (this->ap_active) return;
  Serial.println(F("\n[MobileAP] Iniciando SoftAP Standalone..."));
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP("FlipperKiller-AP", "flipperkiller");
  delay(100);

  IPAddress apIP(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  this->dnsServer.start(53, "*", apIP);

  // Endpoint: Página principal WebUI
  this->server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", MOBILE_HTML);
  });

  // Endpoints de portal cautivo para redirigir a /
  this->server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("http://192.168.4.1/");
  });
  this->server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("http://192.168.4.1/");
  });

  // Endpoint: Ejecución de comando CLI
  this->server.on("/api/cmd", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("c")) {
      String command = request->getParam("c")->value();
      Serial.print(F("[WebUI] Comando recibido: "));
      Serial.println(command);
      cli_obj.runCommand(command);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Missing cmd parameter");
    }
  });

  // Endpoint: Logs en vivo de Marauder
  this->server.on("/api/log", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", getMobileLogs());
  });

  // Fallback para cualquier otra ruta de portal cautivo
  this->server.onNotFound([](AsyncWebServerRequest *request) {
    request->redirect("http://192.168.4.1/");
  });

  this->server.begin();
  this->ap_active = true;

  Serial.println(F("[MobileAP] Red Wi-Fi lista: FlipperKiller-AP (pass: flipperkiller)"));
  Serial.println(F("[MobileAP] Accede desde el movil a: http://192.168.4.1"));
}

void MobileAPClass::stopAP() {
  if (!this->ap_active) return;
  this->dnsServer.stop();
  this->server.end();
  WiFi.softAPdisconnect(true);
  this->ap_active = false;
  Serial.println(F("[MobileAP] SoftAP detenido."));
}

void MobileAPClass::update() {
  // Manejo del botón físico BOOT (SW2 en GPIO 0)
  if (digitalRead(0) == LOW) {
    if (!this->button_pressed) {
      this->button_pressed = true;
      this->button_press_start = millis();
    } else if (millis() - this->button_press_start > 1200) {
      if (this->ap_active) {
        this->stopAP();
      } else {
        this->startAP();
      }
      this->button_pressed = false;
      while (digitalRead(0) == LOW) delay(50);
    }
  } else {
    this->button_pressed = false;
  }

  if (this->ap_active) {
    this->dnsServer.processNextRequest();
  }
}
