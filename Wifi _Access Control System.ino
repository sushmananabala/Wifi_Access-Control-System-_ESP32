#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// Pins
#define ACCESS_BUTTON 19
#define DOOR_SENSOR   21
#define EXIT_BUTTON   22
#define TAMPER_BUTTON 23

#define GREEN_LED 25
#define RED_LED   26
#define BUZZER    27

// Login
const char* correctUser = "admin";
const char* correctPass = "1234";

String lastEvent = "System Started";

// ---------- WEBPAGE ----------

void handleRoot() {

  String doorStatus;

  if (digitalRead(DOOR_SENSOR) == LOW) {
    doorStatus = "CLOSED 🔒";
  } else {
    doorStatus = "OPEN 🚪";
  }

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<title>Wi-Fi Access Control</title>

<meta name="viewport"
content="width=device-width, initial-scale=1">

<style>

body {
  font-family: Arial;
  text-align: center;
  background: #eeeeee;
}

.box {
  background: white;
  max-width: 400px;
  margin: 30px auto;
  padding: 25px;
  border-radius: 15px;
}

input {
  width: 85%;
  padding: 12px;
  margin: 6px;
}

button {
  padding: 12px 25px;
}

.status {
  font-size: 20px;
}

</style>

</head>

<body>

<div class="box">

<h1>🔐 Wi-Fi Access Control</h1>

<hr>

<h2>System Status</h2>

<p class="status">
Door: <b>)rawliteral";

  page += doorStatus;

  page += R"rawliteral(</b>
</p>

<p>
Last Event:
<br>
<b>)rawliteral";

  page += lastEvent;

  page += R"rawliteral(</b>
</p>

<hr>

<h2>Login</h2>

<form action="/login" method="POST">

<input
type="text"
name="username"
placeholder="Username"
required>

<br>

<input
type="password"
name="password"
placeholder="Password"
required>

<br>

<button type="submit">
LOGIN
</button>

</form>

</div>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}


// ---------- LOGIN ----------

void handleLogin() {

  String username = server.arg("username");
  String password = server.arg("password");

  if (username == correctUser &&
      password == correctPass) {

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    lastEvent = "ACCESS GRANTED";

    Serial.println("ACCESS GRANTED");

    server.send(200, "text/html",
      "<html><body style='text-align:center'>"
      "<h1>✅ ACCESS GRANTED</h1>"
      "<h2>Door is UNLOCKED</h2>"
      "<br>"
      "<a href='/'>Dashboard</a>"
      "</body></html>");

  } else {

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);

    lastEvent = "ACCESS DENIED";

    Serial.println("ACCESS DENIED");

    server.send(200, "text/html",
      "<html><body style='text-align:center'>"
      "<h1>❌ ACCESS DENIED</h1>"
      "<h3>Invalid Username or Password</h3>"
      "<br>"
      "<a href='/'>Try Again</a>"
      "</body></html>");
  }
}


// ---------- SETUP ----------

void setup() {

  Serial.begin(115200);

  pinMode(ACCESS_BUTTON, INPUT_PULLUP);
  pinMode(DOOR_SENSOR, INPUT_PULLUP);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);
  pinMode(TAMPER_BUTTON, INPUT_PULLUP);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, LOW);

  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/login",
            HTTP_POST,
            handleLogin);

  server.begin();

  Serial.println("Web Server Started");
  Serial.println("System Ready");
}


// ---------- LOOP ----------

void loop() {

  server.handleClient();

  // Door sensor
  static int lastDoorState = HIGH;
  int currentDoorState = digitalRead(DOOR_SENSOR);

  if (currentDoorState != lastDoorState) {

    if (currentDoorState == LOW) {
      Serial.println("DOOR CLOSED");
      lastEvent = "Door Closed 🔒";
    }
    else {
      Serial.println("DOOR OPEN");
      lastEvent = "Door Open 🚪";
    }

    lastDoorState = currentDoorState;
  }


  // Access button
  if (digitalRead(ACCESS_BUTTON) == LOW) {

    Serial.println("ACCESS REQUEST");
    lastEvent = "Access Button Pressed";

    delay(300);
  }


  // Exit button
  if (digitalRead(EXIT_BUTTON) == LOW) {

    Serial.println("EXIT REQUEST");
    lastEvent = "Exit Request";

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    delay(3000);

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    delay(300);
  }


  // Tamper button
  if (digitalRead(TAMPER_BUTTON) == LOW) {

    Serial.println("!!! TAMPER DETECTED !!!");

    lastEvent = "TAMPER DETECTED 🚨";

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    digitalWrite(BUZZER, HIGH);

    delay(1000);

    digitalWrite(BUZZER, LOW);

    delay(300);
  }

  delay(50);
}
