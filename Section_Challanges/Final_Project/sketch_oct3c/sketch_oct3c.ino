/*
* Final Project
* 
* Goals:
*   - Using a ultrasonic sensor, capture and display the distance to an object - Completed
*   - Changed LED Blink Rate depending on the distance - Completed
*   - Lock the Application when an obstacle is too close. - Completed
*   - Unlock the application when the user presses the push button - Completed
*   - LCD Setup and Welcome message - Completed
*   - Print distance and warning message to LCD Screen - Completed
*   - Setup IR Remote Controller and map buttons - Completed
*   - Setup program unlocking via the "EQ" button on the remote - Completed
*   - Change and save (EEPROM) the distance unit - Completed
*   - Switch between LCD Screens and Reset Settings - Completed
*   - Print Luminosity and adjust the lighting from the photoresistor - Completed
*   - **FINAL PROJECT COMPLETED**
*
*
*/
//LIBS
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <EEPROM.h>

//PIN OUT
#define ECHO_PIN 3
#define TRIGGER_PIN 4
#define RED_PIN 5
#define YELLOW_PIN 6
#define GREEN_PIN 10
#define RESET_PIN 8
#define IR_PIN 9
#define PHOTO_RES_PIN A0

//THRESHOLDS
#define CM_LOCK_UP_THRESHOLD 10 //cm
#define IN_LOCK_UP_THRESHOLD 4 //inches
#define MAX_CM_DISTANCE_LIMIT 400 //cm
#define MAX_IN_DISTANCE_LIMIT 157 //inches

//LCD PARAMETERS
#define ADDR 0x27
#define ROW 16
#define COL 2

//BUTTON MAPPING
#define IR_BUTTON_PWR 69
#define IR_BUTTON_MODE 70
#define IR_BUTTON_MUTE 71
#define IR_BUTTON_PLAY 68
#define IR_BUTTON_REWIND 64
#define IR_BUTTON_FORWARD 67
#define IR_BUTTON_EQ 7 
#define IR_BUTTON_VOL_DOWN 21
#define IR_BUTTON_VOL_UP 9
#define IR_BUTTON_0 22
#define IR_BUTTON_1 12
#define IR_BUTTON_2 24
#define IR_BUTTON_3 94
#define IR_BUTTON_4 8
#define IR_BUTTON_5 28
#define IR_BUTTON_6 90
#define IR_BUTTON_7 66
#define IR_BUTTON_8 82
#define IR_BUTTON_9 74

//IR REMOTE INPUT VAR
int buttonCMD;

//OBJ CREATION(S)
LiquidCrystal_I2C lcd(ADDR, ROW, COL);

//TRIGGER TIMINGS
#define TRIGGER_DELAY 100 //ms
unsigned long lastTriggerTime = millis();

//PRINT DELAY TIMINGS
#define PRINT_DELAY 500 //ms
unsigned long lastPrintTime = millis();

//BLINK TIMINGS
unsigned long lastBlinkTime = millis();

//PROGRAM LOCK TIMINGS
#define LOCK_UP_DELAY 500 //ms 
unsigned long lastLockupBlinkTime = millis();

//IR TIMINGS
#define IR_DELAY 500 //ms
unsigned long lastIRTime = millis();

//Lumonisity Timings
#define LUMON_DELAY 100
unsigned long lastPhotoResReadTime = millis();

//ECHO INTERRUPT VARS
volatile bool newResponse = false;
volatile unsigned long startTime;
volatile unsigned long endTime;

//LED STATES
int RED_STATE = LOW;
int YELLOW_STATE = LOW;
int GREEN_STATE = LOW;

//LCD MODES
#define LCD_MODE_DISTANCE 1
#define LCD_MODE_SETTINGS 2
#define LCD_MODE_LUMONS 3
int LCD_MODE = LCD_MODE_DISTANCE;

//UNIT MODES
#define CM_MODE 0
#define IN_MODE 1

//EEPROM WRITE ADDR
#define UNIT_ADDR 50

//GLOBAL UNIT VAR
int UNIT = EEPROM.read(UNIT_ADDR);

//GLOBAL BRIGHTNESS VALUE
byte brightness;

//GLOBAL ULTRASONIC SENSOR DISTANCE VARIABLE
int distance;

//FOR COMPLIMENTARY FILTER
int previousCMDistance = MAX_CM_DISTANCE_LIMIT;
int previousINDistance = MAX_IN_DISTANCE_LIMIT;

//PROGRAM STATE
bool PROGRAM_LOCKED = false;

//SETUPS LCD
void setupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");
}

//Main setup
void setup() {
  //LCD Setup
  setupLCD();
  
  //Serial for debug
  Serial.begin(115200);
  
  //IR Receiver setup
  IrReceiver.begin(IR_PIN);

  //PINMODES
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  //INTERRUPTS
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoInterrupt,
                  CHANGE
                );

  //Buffer between setup and running
  delay(1000);
  lcd.clear();
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
  int distance;
  if(UNIT == CM_MODE){
    distance = (endTime - startTime) / 58;

    //Sensor can only read up to 400cm
    //If it gets a wild reading over the limit it will set the distance to the max limit
    if(distance > MAX_CM_DISTANCE_LIMIT){
      distance = MAX_CM_DISTANCE_LIMIT;
    }

    //Complimentary Filter
    distance = previousCMDistance * 0.6 + distance * 0.4;

    previousCMDistance = distance;

    return distance;
  } else {
    distance = (endTime - startTime) / 148;

    //Sensor can only read up to 157in
    //If it gets a wild reading over the limit it will set the distance to the max limit
    if(distance > MAX_IN_DISTANCE_LIMIT){
      distance = MAX_IN_DISTANCE_LIMIT;
    }

    //Complimentary Filter
    distance = previousINDistance * 0.6 + distance * 0.4;

    previousINDistance = distance;

    return distance;
  }
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
void printDistanceToLCD(const int distance){
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");

  //Clears the last calculated distance
  for(int i = 10; i < 16; i++){
    lcd.print(" ");
  }
  lcd.setCursor(10, 0);

  lcd.print(distance);

  if(UNIT == CM_MODE){
    if((distance <= 99) && (distance > 10)){
      if(distance <= 50){
        lcd.setCursor(0,1);
        lcd.print("!!! WARNING !!!");
      }
      lcd.setCursor(12, 0);
      lcd.print("cm");
    } else if (distance >= 100){
      lcd.setCursor(0,1);
      //Clears !!! WARNING !!! if any
      for(int i = 0; i < 17; i++){
        lcd.print(" ");
      }

      lcd.setCursor(13,0);
      lcd.print("cm");
    } 
  } else {
      if((distance <= 99) && (distance > IN_LOCK_UP_THRESHOLD)){
        if(distance <= 15){
          lcd.setCursor(0,1);
          lcd.print("!!! WARNING !!!");
        }

        lcd.setCursor(12, 0);
        lcd.print("in");
      } else if (distance >= 100){
        lcd.setCursor(0,1);
        //Clears !!! WARNING !!! if any
        for(int i = 0; i < 17; i++){
          lcd.print(" ");
        }

        lcd.setCursor(13,0);
        lcd.print("in");
      }
    }
  }

//Changes how fast yellow blinks
//& detects if obj is too close it will lock program
void updateWarningLED(int distance){
  unsigned long timeNow = millis();

  if(UNIT == CM_MODE){
    if(distance <= CM_LOCK_UP_THRESHOLD){
      if((timeNow - lastLockupBlinkTime) > LOCK_UP_DELAY){
        lastLockupBlinkTime += LOCK_UP_DELAY;
        YELLOW_STATE = ~YELLOW_STATE;
        RED_STATE = YELLOW_STATE;
        digitalWrite(YELLOW_PIN, YELLOW_STATE);
        digitalWrite(RED_PIN, RED_STATE);

        lockProgram();
      }
      return;
    } else {
      
    }
  } else {
    if(distance <= IN_LOCK_UP_THRESHOLD){
      if((timeNow - lastLockupBlinkTime) > LOCK_UP_DELAY){
        lastLockupBlinkTime += LOCK_UP_DELAY;
        YELLOW_STATE = ~YELLOW_STATE;
        RED_STATE = YELLOW_STATE;
        digitalWrite(YELLOW_PIN, YELLOW_STATE);
        digitalWrite(RED_PIN, RED_STATE);

        lockProgram();
      }
      return;
    }

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

//Locks the program
void lockProgram(){
  if(!PROGRAM_LOCKED){
    PROGRAM_LOCKED = true;
  }
}

//Unlocks program
void unlockProgram(){
  if(PROGRAM_LOCKED){
    PROGRAM_LOCKED = false;
  }
}

//LCD Screen while program is locked
void printLockUpWarning(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("!!!WARNING!!!");
  lcd.setCursor(0, 1);
  lcd.print("OBJECT DETECTED");
  delay(1000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("PRESS 'EQ'");
  lcd.setCursor(1, 1);
  lcd.print("OR PUSH BUTTON");
  delay(1000);
}

//Makes sure LCD_MODE is a valid number
void validateLCDMode(){
  if(LCD_MODE < LCD_MODE_DISTANCE){
      LCD_MODE = LCD_MODE_LUMONS;
    }

  if(LCD_MODE > LCD_MODE_LUMONS){
    LCD_MODE = LCD_MODE_DISTANCE;
  }
}

//Updates Unit mode
void updateUnitMode(){
  if(buttonCMD == IR_BUTTON_MODE){
    if(UNIT == CM_MODE){
      UNIT = IN_MODE;
    } else {
      UNIT = CM_MODE;
    }
    EEPROM.write(UNIT_ADDR, UNIT);
  }
}

//Will display a different screen depending on LCD_MODE
void toggleLCDScreen(int LCD_MODE){
  switch(LCD_MODE){
    case (LCD_MODE_DISTANCE):
      //Display Distance
      printDistanceToLCD(distance);
      break;
    case (LCD_MODE_SETTINGS):
      printSettings();
      break;
    case (LCD_MODE_LUMONS):
      lcd.clear();
      printLuminosity();
      break;
  }
}

//Checks if user pushed a button on IR remote
bool checkForNewIR(){
  unsigned long timeNow = millis();
  if((timeNow - lastIRTime) > IR_DELAY){
    lastIRTime += IR_DELAY;
    return true;
  } else {
    return false;
  }
}

//Resets the program to default settings
void resetSettings(){
  if(LCD_MODE == LCD_MODE_SETTINGS){
    UNIT = CM_MODE;
    EEPROM.write(UNIT_ADDR, UNIT);
    lcd.clear();
    lcd.print("SETTINGS CLEARED");
    delay(1000);
  }
}

//Completes the action for the button the user pushed on the IR Remote
void completeIRButtonAction(const int &command){
  switch(command){
    case (0):
      //Do nothing
      break;
    
    case (IR_BUTTON_MODE):
      updateUnitMode();
      break;

    case (IR_BUTTON_FORWARD):
      LCD_MODE += 1;
      validateLCDMode();
      break;
    
    case(IR_BUTTON_REWIND):
      LCD_MODE -= 1;
      validateLCDMode();
      break;
    case(IR_BUTTON_PWR):
      resetSettings();
      break;
    default:
      //Do nothing
      break;
  }
}

//updates the value for green's luminosity
void changeGreenBrightness(){
  unsigned long timeNow = millis();
  if((timeNow - lastPhotoResReadTime) > LUMON_DELAY){
    lastPhotoResReadTime += LUMON_DELAY;
    int luminosity = analogRead(PHOTO_RES_PIN);
    setGreenBrightness(luminosity);
  }
}

//sets green led brightness to result from previous function
void setGreenBrightness(const int &luminosity){
  brightness = 255 - (luminosity / 4);
  analogWrite(GREEN_PIN, brightness);
}

//print screen for luminonsity LCD_MODE
void printLuminosity(){
  lcd.print("Luminosity: ");
  lcd.print(brightness);
}

//print screen for settings LCD_MODE
void printSettings(){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PRESS POWER");
  lcd.setCursor(0, 1);
  lcd.print("TO RESET SETTING"); 
}

//MAIN
void loop() {
  if(checkToTrigger()){
    newTrigger();
  }

  //Updates value for buttonCMD depending if new IR input was detected
  if(checkForNewIR()){
    //Updates buttonCMD with new input from remote
    if(IrReceiver.decode()){
      IrReceiver.resume();
      buttonCMD = IrReceiver.decodedIRData.command;
      completeIRButtonAction(buttonCMD);
    }
  }

  //if new echo response
  if(newResponse){
    //Reseting flag for detecting a new echo
    newResponse = false;

    //Checks if program is locked
    if(PROGRAM_LOCKED){
      if(UNIT = CM_MODE){
        updateWarningLED(CM_LOCK_UP_THRESHOLD);
      } else {
        updateWarningLED(IN_LOCK_UP_THRESHOLD);
      }

      if(checkToPrintToLCD()){
        printLockUpWarning();
      }

      //If user pushes the button or press EQ on the remote, it will unlock program
      if(digitalRead(RESET_PIN) == HIGH || buttonCMD == IR_BUTTON_EQ){
        unlockProgram();
        lcd.clear();
        //Allows user to get hand out of the way to avoid dupe lock ups
        delay(1000);
      }

    //If Program isn't locked...
    } else {
      //Updates which unit to display
      distance = calculateDistance();

      updateWarningLED(distance);

      if(checkToPrintToLCD()){
        updateUnitMode();
        toggleLCDScreen(LCD_MODE);
      } 
    }
  }

  changeGreenBrightness();

  //RESETING FLAG
  buttonCMD = 0;
}