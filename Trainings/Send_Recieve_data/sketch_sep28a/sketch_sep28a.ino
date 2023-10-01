void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(2); //Tells Arduino how long to way for data. Default is 1000ms -> 1sec
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    String var = Serial.readString();
    Serial.print("Read: ");
    Serial.println(var);
  }
}
