#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//pins
const int hoyre_pin = 3;
const int venstre_pin = 2;
const int forover_pin = 7;
const int bakover_pin = 5;

RF24 radio(9, 10); // CE, CSN

//link adresse å lytte til
//denne må være unik for alle i området.
//denne må være lik på både sender og mottaker.
const byte address[6] = "01010";

void setup() {
  Serial.begin(9600);
  Serial.println("Startup");
  
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  char hoyre[3]; 
  char venstre[3];

  h=!digitalRead(hoyre_pin);
  v=!digitalRead(venstre_pin);
  f=!digitalRead(forover_pin);
  b=!digitalRead(bakover_pin);

  if(h && !v && !f && !b){ //0 grader --- hoyre
    hoyre = 0;
    venstre = 100;
  } else if(h && !v && f && !b){ //45 grader
    hoyre = 75;
    venstre = 100;
  } else if(!h && !v && f && !b){ //90 grader --- fremmover
    hoyre = 100;
    venstre = 100;
  } else if(!h && v && f && !b){ //135 grader
    hoyre = 100;
    venstre = 75;
  } else if(!h && v && !f && !b){ //180 grader --- venstre
    hoyre = 100;
    venstre = 0;
  } else if(!h && v && !f && b){ //225 grader
    hoyre = 0;
    venstre = 25;
  } else if(!h && !v && !f && b){ //270 grader bakover
    hoyre = 0;
    venstre = 0;
  } else if(h && !v && !f && b){ //315 grader
    hoyre = 25;
    venstre = 0;
  }

  char test[32];
  strcpy(test, hoyre);
  strcat(test, ",");
  strcat(test, venstre);

  Serial.println(test);
  radio.write(&test, sizeof(test));
  delay(100);
}
