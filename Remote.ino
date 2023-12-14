/* TrafficLight++ Remote version 1
  Fixes/near future goals
  -Fix bluetooth not sending/receiving data
  -Screen glitches when on for too long
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <math.h>
#include <SoftwareSerial.h>

const byte ROWS = 4; // amount of rows
const byte COLS = 3; // amount of columns
const int size = 3; // size of the time, cant be higher than 3 figures
char time[size + 1]; // increased the size by 1 to accommodate null terminator
int change = 0; // what color to change
int i = 0; // size of the number
bool startScreen = true; // main screen
int redTime = 5; // placeholder for the time in the traffic light
int greenTime = 5; // placeholder for time in the traffic light
bool first = true; 

SoftwareSerial BTSerial(11, 10); // RX | TX

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// pins for the keypad
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int conversion(char num[], int len) {
  int ret = 0;
  for (int i = 0; i < len; i++) {
    if (num[i] >= '0' && num[i] <= '9') {
      ret = ret * 10 + (num[i] - '0');
    }
  }
  return ret;
}

void resetTime() {
  for (int i = 0; i < size; i++) {
    time[i] = ' ';
  }
}

void setup() {
  lcd.backlight();
  lcd.init();
  Serial.begin(9600); // for serial
  BTSerial.begin(38400); // for bluetooth
}

void loop() {
  char customKey = customKeypad.getKey(); // get the key pressed
  // main screen
  if (startScreen) {
    lcd.setCursor(0, 0);
    lcd.print("1: Red, 2: Green");
    lcd.setCursor(2, 1);
    lcd.print(redTime);
    lcd.setCursor(5, 1);
    lcd.print("s");
    lcd.setCursor(10, 1);
    lcd.print(greenTime);
    lcd.setCursor(13, 1);
    lcd.print("s");
    first = true;
    // if the user wants to change the time
    if (customKey == '1' || customKey == '2') {
      time[size] = '\0'; // ensure null termination
      change = customKey - '0'; 
      startScreen = false;
      lcd.clear();
      resetTime(); // reset the time array
      delay(100); // delay to give some time for the display to clear
    }
    delay(100);
  }

  if (!startScreen) {
    lcd.setCursor(0, 0);
    if (customKey && customKey != '#' && !first) {
      if (i < size && customKey != '*') {
        time[i] = customKey;
        i++;
      }
    }

    // Print the currently input numbers
    lcd.print(time);

    // Print blank spaces for the remaining characters
    for (int j = i; j < size; j++) {
      lcd.print(' ');
    }

    delay(100);
    // indicating that the user is done changing the time
    if (customKey == '*') {
      if (change == 1) {
        redTime = conversion(time, i);
        Serial.println(redTime);
        BTSerial.write('R');
        delay(100);
        BTSerial.write(redTime);
      } else if (change == 2) {
        greenTime = conversion(time, i);
        BTSerial.write('G');
        delay(100);
        BTSerial.write(greenTime);
      }
      change = 0;
      startScreen = true;
      lcd.clear();
      resetTime(); // Reset the time array
      i = 0;
    }

    first = false;
    delay(100);
  }
}