//IR Button mapping example
#include <IRremote.h>

#define IR_RECEIVE_PIN 5

//BUTTON MAPPING
#define IR_BUTTON_PWR 69
#define IR_BUTTON_MODE 70
#define IR_BUTTON_MUTE 71
#define IR_BUTTON_PLAY 68
#define IR_BUTTON_REWIND 64
#define IR_BUTTON_FORWARD 67
#define IR_BUTTON_EQ 7 
#define IR_BUTTON_VOL_DOWN 21
#define IR_BUTTON_VOL_UP 9
#define IR_BUTTON_0 22
#define IR_BUTTON_1 12
#define IR_BUTTON_2 24
#define IR_BUTTON_3 94
#define IR_BUTTON_4 8
#define IR_BUTTON_5 28
#define IR_BUTTON_6 90
#define IR_BUTTON_7 66
#define IR_BUTTON_8 82
#define IR_BUTTON_9 74



void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop() {
  if(IrReceiver.decode()){
    IrReceiver.resume();
    
    int buttonCMD = IrReceiver.decodedIRData.command;
    
    switch(buttonCMD){

      case IR_BUTTON_PWR:
          // Handle power button
          break;
      case IR_BUTTON_MODE:
          // Handle mode button
          break;
      case IR_BUTTON_MUTE:
          // Handle mute button
          break;
      case IR_BUTTON_PLAY:
          // Handle play button
          break;
      case IR_BUTTON_REWIND:
          // Handle rewind button
          break;
      case IR_BUTTON_FORWARD:
          // Handle forward button
          break;
      case IR_BUTTON_EQ:
          // Handle EQ button
          break;
      case IR_BUTTON_VOL_DOWN:
          // Handle volume down button
          break;
      case IR_BUTTON_VOL_UP:
          // Handle volume up button
          break;
      case IR_BUTTON_0:
          // Handle button 0
          break;
      case IR_BUTTON_1:
          // Handle button 1
          break;
      case IR_BUTTON_2:
          // Handle button 2
          break;
      case IR_BUTTON_3:
          // Handle button 3
          break;
      case IR_BUTTON_4:
          // Handle button 4
          break;
      case IR_BUTTON_5:
          // Handle button 5
          break;
      case IR_BUTTON_6:
          // Handle button 6
          break;
      case IR_BUTTON_7:
          // Handle button 7
          break;
      case IR_BUTTON_8:
          // Handle button 8
          break;
      case IR_BUTTON_9:
          // Handle button 9
          break;
      default:
          // Handle unknown button
          break;
    }      
  }
}
