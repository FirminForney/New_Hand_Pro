
#include "BluetoothSerial.h"
#include "ESP32Servo.h"

// Classe pour la communication Bluetooth
BluetoothSerial ESP_BT; 

// Définition des broches
#define pin1 33  // Moteur 1 - Sens de rotation 1
#define pin2 25  // Moteur 1 - Sens de rotation 2
#define pinP 26  // Moteur 1 - Commande d'allumage
#define pinS 32  // Servo-moteur
#define pinB1 35  // Bouton 1 (ouverture/fermeture automatique)

// Variables d'état
int etatbp = 0;   // État du bouton 1 (0 = non pressé, 1 = pressé)
bool etat = 0;    // État de la main (0 = ouverte, 1 = fermée)

// Déclaration du servo-moteur
Servo servo1;

void setup() {
  Serial.begin(19200);       // Initialisation de la communication série (pour le débogage)
  ESP_BT.begin("New_Hand_Pro");  // Initialisation de la communication Bluetooth 
  pinMode(33, OUTPUT);      // Configuration des broches des moteurs en sortie
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(35, INPUT_PULLUP); // Configuration de la broche du bouton en entrée avec résistance de pull-up
  servo1.attach(32);        // Attache le servo-moteur à la broche 32
}

void loop() {
  blue();               // Lecture des commandes Bluetooth
  delay(100);           // Délai de 100ms
  detect_ouverture();   // Détection de l'ouverture/fermeture automatique
  delay(100);           // Délai de 100ms
}

// Fonction pour ouvrir la main
void ouvrir(){
  digitalWrite(pin1, HIGH);  // Activation du moteur dans un sens
  digitalWrite(pin2, LOW);   // Désactivation du moteur dans l'autre sens
  digitalWrite(pinP, HIGH);  // Activation de la puissance du moteur
  delay(45);               // Délai pour laisser le temps à la main de s'ouvrir
  digitalWrite(pinP, LOW);   // Désactivation de la puissance du moteur 
}

// Fonction pour fermer la main
void fermer(){
  digitalWrite(pin1, LOW);   // Activation du moteur dans un sens
  digitalWrite(pin2, HIGH);  // Désactivation du moteur dans l'autre sens
  digitalWrite(pinP, HIGH);  // Activation de la puissance du moteur 
}

// Fonction pour gérer les commandes Bluetooth
void blue(){
  int value = ESP_BT.read(); // Lecture de la valeur reçue par Bluetooth

  if (value == 10){          // Si la valeur est 10...
    Serial.print(value);     // Affichage de la valeur (debug)
    servo1.write(0);       // Positionnement du servo-moteur à 180 degrés
    ouvrir();                // Ouverture de la main
  }

  if (value == 11){          // Si la valeur est 11...
    Serial.print(value);     // Affichage de la valeur (debug)
    fermer();                // Fermeture de la main
    delay(150);              // Délai 
    servo1.write(90);        // Positionnement du servo-moteur à 90 degrés
  }

  if (value == 20){          // Si la valeur est 20...
    servo1.write(90);        // Positionnement du servo-moteur à 90 degrés
    Serial.print(value);     // Affichage de la valeur (debug)
    ouvrir();                // Ouverture de la main
  }

  if (value == 21){           // Si la valeur est 21...
    servo1.write(90);         // Positionnement du servo-moteur à 90 degrés
    Serial.print(value);      // Affichage de la valeur (debug)
    fermer();                 // Fermeture de la main
    }
}

// Fonction pour détecter l'ouverture/fermeture via le capteur 
void detect_ouverture(){

  etatbp = digitalRead(pinB1);  // Lecture de l'état du bouton
  delay(50);                   // Délai (anti-rebond)

  Serial.println(etatbp);      // Affichage de l'état du bouton (debug)

  if ( etatbp == 1){            // Si le bouton est pressé...

    if(etat == 1){             // Si la main est fermée...
      Serial.println(etatbp);  // Affichage de l'état du bouton (debug)
      delay(2000);             // Délai de 2 secondes
      servo1.write(0);      // Positionnement du servo-moteur
      ouvrir();                // Ouverture de la main 
      etat = 0;                // Changement d'état de la main (ouverte)
      delay(1000);             // Délai d'une seconde
      etatbp = 0;              // Réinitialisation de l'état du bouton
    }
  }

  if (etatbp == 1){            // Si le bouton est pressé...

    if (etat == 0){             // Si la main est ouverte...
      Serial.println(etatbp);  // Affichage de l'état du bouton (debug)
      delay(2000);             // Délai de 2 secondes
      servo1.write(90);        // Positionnement du servo-moteur
      fermer();                // Fermeture de la main
      etat = 1;                // Changement d'état de la main (fermée)
      delay(1000);             // Délai d'une seconde
      etatbp = 0;              // Réinitialisation de l'état du bouton
    }  
  }
}