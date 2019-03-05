#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE,CSN
const byte address[6] = "00001";

long pos[8];

int x_key = A1;
int y_key = A0;
long x_pos;
long y_pos;

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

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX); // in ascending order of Power, MIN, LOW, HIGH, MAX
  radio.stopListening(); // sets this as trasmitter
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

void loop() {
  x_pos = analogRead (x_key) ;
  y_pos = analogRead (y_key) ;
  up_pos = digitalRead (Up_btn);
  right_pos = digitalRead (Right_btn);
  down_pos = digitalRead (Down_btn);
  left_pos = digitalRead (Left_btn);
  e_pos = digitalRead (E_btn);
  f_pos = digitalRead (F_btn);
  
  pos[0] = x_pos;
  pos[1] = y_pos;
  pos[2] = up_pos;
  pos[3] = right_pos;
  pos[4] = down_pos;
  pos[5] = left_pos;
  pos[6] = e_pos;
  pos[7] = f_pos;
  
  Serial.println(x_pos);
  Serial.println(y_pos);
  Serial.println(up_pos);
  Serial.println(right_pos);
  Serial.println(down_pos);
  Serial.println(left_pos);
  Serial.println(e_pos);
  Serial.println(f_pos);
  
  radio.write(&pos, sizeof(pos));
  //dio.write(&y_pos, sizeof(y_pos));
 }
