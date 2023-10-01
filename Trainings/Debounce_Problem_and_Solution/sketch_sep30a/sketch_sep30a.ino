//Debounce Example

//Sometimes pressing and releasing it will "bounce" and detect more than one button presses & releases

//To fix we add a "debounce delay" via the time functionality

#define BUTTON_PIN 5
#define debounceDelay 750

unsigned lastTimeButtonChanged = micros();


byte buttonState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  unsigned long timeNow = micros();

  if(timeNow - lastTimeButtonChanged > debounceDelay){
    byte newButtonState = digitalRead(BUTTON_PIN);

    if(newButtonState != buttonState){
      lastTimeButtonChanged = timeNow;
      buttonState = newButtonState;

      if(buttonState == HIGH){
        Serial.println("Button Pressed.");
      } else {
        Serial.println("Button Released");
      }
    }

  }

  

}

