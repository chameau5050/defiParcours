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
#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <SharpIR.h>
#include <move.h>
#include <capteur.h>
#include <stdio.h>
#include <math.h>
#include <ADJDS311.h>
#include <PID.h>
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces
int target=1;
void robotA();
void robotB();
void suivre();

int bleu=1, jaune=2, rouge=3, vert=4;
int couleuri=bleu;//ce que tu changes dans le main pour que le défi fonctionne
int couleurf=bleu;//ce que tu changes dans le main pour que le défi fonctionne
int checkline(){
  int gauche_3=analogRead(A7), gauche_2=analogRead(A6), gauche_1=analogRead(A5), gauche_0=analogRead(A4);
  int droite_3=analogRead(A0), droite_2=analogRead(A1), droite_1=analogRead(A2), droite_0=analogRead(A3);
  if(gauche_0<500 || droite_0<500){
    return 0;
  }
  else {
    MOTOR_SetSpeed(0,0.2);
}
}
void robotA(int couleuri){
  if (ROBUS_IsBumper(3)==1){
    SERVO_Enable(0);
    SERVO_SetAngle(0,70);
    if (couleuri==1){
     turnAngle(90);
     avancePID(35);
     turnAngle(45);
    }
    if (couleuri==2){
     turnAngle(-90);
    }
    if (couleuri==3){
      turnAngle(-45);
      avancePID(50);
      turnAngle(45);
    }
    if (couleuri==4){
      turnAngle(45);
      avancePID(50);
      turnAngle(-45);
    }
    suivre();
    avancePID(32);
    SERVO_SetAngle(0,135);
    delay(1000);
    SERVO_SetAngle(0,115);
    MOTOR_SetSpeed(0,-.1);
    MOTOR_SetSpeed(0,-.1);
    delay(100);
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(0,0);
    turnAngle(180);
    avancePID(100);
    //suivre();
    //A AJUSTER
    SERVO_SetAngle(0,70);
    MOTOR_SetSpeed(0,-.1);
    MOTOR_SetSpeed(0,-.1);
    delay(1000);
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(0,0);

}
}
void robotB(int couleurf)
{
  scan(120);
  if (couleurf==1)
  {
    avancePID(30);
    turnAngle(-90);
  }
  if (couleurf==2)
  {
    avancePID(30);
    turnAngle(90);
  }
  if (couleurf==3)
  {
    avancePID(-30);
    turnAngle(90);
  }
  if (couleurf==4)
  {
    avancePID(-30);
    turnAngle(-90);
  }
  suivre(); 
  //a modifier
}

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void suivre()
{
  setup_couleur();
  Serial.println("rentre capteur");
  PID_capteur();
  //Serial.println("rentre decel");
  //Deceleration();
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

void loop() {
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU
  robotA(couleuri);
  //suivre();
}