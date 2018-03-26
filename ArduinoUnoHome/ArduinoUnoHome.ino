

// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)

/////////////////////////////////////////////
unsigned long previousMilisTest = 0;        // will store last time LED was updated
const int pinR=11;
const int pinB=10;
const int pinG=9;

int myPins[] = {pinR, pinG, pinB};
int currentPinIndex = 0;

/////////////////////////////////////////////
unsigned long animatedMilisTest = 0;        // will store last time LED was updated
int animatedCurrentValue = 0;


/////////////////////////////////////////////
const int pinSensorPIR = 12;
const int pinSensorLED = 6;
int valPIR = 0;         // variable to store the read value

/////////////////////////////////////////////
unsigned long aproxMilisTest = 0;
const int trigPin = 3;
const int echoPin = 2;
// defines variables
long duration;
int distance;
const int rangeWidth = 4;
const int changeTreshold = 4;
int currentBaseDistance = 0;
int changeCandidateDistance = 0;
int changeCounter = 0;


void setup() {
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  
  pinMode(pinR,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinG,OUTPUT);

  // ALWAYS CHECK IF PIN MODE WAS SET FOR PINs
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.print("test=");
  Serial.println(sizeof(myPins));
}

void loop() {
//  blinkBuiltInLED();
  
  testMyLED();
//  animatedColoringLED();

//  handlePIR();

  testAprox();
}

bool inRange(int rangeBase, int candidate) {
  int rangeLeftBound = rangeBase - rangeWidth / 2;
  int rangeRightBound = rangeBase + rangeWidth / 2;
  return candidate >= rangeLeftBound || candidate <= rangeRightBound;
}

void testAprox() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;

  bool isValid = false;
  if (inRange(distance, currentBaseDistance)) {
    // in range
    isValid = true;
    changeCounter = 0;
  } else {
    // not in range
    if (inRange(distance, changeCandidateDistance)) {
      changeCounter = changeCounter + 1;
    } else {
      changeCounter = 0;
    }
    
    changeCandidateDistance = distance;
    if (changeCounter >= changeTreshold) {
      currentBaseDistance = distance;
      isValid = true;
      changeCounter = 0;
    }
  }
  
  if (isValid) {
    // 1 meter is a max distance considered here
    int maxDistance = 100;
    int minDistance = 10;
    int tempDistance = distance;
    if (tempDistance > maxDistance)
    {
      tempDistance = maxDistance;
    }
    else if (tempDistance < minDistance) {
      tempDistance = 0;
    }
    float pct = ((float)tempDistance) / maxDistance;
    int minOutput = 0;
    int maxOutput = 255;
    float output =  pct * (maxOutput - minOutput) + minOutput ;
    int finalOutput = (int)output;
    analogWrite(pinSensorLED, finalOutput);
    
  
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(", pct: ");
    Serial.print(pct);
    Serial.print(", Output: ");
    Serial.println(finalOutput);
  }
}

void handlePIR() {
  valPIR= digitalRead(pinSensorPIR);
  Serial.print("sensor=");
  Serial.println(valPIR);
  int state = LOW;
  if (valPIR != 0) {
    state = HIGH;
  }
  digitalWrite(pinSensorLED, state);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}

void animatedColoringLED() {
    unsigned long currentMillis = millis();
    if (currentMillis - animatedMilisTest >= 5) {
      animatedMilisTest = currentMillis;
  
      analogWrite(pinG, animatedCurrentValue);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      animatedCurrentValue = (animatedCurrentValue + 1) % 256;
    }
}

void testMyLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMilisTest >= interval) {
    previousMilisTest = currentMillis;

    for(int i = 0; i < sizeof(myPins) / 2; i++) {
      int state = LOW;
      if (i == currentPinIndex) {
        state = HIGH;
      }

      digitalWrite(myPins[i], state);
    }
    
    currentPinIndex = (currentPinIndex + 1) % (sizeof(myPins) / 2);
  }
}

void blinkBuiltInLED()
{
    // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

