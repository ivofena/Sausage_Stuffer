#ifndef CUSTOM_KEYPAD_H_
#define CUSTOM_KEYPAD_H_

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char DecimalKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad t_Keypad = Keypad( makeKeymap(DecimalKeys), rowPins, colPins, ROWS, COLS); 

#endif
