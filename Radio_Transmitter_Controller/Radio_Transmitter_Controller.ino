#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE,CSN
const byte address[6] = "00001";

int x_key = A5;
int y_key = A0;
long x_pos;
long y_pos;
long pos[2];

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX); // in ascending order of Power, MIN, LOW, HIGH, MAX
  radio.stopListening(); // sets this as trasmitter
  Serial.begin(9600);
  pinMode (x_key, INPUT) ;
  pinMode (y_key, INPUT) ;
}

void loop() {
  x_pos = analogRead (x_key) ;
  y_pos = analogRead (y_key) ;
  pos[0] = x_pos;
  pos[1] = y_pos;
  Serial.println(x_pos);
  Serial.println(y_pos);
  radio.write(&pos, sizeof(pos));
  //dio.write(&y_pos, sizeof(y_pos));
 }
