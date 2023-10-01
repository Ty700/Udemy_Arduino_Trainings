//EEPROM
//Will be able to read from the addresses even if Arduino restarts
//Each address has about a 100,000 write limit
//Can read an inf amount of times but write is limited

#include <EEPROM.h>

void setup() {
  Serial.begin(115200);

  // EEPROM.write(0, 44);
  // EEPROM.write(200, 150);

  delay(1500);

  //Reads EEPROM addr 0
  Serial.println(EEPROM.read(0));
  Serial.println(EEPROM.read(200));
  Serial.println(EEPROM.read(24));

  //Found out unwritten address have a default value of 255
}

void loop() {

}
