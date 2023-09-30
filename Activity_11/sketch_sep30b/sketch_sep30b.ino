/*  Activity 11: 
*     Goal: 
*       - Button pressed (LOW -> HIGH) : toggle LED Green & Blue
*         - Toggle LEDS: power on Green, power off Blue      
*       - Make LED Red blink every 1sec
*     LED Red always blinks regardless of button pressed
*
*     Self Added Goal:
*         - Be able to change how fast red blinks without impeding the toggle of Green and Blue
*
*/

#define GREEN_LED 2
#define BLUE_LED 3
#define RED_LED 4
#define BUTTON_PIN 5

#define debounceDelay 50 //ms

int blinkDelay = 1000; //ms

unsigned long lastTimeButtonPressed = millis();
unsigned long blinkCheck = millis();

byte greenState = LOW; //blue will be inverse of green
byte redState = LOW;
byte buttonState = LOW;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);
}

void toggleLEDS(){
  greenState = ~greenState;

  digitalWrite(GREEN_LED, greenState);
  digitalWrite(BLUE_LED, ~greenState);
}

void blinkRed(int delay){
  unsigned long blinkTime = millis();

  if((blinkTime - blinkCheck) > delay){

    blinkCheck += blinkDelay;
    redState = ~redState;
    digitalWrite(RED_LED, redState);
  }

}

void loop() {
  unsigned long timeNow = millis();

  //Debounce prevention 
  if((timeNow - lastTimeButtonPressed) > debounceDelay){
    byte currentButtonState = digitalRead(BUTTON_PIN);
    
    //Button Press or Release?
    if(currentButtonState != buttonState){
      lastTimeButtonPressed = timeNow;
      buttonState = currentButtonState;

      //Button Press?
      if(currentButtonState == HIGH){
        toggleLEDS();
      }
    } 
  }

  //Capture input if any
  if(Serial.available() > 0){
    int data = Serial.parseInt();

    if((data >= 30) && (data <= 5000)){
      blinkDelay = data;
    }
  }

  blinkRed(blinkDelay);
  
}
