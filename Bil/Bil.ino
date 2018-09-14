#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

//link adresse å lytte til
//denne må være unik for alle i området.
//denne må være lik på både sender og mottaker.
const byte address[6] = "01010"; //-------------------------------------   <- read me!!!

//pins
// motor one
int enA = 3;
int in1 = 4;
int in2 = 5;

// motor two
int enB = 6;
int in3 = 7;
int in4 = 8;

void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

// 50 er ingen fart noen retning. 100 fremover, 0 bakover

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.println(text);
    
    String inn[3];
    int innTeller = 0;

    for (int i =0; i<sizeof(text); i++){
      if (text[i] == ','){
        innTeller++;
      } else {
        inn[innTeller] += text[i]; 
      }
    }

    int hoyre = inn[0].toInt();
    int venstre = inn[1].toInt();
    
    if (hoyre < 49){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      Serial.println("hoyre bakover");
      analogWrite(enA, map(hoyre, 50, 0, 0, 255));
    } else if (hoyre > 51){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      Serial.println("hoyre fremover");
      analogWrite(enA, map(hoyre, 50, 100, 0, 255));
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }

    if (venstre < 49){
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      Serial.println("venstre bakover");
      analogWrite(enB, map(venstre, 50, 0, 0, 215));
    } else if (venstre > 51){
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      Serial.println("venstre fremover");
      analogWrite(enB, map(venstre, 50, 100, 0, 215));
    } else {
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
  }
}
