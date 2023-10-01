#define GREEN_PIN 2
#define BLUE_PIN 3
#define RED_PIN 4

#define NUM_OF_LEDS 3

int GREEN_LED_INTERVAL = 1000
int BLUE_LED_INTERVAL = 1000
int RED_LED_INTERVAL = 1000

unsigned long startTimeGreen = millis();
unsigned long startTimeBlue = millis();
unsigned long startTimeRed = millis();

int LEDS[] = {GREEN_PIN, BLUE_PIN, RED_PIN};

int GREEN_STATE = LOW;
int BLUE_STATE = LOW;
int RED_STATE = LOW;

void setup() {
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.setTimeout(10);
}



void loop() {
  unsigned long timeNow = millis();

  

  if((timeNow - startTimeGreen) > GREEN_LED_INTERVAL){
    //blinkGreen
    GREEN_STATE = ~GREEN_STATE;
    digitalWrite(GREEN_PIN, GREEN_STATE);
    startTimeGreen += GREEN_LED_INTERVAL;
  }

  // if((timeNow - startTimeBlue) > BLUE_LED_INTERVAL){
  //   //blinkBlue
  //   BLUE_STATE = ~BLUE_STATE;
  //   digitalWrite(BLUE_PIN, BLUE_STATE);
  //   startTimeBlue += BLUE_LED_INTERVAL;
  // }

  // if((timeNow - startTimeRed) > RED_LED_INTERVAL){
  //   //blinkRed
  //   RED_STATE = ~RED_STATE;
  //   digitalWrite(RED_PIN, RED_STATE);
  //   startTimeRed += RED_LED_INTERVAL;
  // }

  delayMicroseconds(10);
}
