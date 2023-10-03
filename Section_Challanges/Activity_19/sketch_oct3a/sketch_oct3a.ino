/*
* Goals: Light LEDS different LEDS depending on photoresistor value
* Note: Easy activity - didn't go crazy with best practices as I am trying to move onto STM controller
*/

#define PHOTO_PIN A0
#define RED_PIN 10
#define YELLOW_PIN 11
#define BLUE_PIN 12

#define PHOTO_READ_DELAY 500

unsigned startTime = millis();

void setup() {
  Serial.begin(115200);

}

void lightLEDS(int lumonistyValue){
  if(lumonistyValue <= 300){
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  } else if((lumonistyValue > 300) && (lumonistyValue <= 800)){
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  } else if(lumonistyValue > 801){
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  }
}

void loop() {
  unsigned timeNow = millis();

  if((timeNow - startTime) > PHOTO_READ_DELAY){
    startTime += PHOTO_READ_DELAY;
    Serial.println(analogRead(PHOTO_PIN));
    lightLEDS(analogRead(PHOTO_PIN));
  }
}
