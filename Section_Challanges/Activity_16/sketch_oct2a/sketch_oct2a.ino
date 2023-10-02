/*
* Goals: 
*   - Capture an input from serial and display it on the LCD Screen
*   
* Self Added Goals:
*   - Automatically switch between the lines and clear each one when new input is recieved
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//LCD PARAMETERS
#define ROW 16
#define COL 2
#define ADDRESS 0x27 //Get this from using the I2C port capture

//Makes an obj called lcd
//with parameters specifed above
LiquidCrystal_I2C lcd(ADDRESS,COL,ROW);

void setup(){

  Serial.begin(115200);
  Serial.setTimeout(10);

  lcd.init();
  lcd.backlight();

  //(0.0) - top left
  //(0,1) - bottom left
  //(COL, ROW)
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();
}

bool checkForInput(){
  if(Serial.available() > 0){
    return true;
  } else {
    return false;
  }
}

void clearRow(int rowToClear){
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, rowToClear);
    lcd.print(" ");
  }
}

int currentRow = 0;

void loop()
{ 
  if(checkForInput()){
    if(currentRow == 0){
      clearRow(0);
      lcd.setCursor(0, 0);
      lcd.print(Serial.readString());
      currentRow = 1;
    } else {
      clearRow(1);
      lcd.setCursor(0, 1);
      lcd.print(Serial.readString());
      currentRow = 0;
    }
  }
}
