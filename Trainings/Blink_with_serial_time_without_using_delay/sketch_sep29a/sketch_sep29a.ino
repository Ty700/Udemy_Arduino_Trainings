#define GREEN_PIN 2
#define BLUE_PIN 3
#define RED_PIN 4

int greenState = LOW;
int blueState = LOW;
int redState = LOW;

int greenBlinkDelay = 500;
int blueBlinkDelay = 500;
int redBlinkDelay = 500;

unsigned long greenStartTime = millis();
unsigned long blueStartTime = millis();
unsigned long redStartTime = millis();

String LEDS[] = {"Green", "Blue", "Red"};
#define arrSize 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  delay(1000);

  printLEDS();

  Serial.flush();
}

void printLEDS(){
  Serial.println("\nWhich LED: ");
  for(int i = 0; i < arrSize; i++){
    Serial.print(i+1);
    Serial.print(". ");
    Serial.println(LEDS[i]);
  }
}

int gatherInput(){
  int LED_PIN = -1;
  if(Serial.available() > 0){
    LED_PIN = Serial.parseInt();
    return(LED_PIN);
  }
  return LED_PIN;
}

//Need to figure out how to get rid of the while loop
void changeFrequency(int LED_PIN){
  if(LED_PIN == 1){
    Serial.println("Enter Frequency for Green: ");
    while(Serial.available() == 0) {}
    int freq = Serial.parseInt();
    greenBlinkDelay = freq;

  } else if(LED_PIN == 2){
    Serial.println("Enter Frequency for Blue: ");
    while(Serial.available() == 0) {}
    int freq = Serial.parseInt();
    blueBlinkDelay = freq;

  } else if(LED_PIN == 3){

    Serial.println("Enter Frequency for Red: ");
    while(Serial.available() == 0) {}
    int freq = Serial.parseInt();
    redBlinkDelay = freq;
  }

  printLEDS();
}

//Makes more since than millis() to me
unsigned long getCurrentTime(){
  return millis();
}

//Way to automate this?
void blinkLEDS(){
  unsigned long timeNow = getCurrentTime();
  if((timeNow - greenStartTime) > greenBlinkDelay){
    greenStartTime += greenBlinkDelay;
    greenState = ~greenState;
    digitalWrite(GREEN_PIN, greenState);
  }

  timeNow = getCurrentTime();
  if((timeNow - blueStartTime) > blueBlinkDelay){
    blueStartTime += blueBlinkDelay;
    blueState = ~blueState;
    digitalWrite(BLUE_PIN, blueState);
  }

  timeNow = getCurrentTime();
  if((timeNow - redStartTime) > redBlinkDelay){
    redStartTime += redBlinkDelay;
    redState = ~redState;
    digitalWrite(RED_PIN, redState);
  }
}

void loop() {
  int LED_PIN = gatherInput();

  if(LED_PIN != -1){
    changeFrequency(LED_PIN);
  }

  blinkLEDS();
}
