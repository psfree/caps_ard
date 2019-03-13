#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PAN_PIN 9
#define TILT_PIN 10
#define ZOOM_PIN 5
#define FOCUS_PIN 6

#define STEPS 180
#define MAX_DELAY 10

// Sets Our Variables
unsigned long delayPanTime = 0;
unsigned long delayTiltTime = 0;

// Servo Setup
Servo PanServo;
Servo TiltServo;
Servo ZoomServo;
Servo FocusServo;

//RF24 initiialise
RF24 radio(7, 8); // CE, CSN 9,10 for joystick board
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


void setup() {
  Serial.begin(9600);

  //Servo Setup
  PanServo.attach(PAN_PIN);
  TiltServo.attach(TILT_PIN);
  ZoomServo.attach(ZOOM_PIN);
  FocusServo.attach(FOCUS_PIN);

  //Initialises Radio Stuff
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH); // in ascending order of Power, MIN, LOW, HIGH, MAX
  radio.startListening(); // sets this as a receiver

}

void loop() {
  if (radio.available()) {
    //Read the positions from the radio
    radio.read(&pos, sizeof(pos));
    posPan = pos[1];
    posTilt = pos[0];
    posZoom = pos[2];
    posZoom2 = pos[4];
    posFocus = pos[3];
    posFocus2 = pos[5];

    //Debug logging recieved values
//            Serial.print("Received Values are:");
//            Serial.print(posTilt);
//            Serial.print(",");
//            Serial.print(posPan);
//            Serial.print(",");
//            Serial.print(posFocus);
//            Serial.print(",");
//            Serial.print(posZoom);
//            Serial.print(",");
//            Serial.print(posFocus2);
//            Serial.print(",");
//            Serial.println(posZoom2);
//     Debug Current Pos Values
    Serial.print("Current Pos Values are:");
    Serial.print(curPosPan);
    Serial.print(",");
    Serial.print(curPosTilt);
    Serial.print(",");
    Serial.print(curPosFocus);
    Serial.print(",");
    Serial.println(curPosZoom);

    unsigned long curTime  = millis();
    if ((curTime > delayPanTime) ) {
      if (posPan > 534 && curPosPan < 180) {
        delayPanTime = map (posPan, 534, 1023, MAX_DELAY, 1) + millis();
        curPosPan += 1;
      }
      else if (posPan < 460 && curPosPan > 0) {
        delayPanTime = map (posPan, 0, 460, 1, MAX_DELAY);
        curPosPan -= 1;
      }
      else {
        delayPanTime = 0;
      }
      //delay(delayPanTime);
      PanServo.write(curPosPan);
    }

    if ( (curTime > delayTiltTime)) {
      if (posTilt > 534 && curPosTilt < 180) {
        delayTiltTime = map (posTilt, 534, 1023, MAX_DELAY, 1) + millis();
        curPosTilt += 1;
      }
      else if (posTilt < 460 && curPosTilt > 40) {
        delayTiltTime = map (posTilt, 0, 460, 1, MAX_DELAY) + millis();
        curPosTilt -= 1;
      }
      else {
        delayTiltTime = 0;
      }
      //delay(delayTiltTime);
      TiltServo.write(curPosTilt);
    }

    if (posZoom == 0 && curPosZoom < 135) {
      curPosZoom++;
    }
    if (posZoom2 == 0 && curPosZoom > 45) {
      curPosZoom--;
    }

    if (posFocus == 0 && curPosFocus < 135) {
      curPosFocus++;
    }
    if (posFocus2 == 0 && curPosFocus > 45) {
      curPosFocus--;
    }
    ZoomServo.write(curPosZoom);
    FocusServo.write(curPosFocus);

  }

  else {
    Serial.println("Fail");
  }
}
