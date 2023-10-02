//Solves pulseIn delay
/*
* How?
*   - Interrupt on rising edge of echo
*     -captures startTime <- global var
*   - Interrupt on falling edge of echo
*     - captures endTime <- global var
*     - sets Flag
*
*   - In loop we:
*     - check for a flag
*     - timeDuration = endTime - startTime
*     - distance = timeDuration / 58.0
*     -print distance       
*/

#define ECHO_PIN 3
#define TRIGGER_PIN 8

unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 500;

//Echo interrupt
volatile unsigned long timeStart = 0;
volatile unsigned long timeEnd = 0;
volatile bool echo = false;

void setup() {
  Serial.begin(115200);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoInterrupt,
                  CHANGE
                );

}
//set trigger to high for 10us
void triggerUltrasonicSensor(){
  //Setting low just in case
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}

void echoInterrupt(){
  if(digitalRead(ECHO_PIN) == HIGH){
    timeStart = micros();
  } else {
    timeEnd = micros();
    echo = true;
  }
}

double getUltrasonicDistance(){

  // unsigned long timeBegin = micros();
  
  // double timeDurationInMicros = pulseIn(ECHO_PIN, HIGH);

  // unsigned long timeAfterPulseIn = micros();

  // unsigned long timeForPulseInToRun = timeAfterPulseIn - timeBegin;

  // Serial.print("Duration: ");
  // Serial.println(timeForPulseInToRun);

  double timeDurationInMicros = timeEnd - timeStart;

  double distanceInCentimeters = timeDurationInMicros / 58.0;
  // double ditanceInInches = distanceInCentimeters / 2.54;

  return distanceInCentimeters;
  
}

void loop() {
  unsigned long timeNow = millis();

  if((timeNow - lastTimeUltrasonicTrigger) > ultrasonicTriggerDelay){
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUltrasonicSensor();
  }

  if(echo){
    Serial.println(getUltrasonicDistance());
    echo = false;
  }

  //OLD WAY:
  // if((timeNow - lastTimeUltraSonicTrigger) > ultrasonicTriggerDelay){
  //   lastTimeUltraSonicTrigger += ultrasonicTriggerDelay;
  //   //Trigger Sensor
  //   triggerUltrasonicSensor();
  //   //Read pulse
  //   Serial.println(getUltrasonicDistance());
  // }
}
