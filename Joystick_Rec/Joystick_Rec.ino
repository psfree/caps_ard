#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PAN_PIN 9
#define TILT_PIN 10
#define ZOOM_PIN 6
#define FOCUS_PIN 5

#define MAX_ZOOM 145
#define MIN_ZOOM 30
const int initZoom = 90;

#define MAX_FOCUS 150
#define MIN_FOCUS 30
const int initFocus = 90;

#define MAX_PAN 180
#define MIN_PAN 0
#define MAX_P_ANAL 535
#define MIN_P_ANAL 465
const int initPan = 90;


#define MAX_TILT 180
#define MIN_TILT 30
#define MAX_T_ANAL 535
#define MIN_T_ANAL 465
const int initTilt = 65;

// For Fuzzy Controls
#define MAX_DELAY  80
#define MIN_DELAY 0


unsigned long delayPanTime = 0;
unsigned long delayTiltTime = 0;

// Servo Setup
Servo PanServo;
Servo TiltServo;
Servo ZoomServo;
Servo FocusServo;



//RF24 initiialise
RF24 radio(7, 8); // CE, CSN 9,10 for joystick board  7,8 CE,CSN for breakout PCB
const byte address[6] = "00001";

// Position Array
#define POS_SIZE 9
int pos[POS_SIZE];
int posPan;
int curPosPan = initPan;
int posTilt;
int curPosTilt = initTilt;
int posZoom;
int posZoom2;
int curPosZoom = initZoom;
int posFocus;
int posFocus2;
int curPosFocus = initFocus;
bool toggleReset;
bool toggleMode;


void setup() {
  Serial.begin(9600);

  //Servo Setup
  PanServo.attach(PAN_PIN);
  TiltServo.attach(TILT_PIN);
  ZoomServo.attach(ZOOM_PIN);
  FocusServo.attach(FOCUS_PIN);
  // Set Servo Init Position
  while (curPosPan != initPan || curPosTilt != initTilt || curPosZoom != initZoom || curPosFocus != initFocus) {
    if (curPosPan > initPan) {
      curPosPan--;
    }
    else if (curPosPan < initPan) {
      curPosPan++;
    }

    if (curPosTilt > initTilt) {
      curPosTilt--;
    }
    else if (curPosTilt < initTilt) {
      curPosTilt++;
    }

    if (curPosZoom > initZoom) {
      curPosZoom--;
    }
    else if (curPosZoom < initZoom) {
      curPosZoom++;
    }

    if (curPosFocus > initFocus) {
      curPosFocus--;
    }
    else if (curPosFocus < initFocus) {
      curPosFocus++;
    }
    delay(10);// slows the reset
    PanServo.write(curPosPan);
    TiltServo.write(curPosTilt);
    ZoomServo.write(curPosZoom);
    FocusServo.write(curPosFocus);
  }

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
    posPan = pos[1];//Y axis on JoyStick
    posTilt = pos[0]; //X axis on JoyStick
    posZoom = pos[2]; // Increase Zoom
    posZoom2 = pos[4]; // Decrease Zoom
    posFocus = pos[3]; //Increase Focus
    posFocus2 = pos[5]; // Decrease Focus
    toggleReset = pos[6]; // F button
    toggleMode = pos[7]; // E button

    if (pos[8] != -1) {
      Serial.println("Here");
      return;
    }

    //    Debug logging recieved values
    //Serial.print("Received Values are:");
    //    Serial.print(posTilt);
    //    Serial.print(",");
    //    Serial.print(posPan);
    //    Serial.print(",");
    //    Serial.print(posFocus);
    //    Serial.print(",");
    //    Serial.print(posZoom);
    //    Serial.print(",");
    //    Serial.print(posFocus2);
    //    Serial.print(",");
    //    Serial.println(posZoom2);


    // Debug Current Pos Values
    Serial.print("Current Pos Values are: P=");
    Serial.print(curPosPan);
    Serial.print(",T=");
    Serial.print(curPosTilt);
    Serial.print(",F=");
    Serial.print(curPosFocus);
    Serial.print(",Z=");
    Serial.println(curPosZoom);


    //     Debug Delay Values
    //    Serial.print("Current Delay Values are:");
    //    Serial.print(delayPanTime);
    //    Serial.print(",");
    //    Serial.println(delayTiltTime);

    unsigned long curTime  = millis();
    if ((curTime > delayPanTime) ) {
      if (posPan > MAX_P_ANAL && curPosPan < MAX_PAN) {
        delayPanTime = map (posPan, MAX_P_ANAL, 1023, MAX_DELAY, MIN_DELAY) + millis();
        curPosPan += 1;
      }
      else if (posPan < MIN_P_ANAL && curPosPan > MIN_PAN) {
        delayPanTime = map (posPan, 0, MIN_P_ANAL, MIN_DELAY, MAX_DELAY) + millis();
        curPosPan -= 1;
      }
      else {
        delayPanTime = millis();

      }
      //delay(delayPanTime);
      PanServo.write(curPosPan);
    }

    if ( (curTime > delayTiltTime)) {
      if (posTilt > MAX_T_ANAL && curPosTilt < MAX_TILT) {
        delayTiltTime = map (posTilt, MAX_T_ANAL, 1023, MAX_DELAY, MIN_DELAY) + millis();
        curPosTilt += 1;
      }
      else if (posTilt < MIN_T_ANAL && curPosTilt > MIN_TILT) {
        delayTiltTime = map (posTilt, 0, MIN_T_ANAL, MIN_DELAY, MAX_DELAY) + millis();
        curPosTilt -= 1;
      }
      else {
        delayTiltTime = millis();
      }
      //delay(delayTiltTime);
      TiltServo.write(curPosTilt);
    }

    if (posZoom == 0 && curPosZoom < MAX_ZOOM) {
      curPosZoom++;
    }
    if (posZoom2 == 0 && curPosZoom > MIN_ZOOM) {
      curPosZoom--;
    }

    if (posFocus == 0 && curPosFocus < MAX_FOCUS) {
      curPosFocus++;
    }
    if (posFocus2 == 0 && curPosFocus > MIN_FOCUS) {
      curPosFocus--;
    }
    ZoomServo.write(curPosZoom);
    FocusServo.write(curPosFocus);

    //Reset Button is E
    /* when button E is pressed Mechanism resets to original value
       we go through a while loop that changes the curPos gradually.
       This adds stability to the reset, in case the movement stops
       before a full reset.
    */
    if (toggleReset == 0) {

      while (curPosPan != initPan || curPosTilt != initTilt || curPosZoom != initZoom || curPosFocus != initFocus) {
        if (curPosPan > initPan) {
          curPosPan--;
        }
        else if (curPosPan < initPan) {
          curPosPan++;
        }

        if (curPosTilt > initTilt) {
          curPosTilt--;
        }
        else if (curPosTilt < initTilt) {
          curPosTilt++;
        }

        if (curPosZoom > initZoom) {
          curPosZoom--;
        }
        else if (curPosZoom < initZoom) {
          curPosZoom++;
        }

        if (curPosFocus > initFocus) {
          curPosFocus--;
        }
        else if (curPosFocus < initFocus) {
          curPosFocus++;
        }
        delay(10);// slows the reset
        PanServo.write(curPosPan);
        TiltServo.write(curPosTilt);
        ZoomServo.write(curPosZoom);
        FocusServo.write(curPosFocus);
      }
    }

  }

  else {
    Serial.println("Fail");
    PanServo.write(curPosPan);
    TiltServo.write(curPosTilt);
    ZoomServo.write(curPosZoom);
    FocusServo.write(curPosFocus);
  }
}
