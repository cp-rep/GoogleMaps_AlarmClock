#include <Wire.h>

// compile time consants
#define OUT_ANSWERSIZE 5
#define ARD_ADDRESS 0x09

// runtime constants
// define global grove button data
const int alarmButtonPin = 2; // D2 on arduino shield, turns alarm on/off, turns off buzzer if on
const int buzzerPin = 3;
bool outVal = 0;

// i2c data
String inString = "slave";
String outString = "evals";

String alarmOn = "on";
String alarmOff = "off";
const char buzon[] = {'b','u','z','o','n','\0'};
const char buzof[] = {'b','u','z','o','f','\0'};

bool alarmState = 1;
bool buzzerState = 0;


void setup() {
  delay(250);
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  Wire.begin(ARD_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}



void receiveEvent() {
  inString = "";
  while (0 < Wire.available()) {
    byte x = Wire.read();
    inString += (char)x;
  }
}



void requestEvent() {
    if(alarmState == 1)
    {
      outString = alarmOn;
    }
    else
    {
      outString = alarmOff;
    }
    byte response[OUT_ANSWERSIZE];
    for (int i = 0; i < OUT_ANSWERSIZE; i++) {
      response[i] = (byte)outString.charAt(i);
    }

    Wire.write(response,sizeof(response));
}



void loop() {
  
  if(inString.equals(buzon))
  {
    Serial.println("Buzon");
    buzzerState = 1;
  }

  if(buzzerState == 1)
  {
    digitalWrite(buzzerPin, HIGH);
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
  }

 
  if(digitalRead(alarmButtonPin))
  {
    //button has been pressed. 
    if(buzzerState == 1)
    {
      //if buzzer is on, turn it off
      buzzerState = 0;
      alarmState = 0;
    }
    else
    {
      //buzzer is not on, so flip the value of alarm
      alarmState = alarmState == 1 ? 0 : 1;
    }
  }
  
  delay(300);
}
