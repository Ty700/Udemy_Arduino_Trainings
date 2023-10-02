/*
*
* Goals: 
*   - Light different LEDs depending on how far something is
*     - distance > 100cm => Green lights up
*     - distance 15 - 100cm => Yellow Lights up
*     - dtance 0 - 15 => Red Lights up
*/

#define ECHO_PIN 3
#define TRIGGER_PIN 8

#define RED_PIN 5
#define GREEN_PIN 7
#define YELLOW_PIN 6

#define SENSOR_DISTANCE_LIMIT 400
double previousDistance = 400.0;

unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 60;

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

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  delay(1500);
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

  double timeDurationInMicros = timeEnd - timeStart;

  double distanceInCentimeters = timeDurationInMicros / 58.0;

  //Filters out the random noise. Sensor can only detect up to 400cm
  if(distanceInCentimeters > SENSOR_DISTANCE_LIMIT){
    return SENSOR_DISTANCE_LIMIT;
  }

  distanceInCentimeters = (previousDistance * 0.7) + (distanceInCentimeters * 0.3);

  previousDistance = distanceInCentimeters;

  return previousDistance;
  
}

void warningCheck(double distance){
  if(distance > 100){
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
  } else if ((distance <= 100) && (distance > 15)){
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
  } else {
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
  }
}

void loop() {
  unsigned long timeNow = millis();

  if((timeNow - lastTimeUltrasonicTrigger) > ultrasonicTriggerDelay){
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUltrasonicSensor();
  }

  
  if(echo){
    double distance = getUltrasonicDistance();
    warningCheck(distance);
    Serial.println(distance);
    echo = false;
  }

}
