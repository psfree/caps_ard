#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE,CSN on JS Shield 9/10
const byte address[6] = "00001";

//Joystick Inputs
int const x_key = A1;
int const y_key = A0;

// Button Inputs
int const Up_btn = 2; // zoom ++
int const Right_btn = 3; // focus ++
int const Down_btn = 4; // zoom --
int const Left_btn = 5; // focus --
int const E_btn = 6;
int const F_btn = 7;
long up_pos;
long right_pos;
long down_pos;
long left_pos;
long e_pos;
long f_pos;

boolean pressed_Up = false;
boolean pressed_Right = false;
boolean pressed_Down = false;
boolean pressed_Left = false;
boolean pressed_E = false;
boolean pressed_F = false;

boolean stillpress = false;
//facetracking variables
float focus;
long x_pos = 512;
long y_pos = 512;

//facetracking structs
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


long pos[8];

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setRetries(25, 25);
  radio.setPALevel(RF24_PA_MAX); // in ascending order of Power, MIN, LOW, HIGH, MAX
  radio.stopListening(); // sets this as transmitter
  Serial.begin(9600);

  pinMode (x_key, INPUT) ;
  pinMode (y_key, INPUT) ;

  pinMode(Up_btn, INPUT);
  digitalWrite(Up_btn, HIGH);

  pinMode(Right_btn, INPUT);
  digitalWrite(Right_btn, HIGH);

  pinMode(Down_btn, INPUT);
  digitalWrite(Down_btn, HIGH);

  pinMode(Left_btn, INPUT);
  digitalWrite(Left_btn, HIGH);

  pinMode(E_btn, INPUT);
  digitalWrite(E_btn, HIGH);

  pinMode(F_btn, INPUT);
  digitalWrite(F_btn, HIGH);
}
/*
  @@@JASON@@@@

  Please create a way to send all 8 pieces of information over pos array

  + look at https://github.com/psfree/caps_ard for receiver code to do receiving as well

*/

int count = 50;
boolean forward = true;
void loop() {
  x_pos = analogRead (x_key);
  y_pos = analogRead (y_key);
  up_pos = digitalRead (Up_btn);
  right_pos = digitalRead (Right_btn);
  down_pos = digitalRead (Down_btn);
  left_pos = digitalRead (Left_btn);
  e_pos = digitalRead (E_btn);
  f_pos = digitalRead (F_btn);

  if ( (f_pos == 0) && (!stillpress) ) {
    pressed_F = !pressed_F;
    Serial.println("Entering FaceTrack mode");
    stillpress = true;
    delay(10);
  }
  else if (f_pos == 1) stillpress = false;

  if (pressed_F) {
    if (Serial.available() < 7) {
      //radio.write(&pos, sizeof(pos));
      return;
    }
    for (byte i = 0; i < 7; i++) {
      rx_array[i] = Serial.read();
    }
    if (rx_array[6] != '\n') {
      //todo: add timeout
      char c = ' ';
      while (1) {
        if (Serial.available())
          c = Serial.read();
        if (c == '\n') break;
      }
      return;
    }
    for (byte i = 0; i < 7; i++) {
      rcv.dl[i] = rx_array[i];
    }
    char face_y = rcv.ser.c1;
    char face_x = rcv.ser.c2;
    float focus = rcv.ser.control;
    if (face_x == 'r') {
      x_pos = 950;
    }
    else if (face_x == 'l') {
      x_pos = 20;
    }
    else {
      x_pos = 512;
    }

    if (face_y == 'u') {
      y_pos = 950;
    }
    else if (face_y == 'd') {
      y_pos = 20;
    }
    else {
      y_pos = 512;
    }
    pos[1] = x_pos;
    pos[0] = y_pos;
  }
  else {
    pos[0] = x_pos;
    pos[1] = y_pos;
    pos[2] = up_pos;
    pos[3] = right_pos;
    pos[4] = down_pos;
    pos[5] = left_pos;
    pos[6] = e_pos;
    pos[7] = f_pos;
  }
  Serial.println(x_pos);
  Serial.println(y_pos);
  Serial.println(pos[2]);
  Serial.println(right_pos);
  Serial.println(pos[4]);
  Serial.println(left_pos);
  Serial.println(e_pos);
  Serial.println(f_pos);

  radio.write(&pos, sizeof(pos));
  //dio.write(&y_pos, sizeof(y_pos));

}
