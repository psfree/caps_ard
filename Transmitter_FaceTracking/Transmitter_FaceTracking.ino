#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE,CSN
const byte address[6] = "00001";

float focus;
int x_key = A5;
int y_key = A0;
long x_pos = 512;
long y_pos = 512;
long pos[8];

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

  char c = ' ';
  while(1) {
    if(Serial.available())
      c = Serial.read();
      if(c=='\n') break;
  }

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
  if (Serial.available() < 7) {
     // error
     radio.write(&pos, sizeof(pos));
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
  char str[3] = {face_y, '\n', 0x00};
  Serial.println(str);


  if(face_x == 'l') {
    x_pos = 950;
  }
  else if(face_x == 'r') {
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
  for(int i =0; i<8; i++) {
    pos[i]=1;
  }
  pos[1] = x_pos;
  pos[0] = y_pos;
  for(int i =0; i<8; i++) {
    Serial.println(pos[i]);
  }
  radio.write(&pos, sizeof(pos));
}
