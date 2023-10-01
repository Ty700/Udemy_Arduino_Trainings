#define RED 2
#define YELLOW 3
#define GREEN 4
#define BUTTON_PIN 5
#define POT_PIN A2

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}


void blink(int time){
  int phase1[2] = {RED, GREEN};

  if(digitalRead(BUTTON_PIN) == LOW){
    for(int i = 0; i < 2; i++){
      digitalWrite(phase1[i], HIGH);
    }
    digitalWrite(YELLOW, LOW);
    delay(time);
  }
  if(digitalRead(BUTTON_PIN) == LOW){
    for(int i = 0; i < 2; i++){
      digitalWrite(phase1[i], LOW);
    }
    digitalWrite(YELLOW, HIGH);
    delay(time);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  int wait = analogRead(POT_PIN) / 3;
  blink(wait);
}
