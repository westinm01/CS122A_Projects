#include <LiquidCrystal.h>
const int SW_pin=2;
const int X_pin=0;
const int Y_pin=1;
const int Toggle_pin=52;
const int Delete_pin=50;
const int SortDueDate_pin=48;
const int SortTag_pin=46;

enum toggleStates{tagSet, tagSetWait, dateSet, dateSetWait};
enum toggleStates toggleState=dateSet;
enum joystickStates{up,down,neutral, still};
enum joystickStates joystickState=still;
int currTask;
const int taskNum=20;

String tasks[taskNum];
String taskTags[taskNum];
String taskDueDates[taskNum]; //format: MM/DD
int today;

//light info
int red_light_pin= 10;
int green_light_pin = 9;
int blue_light_pin = 8;

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
      if(currTask<taskNum-1){
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

void swap(String arr[],int index, int index2){
  String temp=arr[index];
  arr[index]=arr[index2];
  arr[index2]=temp;
}

void moveEmptyStrings(){
  int border=0;
  for(int i=0;i<taskNum-1;i++){
    if(taskDueDates[i]!=""){
      break;
    }
    border++;
  }
  //now have number of empty strings.
  for(int i=0;i<taskNum-border;i++){
    swap(taskTags,i,border+i);
    swap(tasks,i,border+i);
    swap(taskDueDates,i,border+i);
  }
  
}

//for bubble sort, I based the code from this video: https://www.youtube.com/watch?v=UNaiGgfHgzA&ab_channel=tsbrownie
void sortByTag(){
  for(int i=0;i<taskNum-1;i++){
    for(int j=0; j<taskNum-1;j++){
      if(taskTags[j]>taskTags[j+1]){
        swap(taskTags,j,j+1);
        swap(tasks,j,j+1);
        swap(taskDueDates,j,j+1);
      }
    }
  }
  moveEmptyStrings();
}

void sortByDueDate(){
  
  for(int i=0;i<taskNum-1;i++){
    for(int j=0; j<taskNum-1;j++){
      if(taskDueDates[j]>taskDueDates[j+1]){
        swap(taskTags,j,j+1);
        swap(tasks,j,j+1);
        swap(taskDueDates,j,j+1);
      }
    }
  }
  moveEmptyStrings();
}

void toggleCheck(){
  switch(toggleState){
    case tagSet:
      if(digitalRead(Toggle_pin)==HIGH){
        toggleState=tagSetWait;
      }
      break;
    case tagSetWait:
      if(digitalRead(Toggle_pin)==LOW){
        toggleState=dateSet;
      }
      break;
    case dateSet:
      if(digitalRead(Toggle_pin)==HIGH){
        toggleState=dateSetWait;
      }
      break;
    case dateSetWait:
      if(digitalRead(Toggle_pin)==LOW){
        toggleState=tagSet;
      }
  }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

// include the library code:

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



void checkDueDate()
{
  if(taskDueDates[currTask]==""){
    RGB_color(0,0,0);
    return;
  }
  int dayInt= (taskDueDates[currTask][3] - 48) * 10 + taskDueDates[currTask][4] - 48;
  Serial.print("Due Date diff: " + String(dayInt - today));
  Serial.print("\n");
  if(dayInt == today)
  {
    RGB_color(255,0,0);
  }
  else if(abs(dayInt - today) < 4 || (dayInt < 2 && today > 28))
  {
    RGB_color(255,255,0);
  }
  else
  {
    RGB_color(0,255,0);
  }
}

void deleteTask(){
  tasks[currTask]="";
  taskTags[currTask]="";
  taskDueDates[currTask]="";
}
void setup() {
  // set up the LCD's number of columns and rows:
  today=25;
  
  tasks[0]="Lab"; taskTags[0]="CS122A"; taskDueDates[0]="04/29";
  tasks[1]="Call Claire"; taskTags[1]="CS175"; taskDueDates[1]="04/28";
  tasks[2]="Project 1"; taskTags[2]="CS122A"; taskDueDates[2]="04/27";
  tasks[3]="Call Max"; taskTags[3]="Family"; taskDueDates[3]="05/01";
  tasks[4]="Meet w devs"; taskTags[4]="Sketch"; taskDueDates[4]="04/30";
  tasks[5]="Demo"; taskTags[5]="CS179N"; taskDueDates[5]="04/29";
  tasks[6]="Lab 1-3 Demo"; taskTags[6]="CS110"; taskDueDates[6]="04/25";
  tasks[7]="Meeting"; taskTags[7]="RHA"; taskDueDates[7]="04/25"; 
  
  lcd.begin(16, 2);

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  //Print a message to the LCD.
  
  
  pinMode(SW_pin, INPUT);  
  digitalWrite(SW_pin, HIGH);  
  
  pinMode(Toggle_pin,INPUT);
  pinMode(Delete_pin,INPUT);
  pinMode(SortDueDate_pin,INPUT);
  pinMode(SortTag_pin,INPUT);
  //digitalWrite(Toggle_pin, HIGH);
  
  Serial.begin(9600); //must match baud rate!!!
  currTask=0;
  
  lcd.print(currTask+1 +": "+tasks[currTask]);
  
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);

    getCurrentTask();
    toggleCheck();
    lcd.clear();
    lcd.print(currTask+1);
    lcd.print(": "+tasks[currTask]);
    lcd.setCursor(0,1);
    if(toggleState==dateSet){
      lcd.print(taskDueDates[currTask]);
    }
    if(toggleState==tagSet){
      lcd.print(taskTags[currTask]);
    }
    //RGB_color(taskLights[currTask*2],taskLights[currTask*2+1],0);
    checkDueDate();
    
    Serial.print(currTask+1);
    Serial.print(": "+tasks[currTask]+"\n");
    
    if(digitalRead(Delete_pin)==HIGH){
      deleteTask();
    }
    if(digitalRead(SortDueDate_pin)==HIGH){
      sortByDueDate();
    }
    if(digitalRead(SortTag_pin)==HIGH){
      sortByTag();
    }
    delay(100);  
}
