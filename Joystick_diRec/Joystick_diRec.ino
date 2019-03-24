#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PAN_PIN 9
#define TILT_PIN 10
#define ZOOM_PIN 5
#define FOCUS_PIN 6

#define MAX_ZOOM 135
#define MIN_ZOOM 45

#define MAX_FOCUS 135
#define MIN_FOCUS 45

#define MAX_PAN 180
#define MIN_PAN 0
#define MAX_P_ANAL 535
#define MIN_P_ANAL 465


#define MAX_TILT 180
#define MIN_TILT 55
#define MAX_T_ANAL 535
#define MIN_T_ANAL 465



#define MAX_DELAY  1

// Sets Our Variables
unsigned long delayPanTime = 0;
unsigned long delayTiltTime = 0;

// Servo Setup
Servo PanServo;
Servo TiltServo;
Servo ZoomServo;
Servo FocusServo;

//RF24 initiialise
RF24 radio(7, 8); // CE, CSN 9,10 for joystick board  8,7 CE,CSN for nano Breakout board
const byte address[6] = "00001";

// Position Array
long pos[8];
long posPan;
long curPosPan = 100;
long posTilt;
long curPosTilt = 90;
long posZoom;
long posZoom2;
long curPosZoom = 90;
long posFocus;
long posFocus2;
long curPosFocus = 90;


struct serdata { //7 bytes
  char c1;  //1
  char c2;  //1
  float control;  //4
  char newline; //1
};

union pcin {
  serdata ser;
  byte dl[7];
};

pcin rcv;
byte rx_array[7];

bool isValid(char c) {
  if((c=='c')||(c=='r')||(c=='l')||(c=='d')||(c=='u')) {
    return true;
  }
  return false;
}
long x_pos = 512;
long y_pos = 512;

void setup() {
  Serial.begin(9600);
  Serial.println("Input Desired Mode");
  //Servo Setup
  PanServo.attach(PAN_PIN);
  TiltServo.attach(TILT_PIN);
  ZoomServo.attach(ZOOM_PIN);
  FocusServo.attach(FOCUS_PIN);
  char c = ' ';
  while(1) {
    if(Serial.available())
      c = Serial.read();
      if(c=='\n') break;
  }
}

void loop() {
  if (Serial.available() < 7) {
     // error
     return;
  }
  for(byte i=0; i<7; i++) {
    rx_array[i] = Serial.read();
  }
  if(rx_array[6]!='\n')   return;
  
  for (byte i = 0; i < 7; i++) {
     rcv.dl[i] = rx_array[i];
  }
  
  char face_y = rcv.ser.c1;
  char face_x = rcv.ser.c2;
  float focus = rcv.ser.control;

  char str[3] = {face_x, '\n', 0x00};
  Serial.println(str);

  if(face_x == 'r') {
    x_pos = 950;
  }
  else if(face_x == 'l') {
    x_pos = 20;
  }
  else {
    x_pos = 512;
  }

  if(face_y =='u') {
    y_pos=950;
  }
  else if(face_y == 'd') {
    y_pos=20;
  }
  else {
    y_pos=512;
  }

  posPan = x_pos;
  posTilt = y_pos;


  Serial.print("Current Delay Values are:");
  Serial.print(x_pos);
  Serial.print(",");
  Serial.println(y_pos);

  unsigned long curTime  = millis();
  if ((curTime > delayPanTime) ) {
    if (posPan > MAX_P_ANAL && curPosPan < MAX_PAN){
      delayPanTime = map (posPan, MAX_P_ANAL, 1023, MAX_DELAY, 1) + millis();
      curPosPan += 1;
      curPosZoom +=1;
    }
    else if (posPan < MIN_P_ANAL && curPosPan > MIN_PAN) {
      delayPanTime = map (posPan, 0,MIN_P_ANAL, 1, MAX_DELAY)+ millis();
      curPosPan -= 1;
      curPosZoom -=1;
    }
    else {
      delayPanTime = millis();  
    }
    //delay(delayPanTime);
    PanServo.write(curPosPan);
    ZoomServo.write(curPosZoom);
  }

  if ( (curTime > delayTiltTime)) {
    if (posTilt > MAX_T_ANAL && curPosTilt < MAX_TILT) {
      delayTiltTime = map (posTilt,MAX_T_ANAL, 1023, MAX_DELAY, 1) + millis();
      curPosTilt += 1;
      curPosFocus +=1;
    }
    else if (posTilt < MIN_T_ANAL && curPosTilt > MIN_TILT) {
      delayTiltTime = map (posTilt, 0, MIN_T_ANAL, 1, MAX_DELAY) + millis();
      curPosTilt -= 1;
      curPosFocus -= 1;
    }
    else {
      delayTiltTime = millis();
    }
    TiltServo.write(curPosTilt);
    FocusServo.write(curPosFocus);
  }  
    //delay(delayTiltTime);  
}
