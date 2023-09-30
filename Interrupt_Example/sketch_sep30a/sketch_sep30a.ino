//Setting up an interrupt
//Uno only has Int pins on 2/3
//This program still has the bounce problem... interrupts don't solve it.

#define BUTTON_PIN 2
#define LED_PIN 5

byte LEDState = LOW;

//When we use a global var inside of an interrupt, it is best to add "volatile"
volatile bool buttonReleased = false;

//Interrupt functions have data type of void and no parameters
void buttonReleasedInterrupt(){
  // NOTE: We could do the action in the interrupt, but not best practice since the interrupt is like a delay in the sense that everything gets stopped
  //       Thus the time to execute the interrupt throws everything off. Thus it is better to ust change a variable and then return back 
  // LEDState = ~LEDState;
  // digitalWrite(LED_PIN, LEDState);

  //Best practice

  buttonReleased = true;
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
