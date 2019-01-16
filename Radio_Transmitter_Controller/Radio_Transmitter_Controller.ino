#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE,CSN
const byte address[6] = "00001";

String input = String(10);
int count = 0;

char mode = 'n';
char faceInput = 'c';
char faceInput2 = 'c';
int x_key = A5;
int y_key = A0;
long x_pos = 512;
long y_pos;
long pos[2];

void setup() {
  Serial.begin(9600);
  Serial.println("Input Desired Mode");
  Serial.println("j = Joystick Mode ...... f = Facial Recogntion Mode");
  

  radio.begin();
  radio.openWritingPipe(address);
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX); // in ascending order of Power, MIN, LOW, HIGH, MAX
  radio.stopListening(); // sets this as trasmitter

  pinMode (x_key, INPUT) ;
  pinMode (y_key, INPUT) ;


}

void loop() {
//  if (mode == 'j') {
//    x_pos = analogRead (x_key) ;
//    y_pos = analogRead (y_key) ;
//    pos[0] = x_pos;
//    pos[1] = y_pos;
//    Serial.println(x_pos);
//    Serial.println(y_pos);
//    radio.write(&pos, sizeof(pos));
//    //radio.write(&y_pos, sizeof(y_pos));
//  }
    char c;
    if(Serial.available()>0) {
      c = Serial.read(); 
      input[count] = c;
      count++;
    }
    if(c=='\n') {
      faceInput2 = input[0];
      faceInput = input[1];
      count = 0;
      c = ' ';
      Serial.println(input);
      input = String(10);
      x_pos = 512;
    if (faceInput == 'r') {
      x_pos = 950;
    }

    if (faceInput == 'l') {
      x_pos = 20;
    }
    
    y_pos = 512;
    if (faceInput2 == 'u') {
      y_pos = 950;
    }

    if (faceInput2 == 'd') {
      y_pos = 20;
    }
    pos[0] = x_pos;
    pos[1] = y_pos;
    radio.write(&pos, sizeof(pos));
    delay(50);
    }
    
    
  

}
