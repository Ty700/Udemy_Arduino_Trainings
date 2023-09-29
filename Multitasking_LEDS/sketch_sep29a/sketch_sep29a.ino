#define GREEN_PIN 2
#define BLUE_PIN 3
#define RED_PIN 4
#define PUSH_PIN 5
#define POT_PIN A0


unsigned long previousTimeLEDBlink = millis();
unsigned long blinkDelay = 500;

int LEDState = LOW;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  
  pinMode(PUSH_PIN, INPUT);
}

void checkButtonPress(){
  if(digitalRead(PUSH_PIN)){
    digitalWrite(RED_PIN, HIGH);
  } else {
    digitalWrite(RED_PIN, LOW);
  }
}

void checkPot(){
  int currentPotVal = (analogRead(POT_PIN) / 4);

  analogWrite(BLUE_PIN, currentPotVal);
} 

void loop() {
  if (Serial.available() > 0) {
    int data = Serial.parseInt();
    if ((data >= 100) && (data <= 4000)) {
      blinkDelay = data;
    }
  }

  unsigned long timeNow = millis();
  if (timeNow - previousTimeLEDBlink > blinkDelay) {
    if (LEDState == LOW) {
      LEDState = HIGH;
    }
    else {
      LEDState = LOW;
    }
    digitalWrite(GREEN_PIN, LEDState);
    previousTimeLEDBlink += blinkDelay;
  }



  checkButtonPress();
  checkPot();

}