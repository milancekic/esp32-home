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

bool isConnectedToWiFi = false;

AsyncWebServer server(80);
////////////////////////////////////////////////

void setup() {
    Serial.begin(9600);
    pinMode(BIG_LED_PIN_RED, OUTPUT);
    pinMode(BIG_LED_PIN_BLUE, OUTPUT);
    pinMode(BIG_LED_PIN_GREEN, OUTPUT);

    isConnectedToWiFi = connectToNetwork();
    if (isConnectedToWiFi) {
      setupRoutes();
      // start server at port 80
      server.begin();
    }
}

void loop(){
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

void setupRoutes() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getHomeHTML());
  });
  server.on("/OFF", HTTP_POST, [](AsyncWebServerRequest *request){
    resetBigLED();
    ledState = off;
    request->send(200, "text/html", getHomeHTML());
  });
  server.on("/RED", HTTP_POST, [](AsyncWebServerRequest *request){
    resetBigLED();
    digitalWrite(BIG_LED_PIN_RED, HIGH);
    ledState = red;
    request->send(200, "text/html", getHomeHTML());
  });
  server.on("/BLUE", HTTP_POST, [](AsyncWebServerRequest *request){
    resetBigLED();
    digitalWrite(BIG_LED_PIN_BLUE, HIGH);
    ledState = blue;
    request->send(200, "text/html", getHomeHTML());
  });
  server.on("/GREEN", HTTP_POST, [](AsyncWebServerRequest *request){
    resetBigLED();
    digitalWrite(BIG_LED_PIN_GREEN, HIGH);
    ledState = green;
    request->send(200, "text/html", getHomeHTML());
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    String html = "<html><head><style> form { display: inline; }</style></head><body><h2>This page does not exist.</h2></body>";
    request->send(404, "text/html", html);
  });
}

String getHomeHTML() {
  String html = "<html><head><style> form { display: inline; }</style> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body>";
  html = html + "<h2>Set LED state:</h2>";
  html = html + "<p>";
  html = html + "<form action = \"/OFF\" method = \"post\"><input type=\"submit\" name=\"offButton\" value=\"Turn Off\"/></form>";
  html = html + "<form action = \"/RED\" method = \"post\"><input type=\"submit\" name=\"redButton\" value=\"Red\"/></form>";
  html = html + "<form action = \"/BLUE\" method = \"post\"><input type=\"submit\" name=\"blueButton\" value=\"Blue\"/></form>";
  html = html + "<form action = \"/GREEN\" method = \"post\"><input type=\"submit\" name=\"greenButton\" value=\"Green\"/></form>";

  html = html + "</p>";
  html = html + "<p>Footer</p></body></html>";
  return html;
}

void resetBigLED() {
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(pinsBigLED[i], LOW);
  }
}

