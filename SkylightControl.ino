//#include <IRLibRecvPCI.h>
#include "IRLibAll.h" 

const int ledPin = 13;
const int redLed =4;
const int greenLed = 5;

const int stepperEn = 7;
const int stepperDir = 8;
const int stepperStep = 9;

const int endpointLow = 11;
const int endpointHigh = 10;

const int stepsPerRevolution = 200;

int step_delay = 8;

int moving = 0;
int up_or_down = LOW;

IRrecvPCI myReceiver(2);
IRdecode myDecoder;   //create decoder 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  pinMode(stepperEn, OUTPUT);
  pinMode(stepperDir, OUTPUT);
  pinMode(stepperStep, OUTPUT);

  pinMode(endpointLow, INPUT);
  pinMode(endpointHigh, INPUT);
  disableStepper();
  myReceiver.enableIRIn();
  myReceiver.blink13(true);
}

void pulseStepper (int nPulses) {
  
  for (int i=0; i<nPulses; i++) {
    digitalWrite(stepperStep, HIGH);
    delayMicroseconds(20);
    digitalWrite(stepperStep, LOW);
    delay(step_delay);
  }
  
}

void setDirection(bool dir) {
    digitalWrite(stepperDir, dir);    
}

void enableStepper(void) {
  digitalWrite(stepperEn, LOW);
}

void disableStepper(void) {
  digitalWrite(stepperEn, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(endpointLow) == HIGH && up_or_down == LOW) {
    disableStepper();
    moving = 0;
  }

  if (digitalRead(endpointHigh) == HIGH && up_or_down == HIGH) {
    disableStepper();
    moving = 0;
  }
  
  if(myReceiver.getResults()){
    myDecoder.decode();           //Decode it 
    myDecoder.dumpResults(false);  //the results of a Now print results. 

    switch(myDecoder.value) { 
        case 0x77E1D0C9:
        case 0x77E1E0C9:
          /*digitalWrite(redLed, HIGH);
          digitalWrite(greenLed, LOW);*/
          step_delay = 7;
          up_or_down = HIGH;
          setDirection(up_or_down);
          enableStepper();
          moving = 1;
          break; 
        case 0x77E1B0C9:
        case 0x77E110C9:
          digitalWrite(greenLed, HIGH);
          digitalWrite(redLed, LOW);
          step_delay = 3;
          up_or_down = LOW;
          setDirection(up_or_down);
          enableStepper();
          moving = 1;
          break; 
        case 0x77E140C9:
        case 0x77E17AC9:
        case 0x77E120C9:
          disableStepper(); 
          moving = 0;
          break; 
      } 
      
    myReceiver.enableIRIn();      //Restart receiver 
  }

  if (moving) {
    pulseStepper(1);
  }
}
