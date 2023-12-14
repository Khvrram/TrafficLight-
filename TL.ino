/* TrafficLight++ container version 1
  Fixes/near future goals
  -Fix bluetooth not sending/receiving data
  -Implement GPS code
*/
#include <SoftwareSerial.h>

// pin values
const int red = 5; // red led
const int yellow = 6; // yellow led
const int green = 7; // green led

// times
int redTime = 1; // time in seconds
const int yellowTime = 1; // time in seconds, can't be changed
int greenTime = 1; // time in seconds
const int convert = 1000; // used to convert from second to ms
int change = 0; // which time needs to be changed
String receivedString = "";

SoftwareSerial BTSerial(11, 10); // RX | TX

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(38400);
  BTSerial.begin(38400);
}

void loop() {
  // reading the data from the remote
  if (BTSerial.available()) {
    Serial.println(BTSerial.read());
    char receivedChar = BTSerial.read();
    Serial.println(receivedChar);
    if (receivedChar == 'R') {
      redTime = BTSerial.read();
      Serial.println(redTime);
    } else if (receivedChar == 'G') {
      greenTime = BTSerial.read();
    }
  }
  

  //code for demonstrating the traffic light simulation
  digitalWrite(red, HIGH);
  delay(redTime * convert);
  digitalWrite(red, LOW);
  digitalWrite(yellow, HIGH);
  delay(yellowTime * convert);
  digitalWrite(yellow, LOW);
  digitalWrite(green, HIGH);
  delay(greenTime * convert);
  digitalWrite(green, LOW);

}
