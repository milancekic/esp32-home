#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define BIG_LED_PIN_RED 27
#define BIG_LED_PIN_BLUE 14
#define BIG_LED_PIN_GREEN 12
int pinsBigLED[] = {BIG_LED_PIN_RED, BIG_LED_PIN_BLUE, BIG_LED_PIN_GREEN};
// 0 - turned off, 1 - red, 2 - blue, 3 - green
enum BigLedState {off, red, blue, green};
BigLedState ledState = off;

////////////////////////////////////////////////
#define MAX_CONNECTION_ATTEMPTS 30

const char* ssid     = "1a6808";
const char* password = "278929194";

AsyncWebServer server(80);

bool isConnectedToWiFi = false;
////////////////////////////////////////////////

void setup() {
    Serial.begin(9600);
    pinMode(BIG_LED_PIN_RED, OUTPUT);
    pinMode(BIG_LED_PIN_BLUE, OUTPUT);
    pinMode(BIG_LED_PIN_GREEN, OUTPUT);

    isConnectedToWiFi = connectToNetwork();
    if (isConnectedToWiFi) {
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send(200, "text/html", getHomeHTML());
      });
      server.on("/OFF", HTTP_GET, [](AsyncWebServerRequest *request){
          resetBigLED();
          ledState = off;
          request->send(200, "text/html", getHomeHTML());
      });
      server.on("/RED", HTTP_GET, [](AsyncWebServerRequest *request){
          resetBigLED();
          digitalWrite(BIG_LED_PIN_RED, HIGH);
          ledState = red;
          request->send(200, "text/html", getHomeHTML());
      });
      server.on("/BLUE", HTTP_GET, [](AsyncWebServerRequest *request){
          resetBigLED();
          digitalWrite(BIG_LED_PIN_BLUE, HIGH);
          ledState = blue;
          request->send(200, "text/html", getHomeHTML());
      });
      server.on("/GREEN", HTTP_GET, [](AsyncWebServerRequest *request){
          resetBigLED();
          digitalWrite(BIG_LED_PIN_GREEN, HIGH);
          ledState = green;
          request->send(200, "text/html", getHomeHTML());
      });
      
      // start server at port 80
      server.begin();
    }
}

bool connectToNetwork() {
//    delay(10);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int currentAttempt = 1;
    while (WiFi.status() != WL_CONNECTED && currentAttempt++ <= MAX_CONNECTION_ATTEMPTS) {
        delay(500);
        Serial.print(".");
    }

    bool isConnected = WiFi.status() == WL_CONNECTED;
    if (isConnected) {
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.println("MAC address: ");
      Serial.println(WiFi.macAddress());
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("");
      Serial.println("Failed to connect to WiFi.");
    }

    return isConnected;
}

void loop(){
}

String getHomeHTML() {
  String html = "";
  html = html + "<form action=\"\">";
  html = html + "<a href=\"/OFF\"><input type=\"radio\" name=\"led\" value=\"off\"";
  if (ledState == off) {
    html = html + "checked=\"checked\"";
  }
  html = html + "> Turned off<br></a>";

  html = html + "<a href=\"/RED\"><input type=\"radio\" name=\"led\" value=\"red\"";
  if (ledState == red) {
    html = html + "checked=\"checked\"";
  }
  html = html + "> Red<br></a>";

  html = html + "<a href=\"/BLUE\"><input type=\"radio\" name=\"led\" value=\"blue\"";
  if (ledState == blue) {
    html = html + "checked=\"checked\"";
  }
  html = html + "> Blue<br></a>";
  
  html = html + "<a href=\"/GREEN\"><input type=\"radio\" name=\"led\" value=\"green\"";
  if (ledState == green) {
    html = html + "checked=\"checked\"";
  }
  html = html + "> Green<br></a>";

  html = html + "</form>";
  return html;
}

void resetBigLED() {
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(pinsBigLED[i], LOW);
  }
}

