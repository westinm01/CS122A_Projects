
#include <LiquidCrystal.h>
const int SW_pin=2;
const int X_pin=0;
const int Y_pin=1;
enum joystickStates{up,down,neutral, still};
enum joystickStates joystickState=still;
const int len = 10;
String taskNames[len];
String incomingData;
int currTask;
int newTask;

char col;
int row;

//getCurrentTask() changes currentTask in response to joystick movement.
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
        row+=1;
      }
     break;
     case down:
      if(currTask>0){
        currTask-=1;
        row-=1;
      }
     break;
     default:
     break;
     
  }
  
}

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
   pinMode(SW_pin, INPUT);  
  digitalWrite(SW_pin, HIGH);  
  Serial.begin(9600); //must match baud rate!!!
  currTask=0;
  newTask=0;
  col='A';
  row=2;
  //lcd.print(currTask+1 +": "+taskNames[currTask]);
  
}



void serialEvent(){
  if(Serial.available()>0){
    incomingData = Serial.read();
    taskNames[newTask]=incomingData;
    newTask++;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  String cellID=col +String(row);
  lcd.setCursor(0, 0);
  String command = "=Sheet1!" + cellID;
  Serial.print(command);
  Serial.print(",");
  getCurrentTask();
  lcd.clear();
  lcd.print(currTask+1);
  lcd.print(": "+taskNames[currTask]);
  Serial.print(currTask+1);
  Serial.print(","+taskNames[currTask]+",");
    
    delay(100);  
}
