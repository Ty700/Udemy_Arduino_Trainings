#define LED_PIN 6

#define lowestDelayTime 100
#define highestDelayTime 1000

#define dimLumonisty 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(50);

  pinMode(LED_PIN, OUTPUT);

  delay(1000);
  printSelectionMenu();
}

//Default values
int delayTime = 1000;
int brightness = HIGH;

//Makes LED blink at a give time in ms
void blink(int time){
  digitalWrite(LED_PIN, HIGH);
  delay(time);
  digitalWrite(LED_PIN, LOW);
  delay(time);
}

//Selection Menu
void printSelectionMenu(){
  String menu[] = {"Change Delay", "Change Brightness"};

  int sizeOfMenu = sizeof(menu)/sizeof(menu[0]);

  Serial.println("===MENU===");
  for(int i = 0; i < sizeOfMenu; i++){
      int value = i+1;
      String temp = ". " + menu[i];
      String output = value + temp;

      Serial.println(output);
  }
}

//Delay Menu
void printDelayMenu(){
  char buffer[50];
  sprintf(buffer, "Enter a number from %d to %d", lowestDelayTime, highestDelayTime);
  Serial.println(buffer);
}


int getInput(){
  return Serial.parseInt();
}

bool validateDelayInput(int input){
  if(input >= lowestDelayTime && input <= highestDelayTime){
    return 1;
  } else {
    return 0;
  }
}



//Brightness Menu
// void printBrightnessMenu(){
//   int percents[6]  = {5, 10, 25, 50, 75, 100};

//   int percentSize = sizeof(percents)/percents[0];

//   Serial.println("Choose a percentage: ");

//   for(int i = 0; i < 6; i++){
//     int value = i+1;
//     String temp = String(percents[i]);


//     Serial.println(output);
//   }
// }

//Wait
void wait(){
  while(Serial.available() == 0){
        //Do nothing
  }
}

void loop() {

  if(Serial.available() > 0){
    int selectionInput = Serial.parseInt();

    if(selectionInput == 1){
      //Print menu
      printDelayMenu();

      //Wait for user input
      wait();
      
      //Caputure user input
      int userInput = getInput();
      
      //Check if input is valid
      if(validateDelayInput(userInput)){
        //if valid change delay time
        delayTime = userInput;
      } else {
        Serial.println("Invalid Option");
      }

    } else if (selectionInput == 2){
      //Can't get this to work.

      //print brightnessMenu always looks funny like something is in the buffer still
      
    } else {
      Serial.println("Invalid Menu Selection");
    }
    printSelectionMenu();
  }

  blink(delayTime);

}

    // int input = Serial.read();
    
    // if(input >= lowestDelayTime && input <= highestDelayTime){
    //   delayTime = input;
    //   Serial.print("Read: ");
    //   Serial.println(delayTime);
      
    //   Serial.print("Delay: ");
    //   Serial.println(delayTime);
      
    // } else if(input == "Dim"){
    //   Serial.print(input);
    //   Serial.println("Changing brightness to 5%");
    //   brightness = dimLumonisty;

    // } else if(input == "Full"){
    //   Serial.print(input);
    //   Serial.println("Changing brightness to 100%");
    //   brightness = 1;
    // }
