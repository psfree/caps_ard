#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PAN_PIN 3
#define TILT_PIN 4
//#define ZOOM_PIN 5
//#define FOCUS_PIN 6

#define STEPS 180
#define MAX_DELAY 50

// Sets Our Variables
unsigned long delayPanTime = 0;
unsigned long delayTiltTime = 0;

// Servo Setup
Servo PanServo;
Servo TiltServo;
//Servo ZoomServo;
//Servo FocusServo;

//RF24 initiialise
RF24 radio(9,10); // CE, CSN
const byte address[6] = "00001";

// Position Array
long posPan;
long curPosPan = 90;
long posTilt;
long curPosTilt = 110;
//long posZoom;
//long posZoom2;
//long curPosZoom = 90;
//long posFocus;
//long posFocus2;
//long curPosFocus = 90;
long pos[2];

void setup() {
  Serial.begin(9600);

  //Servo Setup
  PanServo.attach(PAN_PIN);
  TiltServo.attach(TILT_PIN);
//  ZoomServo.attach(ZOOM_PIN);
//  FocusServo.attach(FOCUS_PIN);

  //Initialises Radio Stuff
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening(); // sets this as a receiver

}

void loop() {
  if (radio.available()) {
    //Read the positions from the radio
    radio.read(&pos, sizeof(pos));
    posPan = pos[1];
    posTilt = pos[0];
//    posZoom = pos[2];
//    posZoom2 = pos[4];
//    posFocus = pos[3];
//    posFocus2 = pos[5];


    //Debug logging recieved values
//    Serial.print(curPosPan);
//    Serial.print(",");
//    Serial.println(curPosTilt);

    //
    unsigned long curTime  = millis();
    if ((curTime > delayPanTime) ) {
      if (posPan > 545 && curPosPan < 180) {
        delayPanTime = map (posPan, 545, 1023, MAX_DELAY, 1) + millis();
        curPosPan++;
      }
      else if (posPan < 500 && curPosPan > 0) {
        delayPanTime = map (posPan, 0, 500, 1, MAX_DELAY);
        curPosPan--;
      }
      else {
        delayPanTime = 0;
      }
      PanServo.write(curPosPan);
    }

    if ( (curTime > delayTiltTime)) {
      if (posTilt > 545 && curPosTilt < 180) {
        delayTiltTime = map (posTilt, 545, 1023, MAX_DELAY, 1) + millis();
        curPosTilt += 1;
      }
      else if (posTilt < 500 && curPosTilt > 0) {
        delayTiltTime = map (posTilt, 0, 500, 1, MAX_DELAY) + millis();
        curPosTilt -= 1;
      }
      else {
        delayTiltTime = 0;
      }
      TiltServo.write(curPosTilt);
    }

//    if (posZoom == 1){
//      curPosZoom++;
//    }
//    if (posZoom2 == 1){
//      curPosZoom--;
//    }
//    
//    if (posFocus == 1){
//      curPosFocus++;
//    }
//    if (posFocus2 == 1){
//      curPosFocus--;
//    }
//    ZoomServo.write(curPosZoom);
//    FocusServo.write(curPosFocus);

  }
}
