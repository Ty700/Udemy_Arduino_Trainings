/*
* Goals:
*   - Capture distance from an ultrasonic sensor
*   - Display the rate at which we read from the sensor on row 0
*   - Display the distance the sensor captures on line 1
* 
* Self Added Goals:
*   - Organize code throughout the process of writing it
*
* LCD Example:
*   ---------------
*  |Rate: x ms     |
*  |Distance: xc cm|
*   ---------------
*/

//LIBS
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//ULTRASONIC SENSOR PIN OUT
#define ECHO_PIN 3
#define TRIGGER_PIN 4

//LCD PARAMETERS 
#define ROW 16
#define COL 2
#define ADDR 0x27

//LIMITS
#define MAX_SENSOR_LIMIT 400 //cm
#define PRINT_TIMING_LIMIT 750 //ms => .75s
double previousDistance = 400;

//ECHO INTERRUPT VARS
volatile bool echoResponse = false;
volatile unsigned long echoStart;
volatile unsigned long echoEnd;

//Creating LCD obj
LiquidCrystal_I2C lcd(ADDR, COL, ROW);

//TIMINGS
unsigned long programTime = millis();
unsigned long printTime = millis();

//TIMING DELAYS
unsigned long ultrasonicDelay = 60;
unsigned long lastPrintTime;

//LCD SETUP
void setupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Rate: ");
  lcd.print(ultrasonicDelay);
  lcd.print("ms");

  lcd.setCursor(0,1);
  lcd.print("Distance: ");
}

void setup() {
  //Serial startup => Testing
  Serial.begin(115200);

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoInterrupt,
                  CHANGE
                );

  //Sets up LCD  
  setupLCD();

  //Give time before program starts
  delay(1500);
}


//================DONE WITH SETUP================\\

//ECHO INTERRUPT
void echoInterrupt(){
  if(digitalRead(ECHO_PIN) == HIGH){
    echoStart = micros();
    echoResponse = false;
  } else {
    echoEnd = micros();
    echoResponse = true;
  }
}

//ULTRASONIC SENSOR TRIGGER
void triggerSensor(){
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}

//Checks if enough time has passed in order for it to send out a trigger
bool checkToTriggerSensor(){
  unsigned long timeNow = millis();

  if((timeNow - programTime) > ultrasonicDelay){
    programTime += ultrasonicDelay;
    return true;
  } else {
    return false;
  }
}

//Checks if enough time has passed in order for it to print to lcd
bool checkToPrint(){
  unsigned long timeNow = millis();

  if((timeNow - printTime) > PRINT_TIMING_LIMIT){
    printTime += PRINT_TIMING_LIMIT;
    return true;
  } else {
    return false;
  }
}

//Calculates the distance
double calculateDistance(){
  double distance = (echoEnd - echoStart) / 58;
  return distance;
}

//Filters the distance captured
int filterDistance(int &distance){
  //Filters out outliers
  if(distance > MAX_SENSOR_LIMIT){
    distance = MAX_SENSOR_LIMIT;
  }
  
  //Complimentary Filter
  distance = previousDistance * 0.6 + distance * 0.4;

  previousDistance = distance;

  return distance;
}

//prints a given distance to LCD
void printToLCD(int &distance){
  //Clears the portion the distance is displayed on
  lcd.setCursor(10,1);
  for(int i = 10; i < 16; i++){
    lcd.print(" ");
  }
  //Writes to display
  lcd.setCursor(10,1);
  lcd.print(distance);

  //Changes where the cm is placed for aesthetic reasons
  if(distance < 10){
    lcd.setCursor(11,1);
    lcd.print("cm");
  } else if ((distance >= 10) && (distance < 100)){
    lcd.setCursor(12, 1);
    lcd.print("cm");
  } else if ((distance >= 10) && (distance < 100)){
    lcd.setCursor(12, 1);
    lcd.print("cm");
  } else {
    lcd.setCursor(13,1);
    lcd.print("cm");
  }
}

void loop() {
  //Triggers senor if enough time has passed
  if(checkToTriggerSensor()){
    triggerSensor();
  }

  //If there is an echo response from interrupt it will calculate, filter, and display the distance
  if(echoResponse){
    echoResponse = false;
    
    int distance = calculateDistance();
    filterDistance(distance);

    //print distance to LCD every 1 sec
    if(checkToPrint()){
      printToLCD(distance);
      Serial.println(distance); //For serial plotter & debug
    }
  }
    
}
