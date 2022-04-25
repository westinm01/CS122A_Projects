#include<SoftwareSerial.h> //library for serial communication

SoftwareSerial abc(12,13); //define Rx-Tx pins, you can use other GPIO pins also.
void setup() {
  abc.begin(9600);//Serial communication abc Begin at 9600 Baud
                  //You need take same baud rate for both Nodemcu and arduino
}

void loop() {
  // put your main code here, to run repeatedly:
  abc.write("0");
  delay(1000);
  abc.write("1");
  delay(1000);
}
