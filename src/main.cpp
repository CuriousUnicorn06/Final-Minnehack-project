#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "alaynas iPhone";
const char* password = "abcdefghij";

WebServer server(80);

// ---- WEBPAGE ----
String htmlPage = R"rawliteral(
 <!DOCTYPE html>
  <html> <head> 
  <title>ESP32 Test</title> 
  </head>
   <body>
   <h1>ESP32 is working!</h1> 
   <p>If you can see this page, the web server works.</p>
    </body>
     </html> 
     )rawliteral";
      void handleRoot() { server.send(200, "text/html", htmlPage); }


void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\nBooting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);              // ✅ prevents watchdog resets
    Serial.print(".");
    if (millis() - start > 30000) {
  Serial.println("\nWiFi failed.");
  Serial.print("WiFi.status() = "); Serial.println(WiFi.status());
  Serial.print("RSSI = "); Serial.println(WiFi.RSSI());
  break;  // stop reboot loop so you can read info
}
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
  delay(2); // ✅ tiny yield to keep system happy
}
