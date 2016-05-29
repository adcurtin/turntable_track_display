#include <avr/pgmspace.h>
#include <Servo.h>
#include "font.h"

Servo servo;
int colTime;
int spaceTime; //continuum

String line1 = "";//                                              ";
String line2 = "";//                                              ";
int inbyte = 0;
int i = 0;

int holdtime;

void printChar(char in){
  int i,j;
  if(in == ' '){
    delay(colTime*5+spaceTime);
    return;
  }
  //char ind = in - 16;//'A';
  for(i=0;i<5;i++){
    for(j=0;j<7;j++){
      digitalWrite(j+2, pgm_read_byte_near(&(font[in][i+j*5])) );
    }
    delay(colTime);
  }
  for(j=0;j<7;j++){
    digitalWrite(j+2, 0);
  }
  delay(spaceTime);
  
}

void track(int t){
  if(t==1){
    servo.write(52);
    colTime = 30;
    spaceTime = 35;
    //7 chars long
  }
  else if(t==2){
    servo.write(66);
    colTime = 15;
    spaceTime = 15;
    //12 chars long
  }
  else if(t==3){
    servo.write(78);
    colTime = 12;
    spaceTime = 12;
    //18 chars long
  }
  delay(250);
}


void setup(){
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  servo.attach(12);  // attaches the servo on pin 12 to the servo object  
  track(2);
  Serial.begin(9600); 
  
}

void loop(){
  if(Serial.available() > 0){ //read data into line1 and line 2
    inbyte = Serial.read();
    //Serial.print(inbyte, BYTE);
    if(inbyte == '@'){
      line1 = "";
      delay(100);
      while(Serial.available() > 0 && inbyte != '#'){ //add @?
        inbyte = Serial.read();
        line1 += byte(inbyte); //empty buffer into line1
      }
      for(int i=0;i<line1.length();i++){
        Serial.print(line1.charAt(i)); 
      }
      if(inbyte=='#') line1 = line1.substring(0,line1.length()-1);
    }
    if(inbyte == '#'){
      line2 = "";
      delay(100);
      while(Serial.available() > 0 && inbyte != '@'){ //add #?
        inbyte = Serial.read();
        line2 += byte(inbyte); //empty buffer into line2
      }
      if(inbyte=='@') line2 = line2.substring(0,line2.length()-1);
    }
  }
  while(Serial.available()==0){ //print line1 and line2 until we get new data
    //holdtime = millis();
    //holdtime += 5000;
    //add delay before switching tracks
    //if (length < [number]) delay(200*number - length)
    track(3);
    for(i=0;i<line1.length();i++){
      printChar(line1.charAt(i));
    }
    if(line1.length() < 14) delay(200*(14-line1.length()));
    if(line1 != "Paused") track(2); //don't switch tracks if paused.
    else delay(2000);
    for(i=0;i<line2.length();i++){
      printChar(line2.charAt(i));
    }
    if(line2.length() < 10) delay(200*(10-line2.length()));
    delay(2000);
    
    /*while(millis() < holdtime){
      //just wait
    };*/
  }
}

