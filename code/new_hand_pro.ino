
#include "BluetoothSerial.h" 
#include "ESP32Servo.h"
// init Class:
BluetoothSerial ESP_BT; 

// init PINs: assign any pin on ESP32
#define pin1 33
#define pin2 25
#define pinP 26
#define pinS 32
#define pinB1 35
#define pinB2 34

int etatbp = 0;
bool etat = 0; 




Servo servo1;

void setup() {
  Serial.begin(19200);
  ESP_BT.begin("New_Hand_Pro"); //Name of your Bluetooth interface -> will show up on your phone
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(35, INPUT_PULLUP);
  servo1.attach(32);
  
  
}

void loop() {
blue();
delay(100);
detect_ouverture();
delay(100);
}

void ouvrir(){
digitalWrite(pin1, HIGH);
digitalWrite(pin2, LOW);
digitalWrite(pinP, HIGH);
delay(45);
digitalWrite(pinP, LOW);
}

void fermer(){
digitalWrite(pin1, LOW);
digitalWrite(pin2, HIGH);
digitalWrite(pinP, HIGH);
}

void blue(){
int value = ESP_BT.read(); //Read what we receive 
    
  if (value == 10){
    Serial.print(value);
    servo1.write(180);
    ouvrir();
  }

  if (value == 11){  
    Serial.print(value);
    fermer();
    delay(150);
    servo1.write(90); 
  }
    
  if (value == 20){ 
    servo1.write(90);
    Serial.print(value);
    ouvrir(); 
  }

  if (value == 21){ 
    servo1.write(90); 
    Serial.print(value);
    fermer();  
    }
}

void detect_ouverture(){

 etatbp = digitalRead(pinB1);
 delay(50);
 Serial.println(etatbp);
if ( etatbp == 1){
 
  if(etat == 1){
    Serial.println(etatbp);
    delay(2000);
    servo1.write(180);
    ouvrir();
    etat = 0;
    delay(1000);
    etatbp = 0;
   }
}

if (etatbp == 1){

  if (etat == 0){
    Serial.println(etatbp);
    delay(2000);
    servo1.write(90);
    fermer();
    etat = 1;
    delay(1000);
    etatbp = 0;
  } 
 }
}
