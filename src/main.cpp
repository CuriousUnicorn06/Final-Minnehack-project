#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "alaynas iPhone";
const char* password = "abcdefghij";

WebServer server(80);

// ---- WEBPAGE ----
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>To-Do Competition</title>
</head>
<body>
  <h2>Pick a User</h2>

  <form action="/submit" method="GET">
    <label><input type="radio" name="user" value="User1" required> User 1</label><br>
    <label><input type="radio" name="user" value="User2"> User 2</label><br><br>

    <h3>Enter To-Do List:</h3>
    <textarea name="todo" rows="8" cols="32" placeholder="- Homework&#10;- Gym&#10;- Laundry"></textarea><br><br>

    <input type="submit" value="Save List">
  </form>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSubmit() {
  String user = server.arg("user");
  String todo = server.arg("todo");

  Serial.println("----- NEW LIST -----");
  Serial.println("User: " + user);
  Serial.println("To-do:\n" + todo);

  String response = "<h2>Saved for " + user + "!</h2>";
  response += "<pre>" + todo + "</pre>";
  response += "<a href='/'>Go back</a>";

  server.send(200, "text/html", response);
}

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
    if (millis() - start > 20000) {   // 20s timeout so it doesn't hang forever
      Serial.println("\nWiFi failed. Rebooting...");
      ESP.restart();
    }
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
  delay(2); // ✅ tiny yield to keep system happy
}