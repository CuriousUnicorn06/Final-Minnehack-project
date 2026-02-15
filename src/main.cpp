#include <WiFi.h>
#include <WebServer.h>

// ---- CHANGE THESE ----
const char* ssid = "alaynas iPhone";
const char* password = "abcdefghij";

// ---- PINS ----
const int LED_PIN = 2;      // Built‑in LED on many ESP32 boards
const int BUTTON_PIN = 14;  // Change to whatever pin your button uses

WebServer server(80);

// ---- WEBPAGE ----
// ---- WEBPAGE ----
String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>To-Do Competition</title>
</head>
<body>

  <h2>Pick a User</h2>

  <form action="/submit" method="GET">
    
    <label>
      <input type="radio" name="user" value="User1" required>
      User 1
    </label><br>

    <label>
      <input type="radio" name="user" value="User2">
      User 2
    </label><br><br>

    <h3>Enter To-Do List:</h3>
    <textarea name="todo" rows="6" cols="30"
      placeholder="Ex: 
- Homework
- Gym
- Laundry"></textarea><br><br>

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
  Serial.println("To-do:");
  Serial.println(todo);

  String response = "<h2>Saved for " + user + "!</h2>";
  response += "<pre>" + todo + "</pre>";
  response += "<a href='/'>Go back</a>";

  server.send(200, "text/html", response);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("this is new and works");
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
  // ---- LED ----
  pinMode(LED_PIN, OUTPUT);

  // ---- BUTTON ----
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // ---- WIFI ----
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // ---- WEB SERVER ----
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();

  // ---- LED BLINK TEST ----
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);

  // ---- BUTTON TEST ----
  if (digitalRead(BUTTON_PIN) == LOW) {  // Button pressed
    Serial.println("Button pressed!");
    delay(300); // Debounce

  }
}
