/*
* Final Project
* 
* Goals:
*   - Using a ultrasonic sensor, capture and display the distance to an object - Completed
*   - Changed LED Blink Rate depending on the distance
*   - Will update the rest after I finish the previous goal
*
*
*/
//LIBS
#include <LiquidCrystal_I2C.h>

//PIN OUT
#define ECHO_PIN 3
#define TRIGGER_PIN 4

#define RED_PIN 5
#define YELLOW_PIN 6
#define GREEN_PIN 7

#define MAX_DISTANCE_LIMIT 400 //cm

//LCD PARAMETERS
#define ADDR 0x27
#define ROW 16
#define COL 2

//OBJ CREATION(S)
LiquidCrystal_I2C lcd(ADDR, ROW, COL);


//TIMINGS
#define TRIGGER_DELAY 100 //ms
unsigned long lastTriggerTime = millis();

#define PRINT_DELAY 500 //ms
unsigned long lastPrintTime = millis();

unsigned long lastBlinkTime = millis();

#define LOCK_UP_DELAY 500 //ms 
unsigned lastLockupBlinkTime = millis();

//ECHO INTERRUPT VARS
volatile bool newResponse = false;
volatile unsigned long startTime;
volatile unsigned long endTime;

//LED STATES
int RED_STATE = LOW;
int YELLOW_STATE = LOW;
int GREEN_STATE = LOW;

//FOR COMPLIMENTARY FILTER
int previousDistance = 400;

//SETUPS LCD
void setupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
}

void setup() {
  setupLCD();
  Serial.begin(115200);
  
  //PINMODES
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoInterrupt,
                  CHANGE
                );
}

//================DONE WITH SETUP================\\

//ECHO INTERUPT
void echoInterrupt(){
  if(digitalRead(ECHO_PIN) == HIGH){
    startTime = micros();
    newResponse = false;
  } else {
    endTime = micros();
    newResponse = true;
  }
}

//CHECKS IF 60ms (TRIGGER_DELAY) HAS PASSED
//IF SO IT KNOWS TO SEND A NEW TRIGGER
bool checkToTrigger(){
  unsigned long timeNow = millis();
  if((timeNow - lastTriggerTime) > TRIGGER_DELAY){
    lastTriggerTime += TRIGGER_DELAY;
    return true;
  } else {
    return false;
  }
}

//WHEN CALLED SENDS OUT NEW TRIGGER
void newTrigger(){
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}

//CALCULATES THE DISTANCE
//endTime & startTime are global vars changed via ECHO INTERRUPT
int calculateDistance(){
  int distance = (endTime - startTime) / 58;

  //Sensor can only read up to 400cm
  //If it gets a wild reading over the limit it will set the distance to the max limit
  if(distance > MAX_DISTANCE_LIMIT){
    distance = MAX_DISTANCE_LIMIT;
  }

  //Complimentary Filter
  distance = previousDistance * 0.6 + distance * 0.4;

  previousDistance = distance;

  return distance;
}

//CHECKS IF PRINT_DELAY TIMING HAS PASSED TO BE ABLE TO PRINT TO LCD
bool checkToPrintToLCD(){
  unsigned long timeNow = millis();
  if((timeNow - lastPrintTime) > PRINT_DELAY){
    lastPrintTime += PRINT_DELAY;
    return true;
  } else {
    return false;
  }
}

//PRINTS DISTANCE ONTO LCD
void printToLCD(const int distance){
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");

  //Clears the last calculated distance
  for(int i = 10; i < 16; i++){
    lcd.print(" ");
  }
  lcd.setCursor(10, 0);

  lcd.print(distance);

  if((distance <= 9) && (distance >= 0)){
    lcd.setCursor(11,0);
    lcd.print("cm");
  } else if((distance <= 99) && (distance >= 10)){
    lcd.setCursor(12, 0);
    lcd.print("cm");
  } else if (distance >= 100){
    lcd.setCursor(13,0);
    lcd.print("cm");
  }
}

void updateWarningLED(int distance){
  unsigned long timeNow = millis();

  if(distance <= 10){
    if((timeNow - lastLockupBlinkTime) > LOCK_UP_DELAY){
      lastLockupBlinkTime += LOCK_UP_DELAY;
      YELLOW_STATE = ~YELLOW_STATE;
      RED_STATE = YELLOW_STATE;
      digitalWrite(YELLOW_PIN, YELLOW_STATE);
      digitalWrite(RED_PIN, RED_STATE);
    }
    return;
  }

  /*Simple blink delay. 
    - 100cm distance = 500 ms blink delay
    - 50cm distance = 250ms blink delay.

    Objective is as the obj gets closer the blink delay gets shorter.
    distance * 5 results in a good blink delay imo
  */
  int newBlinkDelay = distance * 5;

  if((timeNow - lastBlinkTime) > newBlinkDelay){
    lastBlinkTime += newBlinkDelay;
    YELLOW_STATE = ~YELLOW_STATE;
    digitalWrite(YELLOW_PIN, YELLOW_STATE);
    digitalWrite(RED_PIN, LOW);
  }
}


//MAIN
void loop() {
  if(checkToTrigger()){
    newTrigger();
  }

  if(newResponse){
    int distance = calculateDistance();
    updateWarningLED(distance);
    newResponse = false;
    if(checkToPrintToLCD()){
      printToLCD(distance);
    }    
  }

}
