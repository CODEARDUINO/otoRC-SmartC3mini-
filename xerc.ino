#include <WiFi.h>
#include <WebServer.h>
#define AIN1 5
#define AIN2 6
#define BIN1 2
#define BIN2 3
WebServer server(80);

void setup() {
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  WiFi.softAP("XE_RC_OVV", "12345678");
  server.on("/", []() {
    server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1,user-scalable=no'>
<style>
*{box-sizing:border-box} html,body{margin:0;padding:0;background:#0f0f0f;color:#fff;font-family:sans-serif;overflow:hidden;width:100%;height:100%;user-select:none;touch-action:none}
#wrapper{width:100vw;height:100vh;display:flex;flex-direction:column;justify-content:space-between}
h3{margin:12px 0;text-align:center;font-size:18px;letter-spacing:1px}
.main{flex:1;display:flex;justify-content:space-between;align-items:center;padding:0 6vw}
.col{display:flex;flex-direction:column;gap:30px}
.row{display:flex;flex-direction:row;gap:30px;align-items:center}
.btn{width:130px;height:130px;border-radius:30px;border:0;background:#222;color:#fff;font-size:45px;touch-action:none}
.btn.pressed{background:#0099ff;transform:scale(0.95)}
.footer{text-align:center;padding-bottom:14px;font-size:14px;color:#888}
@media screen and (orientation:portrait){#wrapper{transform:rotate(90deg);transform-origin:left top;width:100vh;height:100vw;position:absolute;top:0;left:100vw}}
</style></head><body>
<div id="wrapper">
<h3>Car RC - Made by Oc Vui Ve</h3>
<div class="main">
  <div class="col">
    <button class="btn" data-down="F" data-up="SF">&#9650;</button>
    <button class="btn" data-down="B" data-up="SF">&#9660;</button>
  </div>
  <div class="row">
    <button class="btn" data-down="L" data-up="SS">&#9664;</button>
    <button class="btn" data-down="R" data-up="SS">&#9654;</button>
  </div>
</div>
<div class="footer">MAX SPEED - 2 Motor 130</div>
</div>
<script>
function send(d){fetch('/c?d='+d)}
document.querySelectorAll('.btn').forEach(b=>{
  b.addEventListener('pointerdown', e=>{
    e.preventDefault(); b.setPointerCapture(e.pointerId);
    b.classList.add('pressed'); send(b.dataset.down);
  });
  b.addEventListener('pointerup', e=>{b.classList.remove('pressed'); send(b.dataset.up);});
  b.addEventListener('pointercancel', e=>{b.classList.remove('pressed'); send(b.dataset.up);});
});
</script>
</body></html>)rawliteral");
  });
  server.on("/c", []() {
    String d=server.arg("d");
    if(d=="F"){analogWrite(AIN1,255);analogWrite(AIN2,0);}
    else if(d=="B"){analogWrite(AIN1,0);analogWrite(AIN2,255);}
    else if(d=="SF"){analogWrite(AIN1,0);analogWrite(AIN2,0);}
    else if(d=="L"){analogWrite(BIN1,255);analogWrite(BIN2,0);}
    else if(d=="R"){analogWrite(BIN1,0);analogWrite(BIN2,255);}
    else if(d=="SS"){analogWrite(BIN1,0);analogWrite(BIN2,0);}
    server.send(200,"text/plain","ok");
  });
  server.begin();
}
void loop(){server.handleClient();}