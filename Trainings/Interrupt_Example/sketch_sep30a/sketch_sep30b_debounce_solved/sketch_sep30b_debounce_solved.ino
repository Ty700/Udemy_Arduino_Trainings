//Setting up an interrupt
//Uno only has Int pins on 2/3
//This program still is going to solve the debounce - it is just like fixing it without an interrupt
//NOTE: Debounce is a hardware issue, thus best solved in the hardware spectrum & ! software spectrum
#define BUTTON_PIN 2
#define LED_PIN 5
#define debounceDelay 50

byte LEDState = LOW;

//When we use a global var inside of an interrupt, it is best to add "volatile"
volatile bool buttonReleased = false;
volatile unsigned long lastTimeButtonReleased = millis();


//Interrupt functions have data type of void and no parameters
//NOTE: Notice that the debounce check is inside the interrupt
//NOTE: Notice the volatile added to lastTimeButtonReleased
void buttonReleasedInterrupt(){

  // NOTE: We could do the action in the interrupt, but not best practice since the interrupt is like a delay in the sense that everything gets stopped
  //       Thus the time to execute the interrupt throws everything off. Thus it is better to ust change a variable and then return back 
  // LEDState = ~LEDState;
  // digitalWrite(LED_PIN, LEDState);

  //Best practice:
  unsigned long timeNow = millis();

  if((timeNow - lastTimeButtonReleased) > debounceDelay){
    lastTimeButtonReleased = timeNow;
    buttonReleased = true;
  }
}

void toggleLED(){
  LEDState = ~LEDState;
  digitalWrite(LED_PIN, LEDState);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  //Attaches Interrupt to BUTTON_PIN, calls buttonReleasedInterrupt on the signal's falling edge
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonReleasedInterrupt,
                  FALLING
                  );
}

void loop() {
  //buttonReleased is a flag for the interrupt
  if(buttonReleased){
    buttonReleased = false;
    toggleLED();
  }
}
