void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

void ledCheck(int pin){

  if(digitalRead(pin) == HIGH){
    Serial.println("LED turned on");
  }

  if(digitalRead(pin) == LOW){
    Serial.println("LED turned off");
  }
}

void loop() {
  digitalWrite(13, HIGH);
  ledCheck(13);
  delay(5000);

  digitalWrite(13,LOW);
  ledCheck(13);
  delay(2000);
  
}
