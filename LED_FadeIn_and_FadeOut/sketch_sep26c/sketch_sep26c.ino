//Making an LED Fade in and Fade out

#define LED_PIN 10 
#define stepSize 17 //PWM value

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, HIGH); 
  // analogWrite(LED_PIN, 25);
  // delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i <= 255; i += stepSize){
    analogWrite(LED_PIN, i);
    delay(100);
  }

  for(int i = 255; i >= 0; i -= stepSize){
    analogWrite(LED_PIN, i);
    delay(100);
  }
}
