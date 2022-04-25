/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
#include <LiquidCrystal.h>
const int SW_pin=2;
const int X_pin=0;
const int Y_pin=1;
enum joystickStates{up,down,neutral, still};
enum joystickStates joystickState=still;
int currTask;
void  getCurrentTask(){
  switch(joystickState){
    case up:
      if(analogRead(Y_pin)<550){
        joystickState=neutral;
      }
      if(analogRead(Y_pin)<400){
        joystickState=down;
      }
      else{
        joystickState=still;
      }
      Serial.print("state: up\n");
      break;
    case neutral:
      if(analogRead(Y_pin)>=550){
        joystickState=up;
      }
      else if(analogRead(Y_pin)<=400){
        joystickState=down;
      }
      else{
        joystickState=neutral;
        Serial.print("state: neutral\n");
      }
    break;
    case down:
      if(analogRead(Y_pin)>550){
        joystickState=up;
      }
      if(analogRead(Y_pin)>400){
        joystickState=neutral;
      }
      else{
        joystickState=still;
        
      }
      Serial.print("state: down\n");
    break;
    case still:
      if(analogRead(Y_pin)>400 && analogRead(Y_pin)<550){
        joystickState=neutral;
      }
      Serial.print("state: still\n");
      break;
    default:
    break;
    
  }
  switch(joystickState){
    case up:
      if(currTask<4){
        currTask+=1;
      }
     break;
     case down:
      if(currTask>0){
        currTask-=1;
      }
     break;
     default:
     break;
     
  }
  
}
// include the library code:

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String tasks[5];

void setup() {
  // set up the LCD's number of columns and rows:
  tasks[0]="CS122A Labs";
  tasks[1]="Call Kelly";
  tasks[2]="Email Vahid";
  tasks[3]="Call Max";
  tasks[4]="Meet w devs";
  lcd.begin(16, 2);
  //Print a message to the LCD.
  
  
  pinMode(SW_pin, INPUT);  
  digitalWrite(SW_pin, HIGH);  
  Serial.begin(9600); //must match baud rate!!!
  currTask=0;
  
  lcd.print(currTask+1 +": "+tasks[currTask]);
  
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  //print the number of seconds since reset:
  //lcd.print(millis() / 1000);
    Serial.print("Switch: ");  
    Serial.print(digitalRead(SW_pin));  
    Serial.print("\n");  
    Serial.print("X-axis: ");  
    Serial.print(analogRead(X_pin));
    
    Serial.print(" Y-axis: ");  
    Serial.print(analogRead(Y_pin));  
    Serial.print("\n\n"); 
    getCurrentTask();
    lcd.clear();
    lcd.print(currTask+1);
    lcd.print(": "+tasks[currTask]);
    Serial.print(currTask+1);
    Serial.print(": "+tasks[currTask]+"\n");
    
    delay(100);  
}
