#include<SoftwareSerial.h> //library for serial communication
int led=12;

void setup() {
  // put your setup code here, to run once:
  pinMode(12,OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Input 1 to Turn LED on and 0 to off");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    int state=Serial.parseInt();

    if(state==1){
      digitalWrite(12,HIGH);
      Serial.println("Command received:1, LED turned ON");
      
    }
    if(state==0){
      digitalWrite(12,LOW);
      Serial.println("Command received: 0 LED turned OFF");
    }
  }
}
