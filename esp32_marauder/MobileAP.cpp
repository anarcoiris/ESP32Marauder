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
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>Flipper Killer S3</title>
<style>
:root{--bg:#090d13;--panel:#0f1923;--panel2:#152030;--border:#1e3a5f;--ac:#00d4ff;--ac2:#0055ff;--danger:#ff4040;--warn:#ff9500;--ok:#00c851;--text:#d0e8ff;--mut:#6a9abf}
*{box-sizing:border-box;margin:0;padding:0}
body{background:var(--bg);color:var(--text);font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;padding-bottom:24px}
header{background:linear-gradient(135deg,#0a1628,#0f2540);border-bottom:1px solid var(--border);padding:12px 16px;display:flex;align-items:center;justify-content:space-between;position:sticky;top:0;z-index:10}
.logo{display:flex;align-items:center;gap:10px}
.logo-icon{width:34px;height:34px;background:linear-gradient(135deg,var(--ac),var(--ac2));border-radius:9px;display:flex;align-items:center;justify-content:center;font-size:20px}
h1{font-size:1rem;font-weight:700;color:#fff;line-height:1.15}
h1 small{font-size:0.63rem;color:var(--mut);font-weight:400;display:block}
.dot{width:8px;height:8px;border-radius:50%;background:var(--ok);animation:blink 2s infinite}
@keyframes blink{0%,100%{opacity:1}50%{opacity:.4}}
.btn-help{background:rgba(0,212,255,.1);border:1px solid var(--border);color:var(--ac);border-radius:20px;padding:6px 12px;font-size:.75rem;cursor:pointer}
#tut{position:fixed;inset:0;background:rgba(0,0,0,.88);z-index:100;display:flex;align-items:center;justify-content:center;padding:18px;backdrop-filter:blur(4px)}
.tc{background:linear-gradient(135deg,#0f1923,#152030);border:1px solid var(--ac);border-radius:16px;padding:22px;max-width:360px;width:100%;box-shadow:0 0 40px rgba(0,212,255,.2)}
.tdots{display:flex;gap:7px;justify-content:center;margin-bottom:18px}
.tdot{width:8px;height:8px;border-radius:50%;background:var(--border);transition:.3s}
.tdot.a{background:var(--ac)}.tdot.d{background:var(--ok)}
.tico{font-size:2.4rem;text-align:center;margin-bottom:10px}
.ttitle{font-size:1.05rem;font-weight:700;color:#fff;text-align:center;margin-bottom:8px}
.tdesc{font-size:.83rem;color:var(--mut);line-height:1.65;text-align:center;margin-bottom:14px}
.tdesc b{color:var(--text)}
.tcmd{background:rgba(0,212,255,.08);border:1px solid rgba(0,212,255,.25);border-radius:8px;padding:10px 12px;font-size:.76rem;color:var(--ac);font-family:monospace;white-space:pre;margin-bottom:16px}
.tnav{display:flex;gap:8px}
.tnav button{flex:1;padding:10px;border-radius:8px;border:none;font-size:.85rem;font-weight:700;cursor:pointer;transition:.15s}
.tskip{background:rgba(255,255,255,.06);color:var(--mut)}
.tnxt{background:linear-gradient(135deg,var(--ac),var(--ac2));color:#fff}
.tfin{background:linear-gradient(135deg,#00c851,#00864a);color:#fff}
.tabs{display:flex;border-bottom:1px solid var(--border);background:var(--panel);padding:0 10px;overflow-x:auto}
.tab{padding:11px 14px;font-size:.76rem;font-weight:600;color:var(--mut);cursor:pointer;border-bottom:2px solid transparent;transition:.2s;white-space:nowrap}
.tab.on{color:var(--ac);border-bottom-color:var(--ac)}
.panel{display:none;padding:12px}.panel.on{display:block}
.sec{font-size:.68rem;color:var(--mut);font-weight:600;text-transform:uppercase;letter-spacing:.1em;margin-bottom:8px;margin-top:2px}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:12px}
.cb{background:var(--panel2);border:1px solid var(--border);color:var(--text);padding:12px 8px;border-radius:10px;font-size:.77rem;font-weight:600;cursor:pointer;display:flex;flex-direction:column;align-items:center;gap:3px;transition:.15s;text-align:center}
.cb:active{transform:scale(.96)}
.cb .em{font-size:1.35rem}.cb .lb{font-size:.72rem}
.cb .hi{font-size:.61rem;color:var(--mut);font-weight:400;line-height:1.3;margin-top:1px}
.cb.acc{background:rgba(0,212,255,.07);border-color:rgba(0,212,255,.28);color:var(--ac)}
.cb.red{background:rgba(255,64,64,.1);border-color:rgba(255,64,64,.35);color:#ff7070;grid-column:span 2}
.cb.orn{background:rgba(255,149,0,.08);border-color:rgba(255,149,0,.3);color:#ffb060}
.cb.wide{grid-column:span 2}
.th{display:flex;align-items:center;justify-content:space-between;padding:10px 14px 4px}
.tl{font-size:.7rem;color:var(--mut);font-weight:600;text-transform:uppercase;letter-spacing:.08em;display:flex;align-items:center;gap:6px}
.tlive{width:6px;height:6px;border-radius:50%;background:var(--ok);animation:blink 1.5s infinite}
.tclr{background:none;border:1px solid var(--border);color:var(--mut);font-size:.7rem;padding:3px 8px;border-radius:5px;cursor:pointer}
#term{background:#000;color:#39ff14;font-family:'Courier New',monospace;font-size:.74rem;padding:10px;margin:6px 14px;border-radius:8px;border:1px solid #152015;height:210px;overflow-y:auto;white-space:pre-wrap;word-break:break-all;line-height:1.45}
.cli{padding:0 14px 14px;display:flex;flex-direction:column;gap:8px}
.crow{display:flex;gap:6px}
.cinput{flex:1;background:var(--panel2);border:1px solid var(--border);color:#fff;padding:10px 12px;border-radius:8px;font-size:.84rem;font-family:monospace}
.cinput:focus{outline:none;border-color:var(--ac)}
.csend{background:linear-gradient(135deg,var(--ac),var(--ac2));color:#000;border:none;padding:0 16px;border-radius:8px;font-weight:700;font-size:.84rem;cursor:pointer}
.chips{display:flex;flex-wrap:wrap;gap:5px}
.chip{background:rgba(0,212,255,.07);border:1px solid rgba(0,212,255,.18);color:var(--mut);font-size:.64rem;padding:4px 9px;border-radius:12px;cursor:pointer;font-family:monospace;transition:.15s}
.chip:active{background:rgba(0,212,255,.18);color:var(--ac)}
</style>
</head>
<body>

<header>
  <div class="logo">
    <div class="logo-icon">&#x26A1;</div>
    <h1>Flipper Killer S3<small>ESP32 Marauder &middot; 192.168.4.1</small></h1>
  </div>
  <div style="display:flex;align-items:center;gap:10px">
    <div class="dot"></div>
    <button class="btn-help" onclick="showTut()">&#x2753; Guia</button>
  </div>
</header>

<div id="tut" style="display:none">
  <div class="tc">
    <div class="tdots" id="tdots"></div>
    <div class="tico" id="tico"></div>
    <div class="ttitle" id="ttitle"></div>
    <div class="tdesc" id="tdesc"></div>
    <div class="tcmd" id="tcmd" style="display:none"></div>
    <div class="tnav">
      <button class="tskip" onclick="hideTut()">Saltar</button>
      <button id="tnxt" onclick="nextStep()">Siguiente &rarr;</button>
    </div>
  </div>
</div>

<div class="tabs">
  <div class="tab on" onclick="switchTab('wifi',this)">&#x1F4E1; WiFi</div>
  <div class="tab" onclick="switchTab('ble',this)">&#x1F4FB; BLE</div>
  <div class="tab" onclick="switchTab('cap',this)">&#x1F3A3; Captura</div>
  <div class="tab" onclick="switchTab('sys',this)">&#x2699;&#xFE0F; Sistema</div>
</div>

<div class="panel on" id="panel-wifi">
  <p class="sec">Escaneo &amp; Reconocimiento</p>
  <div class="grid">
    <button class="cb acc" onclick="cmd('scanap')">
      <span class="em">&#x1F50D;</span><span class="lb">Scan AP</span>
      <span class="hi">Descubre redes WiFi cercanas</span>
    </button>
    <button class="cb" onclick="cmd('list -a')">
      <span class="em">&#x1F4CB;</span><span class="lb">Ver APs</span>
      <span class="hi">Lista las redes detectadas</span>
    </button>
    <button class="cb" onclick="cmd('sniffbeacon')">
      <span class="em">&#x1F4E1;</span><span class="lb">Sniff Beacons</span>
      <span class="hi">Captura tramas beacon 802.11</span>
    </button>
    <button class="cb" onclick="cmd('scansta')">
      <span class="em">&#x1F4F1;</span><span class="lb">Scan Stations</span>
      <span class="hi">Detecta dispositivos conectados</span>
    </button>
    <button class="cb red" onclick="cmd('stopscan')">
      <span class="em">&#x1F6D1;</span><span class="lb">Detener Todo &mdash; stopscan</span>
    </button>
  </div>
</div>

<div class="panel" id="panel-ble">
  <p class="sec">Bluetooth Low Energy</p>
  <div class="grid">
    <button class="cb acc" onclick="cmd('sniffbt')">
      <span class="em">&#x1F535;</span><span class="lb">BLE Scan</span>
      <span class="hi">Detecta dispositivos BT cercanos</span>
    </button>
    <button class="cb" onclick="cmd('list -b')">
      <span class="em">&#x1F4CB;</span><span class="lb">Ver BLE</span>
      <span class="hi">Lista los dispositivos encontrados</span>
    </button>
    <button class="cb orn" onclick="cmd('blespam -t applejuice')">
      <span class="em">&#x1F34E;</span><span class="lb">Apple Spam</span>
      <span class="hi">Notificaciones falsas a iPhone/iPad</span>
    </button>
    <button class="cb orn" onclick="cmd('blespam -t google')">
      <span class="em">&#x1F916;</span><span class="lb">Android Spam</span>
      <span class="hi">Notificaciones falsas a Android</span>
    </button>
    <button class="cb orn" onclick="cmd('blespam -t windows')">
      <span class="em">&#x1FA9F;</span><span class="lb">Windows Spam</span>
      <span class="hi">Popups Swift Pair en Windows</span>
    </button>
    <button class="cb orn" onclick="cmd('blespam -t samsung')">
      <span class="em">&#x1F4F2;</span><span class="lb">Samsung Spam</span>
      <span class="hi">Fast Pair en Galaxy/Samsung</span>
    </button>
    <button class="cb red" onclick="cmd('stopscan')">
      <span class="em">&#x1F6D1;</span><span class="lb">Detener Todo</span>
    </button>
  </div>
</div>

<div class="panel" id="panel-cap">
  <p class="sec">Captura &amp; Analisis</p>
  <div class="grid">
    <button class="cb acc" onclick="cmd('sniffpmkid')">
      <span class="em">&#x1F510;</span><span class="lb">PMKID</span>
      <span class="hi">Captura handshakes WPA2 pasivamente</span>
    </button>
    <button class="cb" onclick="cmd('sniffpwn')">
      <span class="em">&#x1F608;</span><span class="lb">Pwnagotchi</span>
      <span class="hi">Detecta Pwnagotchis y WPA en el aire</span>
    </button>
    <button class="cb" onclick="cmd('sniffraw')">
      <span class="em">&#x1F4E6;</span><span class="lb">Raw Packets</span>
      <span class="hi">Captura todos los paquetes 802.11</span>
    </button>
    <button class="cb" onclick="cmd('sniffdeauth')">
      <span class="em">&#x1F575;&#xFE0F;</span><span class="lb">Sniff Deauth</span>
      <span class="hi">Detecta ataques de desautenticacion</span>
    </button>
    <button class="cb wide" onclick="cmd('channel -s 6')">
      <span class="em">&#x1F4F6;</span><span class="lb">Canal 6 (mas comun)</span>
    </button>
    <button class="cb red" onclick="cmd('stopscan')">
      <span class="em">&#x1F6D1;</span><span class="lb">Detener Captura</span>
    </button>
  </div>
</div>

<div class="panel" id="panel-sys">
  <p class="sec">Dispositivo</p>
  <div class="grid">
    <button class="cb" onclick="cmd('help')">
      <span class="em">&#x1F4D6;</span><span class="lb">Ayuda</span>
      <span class="hi">Lista todos los comandos disponibles</span>
    </button>
    <button class="cb" onclick="cmd('memfree')">
      <span class="em">&#x1F9E0;</span><span class="lb">Memoria</span>
      <span class="hi">Muestra RAM libre disponible</span>
    </button>
    <button class="cb" onclick="cmd('settings')">
      <span class="em">&#x2699;&#xFE0F;</span><span class="lb">Settings</span>
      <span class="hi">Ver configuracion actual</span>
    </button>
    <button class="cb" onclick="showTut()">
      <span class="em">&#x2753;</span><span class="lb">Ver Guia</span>
      <span class="hi">Vuelve a mostrar el tutorial</span>
    </button>
    <button class="cb red" onclick="if(confirm('Reiniciar el dispositivo?'))cmd('reboot')">
      <span class="em">&#x1F504;</span><span class="lb">Reiniciar ESP32</span>
    </button>
  </div>
</div>

<div class="th">
  <div class="tl"><div class="tlive"></div> Terminal en vivo</div>
  <button class="tclr" onclick="clearTerm()">x limpiar</button>
</div>
<pre id="term">Conectado a Flipper Killer S3...
Esperando actividad. Pulsa un boton o escribe un comando.
Tip: usa el boton Guia [?] para el tutorial de inicio.
</pre>

<div class="cli">
  <div class="crow">
    <input type="text" class="cinput" id="ci" placeholder="Escribe un comando... (ej: scanall, help)" onkeydown="if(event.key==='Enter')send()">
    <button class="csend" onclick="send()">&#x23CE;</button>
  </div>
  <div class="chips">
    <span class="chip" onclick="fill('scanall')">scanall</span>
    <span class="chip" onclick="fill('stopscan')">stopscan</span>
    <span class="chip" onclick="fill('sniffbt')">sniffbt</span>
    <span class="chip" onclick="fill('sniffpmkid')">sniffpmkid</span>
    <span class="chip" onclick="fill('channel -s 6')">channel -s 6</span>
    <span class="chip" onclick="fill('list -a')">list -a</span>
    <span class="chip" onclick="fill('list -b')">list -b</span>
    <span class="chip" onclick="fill('help')">help</span>
    <span class="chip" onclick="fill('memfree')">memfree</span>
  </div>
</div>

<script>
var ST=[
  {i:'&#x26A1;',t:'Bienvenido a Flipper Killer S3',
   d:'Este dispositivo ejecuta <b>ESP32 Marauder</b>, firmware de auditoria WiFi y BLE para pentesters y entusiastas de la seguridad.<br><br>Esta guia te muestra lo esencial en 5 pasos.',c:null},
  {i:'&#x1F4E1;',t:'Paso 1 - Escanear redes WiFi',
   d:'La pestana <b>WiFi</b> contiene las herramientas de reconocimiento.<br><br>Empieza siempre con <b>Scan AP</b> para descubrir redes cercanas, luego <b>Ver APs</b> para ver los resultados.',
   c:'scanall   -> inicia escaneo\nlist -a   -> ver resultados'},
  {i:'&#x1F4FB;',t:'Paso 2 - Bluetooth Low Energy',
   d:'En la pestana <b>BLE</b> puedes detectar dispositivos Bluetooth y enviar anuncios de emparejamiento falsos a moviles y PCs cercanos.<br><br><b>Aviso:</b> Usalo solo donde tengas permiso.',
   c:'sniffbt              -> detectar\nblespam -t applejuice -> spam iPhone'},
  {i:'&#x1F510;',t:'Paso 3 - Captura de handshakes',
   d:'La pestana <b>Captura</b> incluye <b>PMKID</b>, que captura info de autenticacion WPA2 sin desconectar a nadie.<br><br>Para guardar los datos necesitas una tarjeta SD.',
   c:'sniffpmkid -> captura PMKID\nsniffraw   -> paquetes crudos'},
  {i:'&#x2328;&#xFE0F;',t:'Paso 4 - Linea de comandos',
   d:'El campo inferior ejecuta <b>cualquier comando</b> de Marauder. Los chips azules son atajos rapidos.<br><br>Escribe <b>help</b> para ver la lista completa de comandos.',
   c:'help          -> todos los comandos\nchannel -s 6  -> cambiar canal'},
  {i:'&#x1F6D1;',t:'Paso 5 - Detener operaciones',
   d:'El boton rojo <b>Detener Todo (stopscan)</b> para inmediatamente cualquier escaneo, ataque o captura activa.<br><br>Usalo siempre antes de cambiar de modo o reiniciar.',
   c:'stopscan -> detiene todo\nreboot   -> reinicia ESP32'},
];
var step=0;
function buildDots(){
  var d=document.getElementById('tdots');d.innerHTML='';
  for(var i=0;i<ST.length;i++){
    var s=document.createElement('div');
    s.className='tdot'+(i===step?' a':i<step?' d':'');
    d.appendChild(s);
  }
}
function renderStep(){
  var s=ST[step];
  document.getElementById('tico').innerHTML=s.i;
  document.getElementById('ttitle').textContent=s.t;
  document.getElementById('tdesc').innerHTML=s.d;
  var ce=document.getElementById('tcmd');
  if(s.c){ce.style.display='block';ce.textContent=s.c;}else{ce.style.display='none';}
  var nb=document.getElementById('tnxt');
  if(step===ST.length-1){nb.className='tfin';nb.textContent='Empezar!';}
  else{nb.className='tnxt';nb.innerHTML='Siguiente &rarr;';}
  buildDots();
}
function showTut(){step=0;document.getElementById('tut').style.display='flex';renderStep();}
function hideTut(){document.getElementById('tut').style.display='none';}
function nextStep(){if(step<ST.length-1){step++;renderStep();}else{hideTut();}}
if(!localStorage.getItem('fk_t')){localStorage.setItem('fk_t','1');showTut();}

function switchTab(id,el){
  document.querySelectorAll('.tab').forEach(function(t){t.classList.remove('on');});
  document.querySelectorAll('.panel').forEach(function(p){p.classList.remove('on');});
  el.classList.add('on');
  document.getElementById('panel-'+id).classList.add('on');
}

function cmd(v){fetch('/api/cmd?c='+encodeURIComponent(v)).catch(function(){});}
function send(){var el=document.getElementById('ci'),v=el.value.trim();if(v){cmd(v);el.value='';}}
function fill(v){document.getElementById('ci').value=v;document.getElementById('ci').focus();}

var term=document.getElementById('term'),lastLen=0;
function updateLog(){
  fetch('/api/log').then(function(r){return r.text();}).then(function(txt){
    if(txt.length!==lastLen){lastLen=txt.length;term.textContent=txt;term.scrollTop=term.scrollHeight;}
  }).catch(function(){});
}
function clearTerm(){term.textContent='[Terminal limpiado]\n';lastLen=0;}
setInterval(updateLog,1000);updateLog();
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
