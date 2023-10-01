#define RED 2
#define YELLOW 3
#define GREEN 4


void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
  delay(6000);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  delay(5000);
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW,HIGH);
  delay(3000);

}
