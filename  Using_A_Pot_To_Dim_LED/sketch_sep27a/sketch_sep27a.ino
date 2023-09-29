#define POT_PIN A1
#define LED_PIN 6

//Analog Reading Inputs
#define firstThreshold 205
#define secondThreshold 410
#define thirdThreshold 615
#define fourthThreshold 820
#define fifthThreshold 1025


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(4, INPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(analogRead(POT_PIN) == 0){
    analogWrite(LED_PIN,0);  
  }else if(analogRead(POT_PIN) <= 205 && analogRead(POT_PIN) > 0){
    analogWrite(LED_PIN, 51);
  } else if (analogRead(POT_PIN) > firstThreshold && analogRead(POT_PIN) <= secondThreshold){
    analogWrite(LED_PIN, 102);
  } else if (analogRead(POT_PIN) > secondThreshold && analogRead(POT_PIN) <= thirdThreshold){
    analogWrite(LED_PIN,153);
  } else if (analogRead(POT_PIN) > thirdThreshold && analogRead(POT_PIN) <= fourthThreshold){
    analogWrite(LED_PIN,204);
  } else {
    analogWrite(LED_PIN,255);
  }

  Serial.println(analogRead(POT_PIN));
}
