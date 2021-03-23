
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);


// Access point credentials
const char *ap_ssid     = "Minh Tri";
const char *ap_password = "12345678";


String Header    = "<html><head><title>IoT  Viet Nam</title></head> <body style='background-color:#c0c0c0' > ";
String EndHeader = "</body></html>";


// ESP8266 GPIO pins
#define GPIO2  2
#define GPIO0  0
#define GPIO5  5
#define GPIO4  4
#define GPIO15 15
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////   Web function   ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// document.getElementById('CbbTuoiNhanh').value = 1; 
// document.getElementById('NutTuoi').innerHTML
String JavaScriptUser()
{
    String JVScript = "<script>\
                     var nTimerdown = 0;\
                     function Desktop(){ SendURL('/desktop'); ReloadWebTimer(2000); }\
                     function SendURL(URL){  var xhttp = new XMLHttpRequest(); xhttp.open('GET', URL, true); xhttp.send();}\
                     function OpenLink(URL){ window.location.href = URL ; }\
                     function ReloadWeb(){ SendURL('/refesh'); window.location.reload(); }\
                     function ReloadWebTimer(_timer){ setTimeout('ReloadWeb()',_timer); }\
                     function HoatDong(_value){ SendURL('/hoatdong/?T:' + _value + '$' ); }\
                     function TuoiNhanh(_value){ var url = '/tuoinhanh/?T:' + _value + '$'; SendURL(url); nTimerdown = -1;  setTimeout('KiemTraTuoiNhanh(' + _value + ')',1000);  }\
                     function KiemTraTuoiNhanh(nTime){ nTimerdown = nTime*60; ThoiGianLui(); }\
                     function ThoiGianLui(){ if(nTimerdown>=0){ var phut = parseInt(nTimerdown/60); var giay = nTimerdown%60; document.getElementById('TimerDown').innerHTML = phut + ':' + giay ; nTimerdown--; var sVal = 'ThoiGianLui('+nTimerdown+')'; setTimeout(sVal,1000); }} \
                     </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String Javascript()
{
  String JVScript = "<script>\
                     function AnNutSave()  { document.getElementById('btSave').style.visibility = 'hidden'; }\ 
                     function HienNutSave(){ document.getElementById('btSave').style.visibility = 'visible'; }\ 
                     function LuuSetting(){\
                        var wname   = document.getElementById('wifiname').value;\
                        var wpass   = document.getElementById('wifipass').value;\
                        var wip     = document.getElementById('wifiip').value;\
                        var wgetway = document.getElementById('wifigetway').value;\
                        var admname = document.getElementById('admname').value;\
                        var admpass = document.getElementById('admpass').value;\
                        var admip   = document.getElementById('admip').value;\
                        var admport = document.getElementById('admport').value;\
                        if(wname !='' && wpass != '' && wip !='' && wgetway != '' && admname !='' && admpass != '' && admip !='' && admport != ''){\
                          var URL = '/dmtLuuCaiDat/?ABCX' + wname + '\,' + wpass + '\,' + wip + '\,' + wgetway + '\,' + admname + '\,' + admpass + '\,' + admip + '\,' + admport + '\,$';\
                          SendURL(URL);  AnNutSave(); setTimeout('HienNutSave()',10000);\
                          document.getElementById('dmtkk').value = URL ;\
                        }else{alert('Error input !'); return ;}\
                     }</script>";
  return JVScript;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KiemTraTrangThaiDen(String _Name, uint8_t _Pin )
{
  if (server.hasArg(_Name))
  { 
    if (server.arg(_Name) == "1"){  digitalWrite(_Pin, HIGH); }
    else{ digitalWrite(_Pin, LOW);  }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CaiDat_GPIO()
{
    KiemTraTrangThaiDen("gpio2",  GPIO2  );
    KiemTraTrangThaiDen("gpio0",  GPIO0  );
    KiemTraTrangThaiDen("gpio5",  GPIO5  );
    KiemTraTrangThaiDen("gpio4",  GPIO4  );
    KiemTraTrangThaiDen("gpio15", GPIO15 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String GiaoDienNutBam()
{
  String HTML +=" <script>\
                      function SendURL(URL){  var xhttp = new XMLHttpRequest(); xhttp.open('GET', URL, true); xhttp.send();}\
                  </script>\
                      <div align=center>\
                          <table border='0' cellspacing='5'>\
                              <tr>\
                                  <td width='83'>Light 1</td>\
                                  <td width='62'><input type='button' value='On'  name='B3' onclick=\"SendURL('/?gpio2=1')\"  style='float: right'></td>\
                                  <td width='56'><input type='button' value='Off' name='B4' onclick=\"SendURL('/?gpio2=0')\">                      </td>\
                              </tr>\
                              <tr>\
                                  <td width='83'>Light 2</td>\
                                  <td width='62'><input type='button' value='On'  name='B3' onclick=\"SendURL('/?gpio0=1')\"  style='float: right'></td>\
                                  <td width='56'><input type='button' value='Off' name='B4' onclick=\"SendURL('/?gpio0=0')\">                      </td>\
                              </tr>\
                              <tr>\
                                  <td width='83'>Light 3</td>\
                                  <td width='62'><input type='button' value='On'  name='B3' onclick=\"SendURL('/?gpio5=1')\"  style='float: right'></td>\
                                  <td width='56'><input type='button' value='Off' name='B4' onclick=\"SendURL('/?gpio5=0')\">                      </td>\
                              </tr>\
                              <tr>\
                                  <td width='83'>Light 4</td>\
                                  <td width='62'><input type='button' value='On'  name='B3' onclick=\"SendURL('/?gpio4=1')\"  style='float: right'></td>\
                                  <td width='56'><input type='button' value='Off' name='B4' onclick=\"SendURL('/?gpio4=0')\">                      </td>\
                              </tr>\
                          </table>\
                      </div>";

  return HTML;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Desktop() 
{
  
  CaiDat_GPIO();
  String HTML  = Header;
         HTML += GiaoDienNutBam();

              
  
  HTML += "<center><a href='/Admin'> WLAN settings</a></center>";
  HTML += EndHeader;
  server.send(200, "text/html", HTML);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrangAdmin()
{
  
 String HTML = Header + "<h1><center>WLAN Settings</center></h1>";

  int ap_count = WiFi.scanNetworks();
  if (ap_count == 0)
  {
       HTML += "No access points found.<br>";
  }
  else
  {
    HTML += "<form method=\"get\">";
    for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
    {
      HTML += "<input type='radio'     name='ssid'      value='" + String(WiFi.SSID(ap_idx)) + "'>";
      HTML += String(WiFi.SSID(ap_idx)) + " (RSSI: " + WiFi.RSSI(ap_idx) +")";
      (WiFi.encryptionType(ap_idx) == ENC_TYPE_NONE) ? HTML += "No password" : HTML += " - Password";
      HTML += "<p>";
    }
    HTML += "WiFi password (if required):<br>";
    HTML += "<input type='text' name='password'><br>";
    HTML += "<input type='submit' value='Connect'>";
    HTML += "</form>";
  }
  HTML += EndHeader;
  server.send(200, "text/html", HTML);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(GPIO2,  OUTPUT);
  pinMode(GPIO0,  OUTPUT);
  pinMode(GPIO5,  OUTPUT);
  pinMode(GPIO4,  OUTPUT);
  pinMode(GPIO15, OUTPUT);
  /////////////////////////////
  Serial.begin(115200);
  Serial.println("Configuring access point...");
  /* Cài đặt chế độ password hoặc không password */
  WiFi.softAP(ap_ssid, ap_password);  //   WiFi.softAP(ap_ssid); 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  /* Các trang mặc định */
  server.on("/",      Desktop);
  server.on("/Admin", TrangAdmin);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
}
