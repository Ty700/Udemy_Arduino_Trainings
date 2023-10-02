//Setting up ultrasonic sensor example

#define ECHO_PIN 3
#define TRIGGER_PIN 8

unsigned long lastTimeUltraSonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 100;



void setup() {
  Serial.begin(115200);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

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

double getUltrasonicDistance(){
  double timeDurationInMicros = pulseIn(ECHO_PIN, HIGH);

  double distanceInCentimeters = timeDurationInMicros / 58.0;

  // double ditanceInInches = distanceInCentimeters / 2.54;
  return distanceInCentimeters;
}

void loop() {
  unsigned long timeNow = millis();

  if((timeNow - lastTimeUltraSonicTrigger) > ultrasonicTriggerDelay){
    lastTimeUltraSonicTrigger += ultrasonicTriggerDelay;
    //Trigger Sensor
    triggerUltrasonicSensor();
    //Read pulse
    Serial.println(getUltrasonicDistance());
  }
}
