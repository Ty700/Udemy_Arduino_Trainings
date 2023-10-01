/*
*
* Goal: Count how many times you push the button and print it to the cmd
*
*/

#define BUTTON_PIN 2
#define debounceDelay 50

volatile bool buttonPushed = false;
volatile unsigned long lastTimeButtonPushed = millis();

void buttonPushInterrupt(){
  unsigned long timeNow = millis();

  if((timeNow - lastTimeButtonPushed) > debounceDelay){
    buttonPushed = true;
  }  
}

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonPushInterrupt,
                  RISING
                  );
}


int count = 0;
void loop() {
  
  if(buttonPushed){
    count += 1;
    Serial.println(count);

    buttonPushed = false;
  }

}
