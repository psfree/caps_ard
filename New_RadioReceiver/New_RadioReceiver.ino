#define PanPin 3
#define TiltPin 9
#define Steps 180


// Sets Our Variables
//int PosTilt = 0;
//int PosPan = 0;
int long val = 0;
int oldPan = 0;
int oldTilt = 0;
unsigned int y;


#include <Servo.h>
// Servo Setup
Servo PanServo;
Servo TiltServo; 


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//RF24 initiialise
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Position Array
long posPan;
long curPosPan = 90;
long posTilt;
long curPosTilt = 110;
long pos[2];

void setup() {
  Serial.begin(9600);

  //Servo Setup
  PanServo.attach(PanPin);
  TiltServo.attach(TiltPin);

  //Initialises Radio Stuff
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening(); // sets this as a receiver


//  while (!Serial) {}
}


void loop() {

  //GetPos();
  
    if (radio.available()) {

    radio.read(&pos, sizeof(pos));
    posPan = pos[1];
    posTilt = pos[0];
    Serial.print(posPan);
    Serial.print(",");
    Serial.println(posTilt);

    posPan = map (posPan, 0, 1023, 0, Steps);
    posTilt = map (posTilt, 0, 1023, 0, Steps);
    
    if (posPan > 100 && curPosPan < 180){
      curPosPan += 1;
    }
    if (posPan < 80 && curPosPan > 0){
      curPosPan -= 1;
    }
    if (posPan > 85 && posPan < 95){
      
    }
    else{
      PanServo.write(curPosPan);
    }

    if (posTilt > 100 && curPosTilt < 180){
      curPosTilt += 1;
    }
    if (posTilt < 80 && curPosTilt > 0){
      curPosTilt -= 1;
    }
    if (posTilt > 85 && posTilt < 95){
      
    }
    else{
      TiltServo.write(curPosTilt);
    }
 
   }
     else Serial.println("Fail");
 }


void GetPos() {
  if (radio.available()) {

    radio.read(&pos, sizeof(pos));
    posPan = pos[0];
    posTilt = pos[1];
    Serial.print(posPan);
    Serial.print(",");
    Serial.println(posTilt);

    posPan = map (posPan, 0, 1023, 0, Steps);
    
    //&& curPosPan < 180
    if (posPan > 90 ){
      curPosPan += 5;
    }
    
    //&& curPosPan > 0
    if (posPan < 90 ){
      curPosPan -= 5;
    }

    //if (posPan == 90){
      
    //}
    //else{
      PanServo.write(posPan);
    //}
    

  }
  else Serial.println("Fail");

}
