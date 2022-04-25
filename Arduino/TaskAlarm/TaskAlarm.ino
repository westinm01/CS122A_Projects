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
const int tagButton=3;
bool organizeByTag;
enum tagButtonState{tagOff, tagOn};
enum tagButtonState tagButtonState=tagOff;
enum joystickStates{up,down,neutral, still};
enum joystickStates joystickState=still;
int currTask;
char col;
int row;
String cellID;
String command;
const int len = 60;
const int taskAmount=20;
char my_str[len];
char pos = 0;
bool change=false;


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
      //Serial.print("state: up\n");
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
        //Serial.print("state: neutral\n");
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
      //Serial.print("state: down\n");
    break;
    case still:
      if(analogRead(Y_pin)>400 && analogRead(Y_pin)<550){
        joystickState=neutral;
      }
      //Serial.print("state: still\n");
      break;
    default:
    break;
    
  }
  switch(joystickState){
    case up:
      if(currTask<taskAmount-1){
        currTask+=1;
        row+=1;
        change=true;
      }
     break;
     case down:
      if(currTask>0){
        currTask-=1;
        row-=1;
        change=true;
      }
     break;
     default:
     break;
     
  }
  
}

void tagButtonCheck(){
  switch(tagButtonState){
    case tagOff:
      if(organizeByTag){
        tagButtonState=tagOn;
      }
      break;
    case tagOn:
      if(organizeByTag){
        tagButtonState=tagOff;
      }
  }
}

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String tasks[taskAmount];
String taskTags[taskAmount];
String taskDueDates[taskAmount]; //format: MM/DD

void setup() {
  
  tasks[0]="CS122A Labs"; taskTags[0]="CS122A"; taskDueDates[0]="04/29";
  tasks[1]="Call Kelly"; taskTags[1]="Family"; taskDueDates[1]="04/25";
  tasks[2]="Email Vahid"; taskTags[2]="CS122A"; taskDueDates[2]="04/30";
  tasks[3]="Call Max"; taskTags[3]="Family"; taskDueDates[3]="05/01";
  tasks[4]="Meet w devs"; taskTags[4]="Sketch"; taskDueDates[4]="04/30";
  tasks[5]="CS179N Demo"; taskTags[5]="CS179N"; taskDueDates[5]="04/29";
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  pinMode(SW_pin, INPUT);  
  digitalWrite(SW_pin, HIGH);  
  pinMode(tagButton, INPUT);
  organizeByTag=false;
  Serial.begin(9600); //must match baud rate!!!
  currTask=0;
  col='A';
  row=2;
  cellID=col+String(row);
  command="=Sheet1!"+cellID;
  Serial.print(command);
  Serial.print("\n");
  
  lcd.print(currTask+1);
  lcd.print(": "+ tasks[currTask]);
  
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  getCurrentTask();
  
  if(change){
    cellID=col+String(row);
    command="=Sheet1!"+cellID;
    

    Serial.print(command);
    Serial.print("\n");

    lcd.clear();
    lcd.print(currTask+1);
    lcd.print(": "+ tasks[currTask]);
    change=false;
  }
    delay(100);  
}
int incomingByte=0;
void outputStuff(){
  if(Serial.available()>0){
      incomingByte=Serial.read();
      my_str[pos] = incomingByte;
      pos++;
      if(incomingByte==10){
        pos=0;
      
        lcd.print(my_str);
      }
      for(int i=0;i<=len-1;i++){
        my_str[i] = 0;
      }
    }
}
void serialEvent(){
  outputStuff();
}
