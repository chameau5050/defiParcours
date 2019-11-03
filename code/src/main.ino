/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <stdio.h>
#include <arduino.h>
#include "PID.h"

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces



/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void suivre()
{
  PID_capteur();
}

void test()
{
  int gauche_3=analogRead(A7), gauche_2=analogRead(A6), gauche_1=analogRead(A5), gauche_0=analogRead(A4);
  int droite_3=analogRead(A0), droite_2=analogRead(A1), droite_1=analogRead(A2), droite_0=analogRead(A3);

  Serial.print("gauche_3: ");
  Serial.println(gauche_3);
  Serial.print("gauche_2: ");
  Serial.println(gauche_2);
  Serial.print("gauche_1: ");
  Serial.println(gauche_1);
  Serial.print("gauche_0: ");
  Serial.println(gauche_0);
  Serial.print("droite_0: ");
  Serial.println(droite_0);
  Serial.print("droite_1: ");
  Serial.println(droite_1);
  Serial.print("droite_2: ");
  Serial.println(droite_2);
  Serial.print("droite_3: ");
  Serial.println(droite_3);
  Serial.print("Erreur: ");
  Serial.println((gauche_2*5+gauche_1*2+gauche_0)-(droite_0+droite_1*2+droite_2*5));
}


/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  const int BUMPER_ARRIERE=3;

  if(ROBUS_IsBumper(BUMPER_ARRIERE)==1)
  {
    delay(1000);
    suivre();
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
    delay(1000);
  }

//test();

delay(10);// Delais pour décharger le CPU
}