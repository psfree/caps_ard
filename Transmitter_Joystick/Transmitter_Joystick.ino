#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE,CSN
const byte address[6] = "00001";

char mode = 'j';
char faceInput = 'c';
int x_key = A5;
int y_key = A0;
long x_pos = 512;
long y_pos;
long pos[2];

void setup() {
  Serial.begin(9600);
  Serial.println("Input Desired Mode");
  Serial.println("j = Joystick Mode ...... f = Facial Recogntion Mode");
//   while (!Serial.available()) {
//    // wait for input from user
//  };
//
//  if (Serial.available() > 0) {
//    mode = Serial.read();
//  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX); // in ascending order of Power, MIN, LOW, HIGH, MAX
  radio.stopListening(); // sets this as trasmitter

  pinMode (x_key, INPUT) ;
  pinMode (y_key, INPUT) ;


}

void loop() {
  if (mode == 'j') {
    x_pos = analogRead (x_key) ;
    y_pos = analogRead (y_key) ;
    pos[0] = x_pos;
    pos[1] = y_pos;
    Serial.println(x_pos);
    Serial.println(y_pos);
    radio.write(&pos, sizeof(pos));
    //radio.write(&y_pos, sizeof(y_pos));
  }
  if (mode == 'f') {
    faceInput = Serial.read();
    if (faceInput == 'c') {
      x_pos = 512;
    }
    if (faceInput == 'r') {
      x_pos = 1024;
    }

    if (faceInput == 'l') {
      x_pos = 0;
    }
    Serial.println(x_pos);
    y_pos = 512;
    radio.write(&pos, sizeof(pos));
    
  }
  

}
