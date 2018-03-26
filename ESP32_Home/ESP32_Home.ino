#include <WiFi.h>

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

WiFiServer server(80);
////////////////////////////////////////////////

void setup() {
    Serial.begin(9600);
    pinMode(BIG_LED_PIN_RED, OUTPUT);
    pinMode(BIG_LED_PIN_BLUE, OUTPUT);
    pinMode(BIG_LED_PIN_GREEN, OUTPUT);

    bool isConnected = connectToNetwork();
    if (isConnected) {
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
  if (server) {
    serveHTTPRequest();
  }
}

void serveHTTPRequest() {
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

            // the content of the HTTP response follows the header:
//            client.print("Click <a href=\"/RED\">here</a> to turn the <span style=\"color:red\">red</span> LED.<br>");
//            client.print("Click <a href=\"/BLUE\">here</a> to turn the <span style=\"color:blue\">blue</span> LED.<br>");
//            client.print("Click <a href=\"/GREEN\">here</a> to turn the <span style=\"color:green\">green</span> LED.<br>");

            client.print("<form action=\"\">");
            client.print("<a href=\"/OFF\"><input type=\"radio\" name=\"led\" value=\"off\"");
            if (ledState == off)
            {
              client.print("checked=\"checked\"");
            }
            client.print("> Turned off<br></a>");
            client.print("<a href=\"/RED\"><input type=\"radio\" name=\"led\" value=\"red\"");
            if (ledState == red)
            {
              client.print("checked=\"checked\"");
            }
            client.print("> Red<br></a>");
            client.print("<a href=\"/BLUE\"><input type=\"radio\" name=\"led\" value=\"blue\"");
            if (ledState == blue)
            {
              client.print("checked=\"checked\"");
            }
            client.print("> Blue<br></a>");
            client.print("<a href=\"/GREEN\"><input type=\"radio\" name=\"led\" value=\"green\"");
            if (ledState == green)
            {
              client.print("checked=\"checked\"");
            }
            client.print("> Green<br></a>");
            client.print("</form>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /OFF")) {
          resetBigLED();
          ledState = off;
        }
        if (currentLine.endsWith("GET /RED")) {
          resetBigLED();
          digitalWrite(BIG_LED_PIN_RED, HIGH);
          ledState = red;
        }
        if (currentLine.endsWith("GET /BLUE")) {
          resetBigLED();
          digitalWrite(BIG_LED_PIN_BLUE, HIGH);
          ledState = blue;
        }
        if (currentLine.endsWith("GET /GREEN")) {
          resetBigLED();
          digitalWrite(BIG_LED_PIN_GREEN, HIGH);
          ledState = green;
        }
      }
    }
    
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void resetBigLED() {
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(pinsBigLED[i], LOW);
  }
}

