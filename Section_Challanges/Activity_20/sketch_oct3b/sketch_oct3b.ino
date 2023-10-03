/*
* Goals:
*   - Get lumonisty sample every 50ms, for 5 seconds
*   - Compute the average
*
* Self Added Goals:
*   - Display result onto LCD
*/

//LIBS
#include <LiquidCrystal_I2C.h>

//PHOTO RESISTOR PIN
#define PHOTO_PIN A0

//LCD PARAMETERS
#define ADDR 0x27
#define ROW 16
#define COL 2

//TIMINGS
#define PHOTO_READ_DELAY 50
unsigned long startTime = millis();

#define FIVE_SECONDS 5000
unsigned long fiveSecondCheck = millis();

//OBJ CREATION(S)
LiquidCrystal_I2C lcd(ADDR, ROW, COL);

//SAMPLE PARAMETERS
#define SAMPLE_SIZE 100

//SAMPLE ARRAY
int lumonistySamples[SAMPLE_SIZE];

//LCD SETUP
void setupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

//MAIN SETUP
void setup() {
  Serial.begin(115200);
  setupLCD();
  delay(1500);
}

//==============SETUP DONE==============\\

//Checks to see if 5 seconds has passed
bool hasFiveSecondsPassed(){
  unsigned long timeNow = millis();
  if((timeNow - fiveSecondCheck) > FIVE_SECONDS){
    fiveSecondCheck += FIVE_SECONDS;
    return true;
  } else {
    return false;
  }
}

//Checks to see if 50ms has passed
bool validTimeToReadPhoto(){
  unsigned long timeNow = millis();
  if((timeNow - startTime) > PHOTO_READ_DELAY){
    startTime += PHOTO_READ_DELAY;
    return true;
  } else {
    return false;
  }
}

//Captures the current photoresistor value and assigned it to a location in the given array
void captureSample(int *arr, int sample){
  static int currentLoc = 0;

  arr[currentLoc] = sample;

  currentLoc += 1;

  if(currentLoc == 101){
    currentLoc = 0;
  }

}

//Calculates the sum of a given array
unsigned long calculateSum(int *arr){
  unsigned long sum = 0;
  for(int i = 0; i < SAMPLE_SIZE; i++){
    sum += arr[i];
  }
  return sum;
}

//Calculates the average of the given sum
double calculateAvgOfSample(const unsigned long &sum){
  return (sum / SAMPLE_SIZE);
}

//Prints the given avg to an LCD
void printToLCD(const double &avg){
  lcd.clear();
  lcd.print("Average: ");
  lcd.print(avg);

}

//=======DEBUG SECTION=======\\
//Prints all the samples it gathered - for debug
// void printSample(){
//   for(int i = 0; i < SAMPLE_SIZE; i++){
//     Serial.print(i);
//     Serial.print(". ");
//     Serial.println(lumonistySamples[i]);
//   }
// }

// void debug(){
//   Serial.print("Sum: ");
//   Serial.println(sum);
//   printToLCD(avg);
//   Serial.print("Average: ");
//   Serial.println(avg);
//   printSample();
//   Serial.println("________________________________________________");
// }

//=======END OF DEBUG SECTION=======\\

void loop() {

  if(validTimeToReadPhoto()){
    int currentSample = analogRead(PHOTO_PIN);
    captureSample(lumonistySamples, currentSample);
  }

  if(hasFiveSecondsPassed()){
    unsigned long sum = calculateSum(lumonistySamples);
    double avg = calculateAvgOfSample(sum);
    printToLCD(avg);
    //debug(); //DEBUG
  }
}
