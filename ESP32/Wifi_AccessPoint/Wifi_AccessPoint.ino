#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

WiFiServer server(80);

const char* ssid     = "Robo-T";
const char* password = "dmt@123abc";



void setup()
{
    Serial.begin(115200);
    delay(10);
    ////////////
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){ delay(500); Serial.print(".");}
    Serial.println("");
    Serial.println(WiFi.localIP());
    ////////////
    WiFi.softAP("Radio", "dmt@1234");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);
    server.begin();
}
///////////////////////////
void loop()
{
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            String sHtml = "<html><head><meta name='viewport' content='width=device-width'></head><body><script src='https://cdn.jsdelivr.net/npm/hls.js@latest'></script><video id='video'></video><script>if(Hls.isSupported()) { var video = document.getElementById('video'); var hls = new Hls(); hls.loadSource('https://5a6872aace0ce.streamlock.net/nghevov1/vov1.stream_aac/playlist.m3u8'); hls.attachMedia(video); hls.on(Hls.Events.MANIFEST_PARSED,function() { video.play(); }); }</script></body></html>";
            // the content of the HTTP response follows the header:
            client.print(sHtml);
            client.println();
 
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) { }
          
        if (currentLine.endsWith("GET /L")) {
          
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }

}
