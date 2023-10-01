/*
* Goals: 
*   - LED fade in/out from pot
*   - max intensity from serial
*     - ex: sending 150 means instead of 0-255 it becomes 0-150
*   - save max intensity to EEPROM
*
*   - Self Goals: 
*     - To save max intensity to EEPROM, you have to push the button
*/

#include <EEPROM.h>

#define LED_PIN 3
#define POT_PIN A0
#define BUTTON_PIN 2
#define debounceDelay 50
#define EERPROM_SAVE_ADDR 0

unsigned long lastTimeButtonPressed = millis();

volatile bool buttonPressed = false;
bool maxIntensityLimit = false;

byte maxIntensity = 255;

int runCounter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonPressInterrupt,
                  RISING
                );

  delay(1500);
  Serial.println("Getting max intensity...");
  delay(500);
  Serial.println("Retrieved max intensity");
  delay(250);
  Serial.print("Max intensity: ");
  Serial.println(EEPROM.read(0));
  
  maxIntensity = EEPROM.read(0);
}

void buttonPressInterrupt(){
  unsigned long timeNow = millis();

  if((timeNow - lastTimeButtonPressed) > debounceDelay){
    lastTimeButtonPressed = timeNow;
    buttonPressed = true;
  }

}

void changeMaxIntensity(int newMax){
  maxIntensity = newMax;
}

void loop() {
  byte currentIntensity = (analogRead(POT_PIN) / 4);

  if(Serial.available() > 0){
    byte data = Serial.parseInt();
    
    if((data >= 0) && (data <= 255)){
      changeMaxIntensity(data);
      Serial.print("New max intensity: ");
      Serial.println(maxIntensity);
    }
  }

  if(buttonPressed){
    //save value
    Serial.print("Saving: ");
    Serial.print(maxIntensity);
    Serial.print(" to EEPROM Addr: ");
    Serial.println(EERPROM_SAVE_ADDR);

    EEPROM.write(EERPROM_SAVE_ADDR, maxIntensity);
    buttonPressed = false;
  }

  //This way saves on memory (I think?)
  if(currentIntensity >= maxIntensity){
    maxIntensityLimit = true;
    analogWrite(LED_PIN, maxIntensity);
  } else {
    maxIntensityLimit = false;
    analogWrite(LED_PIN, currentIntensity);
  }

  if(runCounter % 10000 == 0){
    Serial.println(currentIntensity);
    if(maxIntensityLimit){
      Serial.println("MAX INTENSITY LIMIT");
    } else {
      Serial.println("POT_PIN LIMIT");
    }
  }
  runCounter += 1;
}
