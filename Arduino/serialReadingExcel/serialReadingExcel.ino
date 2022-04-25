const int len = 60;
char my_str[len];
char pos = 0;

int incomingByte = 0;//for incoming serial data
void setup() {
  Serial.begin(9600);//baud rate
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
void outputStuff(){
  if(Serial.available()>0){
    incomingByte = Serial.read();
    my_str[pos] = incomingByte;
    pos++;

    if(incomingByte==10){
      pos=0;
    
      if(my_str[0] =='t'){
        Serial.print("t,HIGH,");
        digitalWrite(13,HIGH);
      }
      else{
        digitalWrite(13,LOW);
        Serial.print("not t,LOW,");
      }
      Serial.print(my_str);
      for(int i=0;i<=len-1;i++){
        my_str[i] = 0;
      }
    }
  }
}
void serialEvent(){

  outputStuff();
}
