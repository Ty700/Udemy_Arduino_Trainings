/*
*
* Goals: 
*   - Program a remote to light up 4 leds
*     - Button 0 -> Power off leds
*     - Button 1 -> Toggle RED
*     - Button 2 -> Toggle YELLOW
*     - BUTTON 3 -> Toggle BLUE
*     - BUTTON 5 -> Clear LCD
*
*   - LCD: 
*        -1st Line -> Data (CMD from button)
*       -2nd Line -> Text stating what CMD is being done
*         - Example: "Toggle RED LED"
*
*/

//LIBS
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

//PINOUT
#define IR_PIN 5
#define RED_PIN 10
#define YELLOW_PIN 11
#define BLUE_PIN 12

//LCD PARAMETERS
#define ROW 2
#define COL 16
#define ADDR 0x27

//BUTTON MAPPING
#define IR_BUTTON_0 22
#define IR_BUTTON_1 12
#define IR_BUTTON_2 24
#define IR_BUTTON_3 94
#define IR_BUTTON_5 28

//TIMINGS
#define irDelay 500
unsigned long startTime = millis();
unsigned long lastIRTime;

//LED STATES
int redState = LOW;
int yellowState = LOW;
int blueState = LOW;

//IR STATE
int previousCMD;

//LCD OBJ
LiquidCrystal_I2C lcd(ADDR, COL, ROW);

//LED ARRAY & SIZE
int LEDS[] = {RED_PIN, YELLOW_PIN, BLUE_PIN};
int sizeofLEDS = sizeof(LEDS)/sizeof(LEDS[0]);

//SETUPS LEDS FOR OUTPUT
void setupLEDS(){
  for(int i = 0; i < sizeofLEDS; i++){
    pinMode(LEDS[i], OUTPUT);
  }
}

//STARTS LCD
void setupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_PIN);

  setupLCD();
  setupLEDS();
}

void turnOnLEDS(){
  for(int i = 0; i < 3; i++){
    digitalWrite(LEDS[i], HIGH);
  }
}

void turnOffLEDS(){
  for(int i = 0; i < sizeofLEDS; i++){
    digitalWrite(LEDS[i], LOW);
  }
}

void writeToLCD(int version){
  lcd.print("Function:");
  
  //Clears row I write to
  lcd.setCursor(0, 1);
  for(int i = 0; i < 16; i++){
    lcd.print(" ");
  }

  lcd.setCursor(0, 1);
  switch(version){
    case 0:
      
      lcd.print("Turning off LEDS");
      break;
  
    case 1:
      lcd.print("Toggling Red");
      break;
    
    case 2:
      lcd.print("Toggling Yellow");
      break;
    
    case 3:
      lcd.print("Toggling Blue");
      break;
    
    case 5:
      for(int i = 0; i < 16; i++){
        lcd.print(" ");
      }
    
    default:
      lcd.print("Select Action");
      break;
    
  }
}


void toggleLED(int pin){
  if(pin == RED_PIN){
    redState = ~redState;
    digitalWrite(RED_PIN, redState);
  } else if (pin == YELLOW_PIN){
    yellowState = ~yellowState;
    digitalWrite(YELLOW_PIN, yellowState);
  } else if (pin == BLUE_PIN){
    blueState = ~blueState;
    digitalWrite(BLUE_PIN, blueState);
  }
}

void loop() {
  unsigned long timeNow = millis();
  if((timeNow - startTime) > irDelay){
    startTime += irDelay;

    if(IrReceiver.decode()){
      IrReceiver.resume();

      int buttonCMD = IrReceiver.decodedIRData.command;
      

      if(buttonCMD == 0){
        return;
      }

      Serial.println(buttonCMD);

      switch(buttonCMD){
        case IR_BUTTON_0:
          turnOffLEDS();
          writeToLCD(0);
          break;
        case IR_BUTTON_1:
          //Toggle RED
          toggleLED(RED_PIN);
          writeToLCD(1);
          break;
        case IR_BUTTON_2:
          //Toggle YELLOW
          toggleLED(YELLOW_PIN);
          writeToLCD(2);
          break;
        case IR_BUTTON_3:
          //Toggle BLUE
          toggleLED(BLUE_PIN);
          writeToLCD(3);
          break;
        case IR_BUTTON_5:
          //CLEAR LCD
          writeToLCD(5);
          break;

        default:
          writeToLCD(-1);
      }
    }
  }
}


