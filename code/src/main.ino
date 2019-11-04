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
#include "servomoteur.h"
#define model 1080
#define ir1 1
#define ir2 2
SharpIR SharpIR1(ir1, model);
SharpIR SharpIR2(ir2, model);
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces
double cirRoue = 24.35;
double rayonRot = 9.75;
double cirRot = 61.261;


/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void turn(int angle){ //en degres
  const int cycle = 100;
  const float KP = 0.0001;
  const float KI = 0.00002;
  const int pulse1 = 21 ;
  const float erreur = 3;
  float tour = angle*pulse1;
  
  if(tour<0)
    tour*=-1;
  Serial.println("tour");
  Serial.println(tour);
  float vitesseDroite=-0.2, vitesseGauche=0.2;

  int tGauche=0, tDroite=0;
  int gauche = 0, droite=0;
  int lastG=0, lastD=0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);

  if (angle>0)
  {
    //setup
    vitesseGauche =0.2;
    vitesseDroite=-0.2;
  
    //démarage moteur
    MOTOR_SetSpeed(0,vitesseGauche);
    MOTOR_SetSpeed(1,vitesseDroite);

    while (tGauche < tour-erreur || tDroite < tour-erreur)
    {
      droite =-1*ENCODER_Read(1);
      gauche =ENCODER_Read(0);
      tGauche += gauche-lastG;
      tDroite += droite-lastD;

      lastG= gauche;
      lastD = droite;

      if(gauche>cycle){
        ENCODER_Reset(1);
        ENCODER_Reset(0);
        //PID Proportionelle et intégral
        vitesseDroite += -1*((gauche - droite)*KP + (tGauche-tDroite)*KI);
        MOTOR_SetSpeed(1,vitesseDroite);
        lastG=0;
        lastD=0;
      }

      if(tGauche > tour-erreur){
        MOTOR_SetSpeed(0,0);
      }

      if(tDroite > tour-erreur){
        MOTOR_SetSpeed(1,0);
      }

    }
    
  }
  else if (angle<0)
  {

    vitesseDroite=0.2;
    vitesseGauche=-0.2;

    MOTOR_SetSpeed(0,vitesseGauche);
    MOTOR_SetSpeed(1,vitesseDroite);

    while (tGauche < tour-erreur  || tDroite < tour-erreur)
    {
       Serial.println("lol1");
      gauche=-1*ENCODER_Read(0);
      droite=ENCODER_Read(1);
      tGauche += gauche-lastG;
      tDroite += droite-lastD;

      lastG= gauche;
      lastD = droite;
      if(gauche>cycle){
        ENCODER_Reset(1);
        ENCODER_Reset(0);
        
        //PID Proportionelle et intégral
        Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
        vitesseDroite += ((gauche - droite)*KP + (tGauche-tDroite)*KI);
        lastG=0;
        lastD=0;
      }
      if(tGauche > tour-erreur){
        MOTOR_SetSpeed(0,0);
      }

      if(tDroite > tour-erreur){
        MOTOR_SetSpeed(1,0);
      }
    
    }
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  Serial.println(tGauche);
  Serial.println(tDroite);
  Serial.println("meu");
  Serial.println(tour-erreur);
  Serial.println("valeur");
  Serial.println(gauche);
  Serial.println(droite);
  Serial.println(ENCODER_Read(0));
  Serial.println(ENCODER_Read(1));

}
void avancePID(int32_t distance){  //en cm

  Serial.println((distance*3200));
  int32_t tour = (distance*3200)/24.5;
  const float KP = 0.0002;
  const float KI = 0.00004;
  int i;

  float vitesseGauche=0.3,vitesseDroite=0.3;
  ENCODER_Reset(0);
  ENCODER_ReadReset(1);
  MOTOR_SetSpeed(0,vitesseGauche);
  MOTOR_SetSpeed(1,vitesseDroite);
  
   int tGauche=0, tDroite=0;

   int gauche = 0, droite=0;

  while (tGauche < tour -225 || tDroite < tour-225 )
  {
    /*if(vitesseGauche<0.3 && i%3==0){
      vitesseGauche+=0.01;
      vitesseDroite+=0.01;
      MOTOR_SetSpeed(0,vitesseGauche);
    }*/
    droite =ENCODER_Read(1);
    gauche =ENCODER_Read(0);
    ENCODER_Reset(1);
    ENCODER_Reset(0);
    tGauche += gauche;
    tDroite += droite;
    //PID Proportionelle et intégral
    Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
    vitesseDroite += (gauche - droite)*KP + (tGauche-tDroite)*KI;//+0.0025;
    MOTOR_SetSpeed(1,vitesseDroite);
    //Serial.println(gauche);
    delay(50);
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  delay(100);
  
}
void lineFollow(){

}
int scanBall(){
  int startAngle=60;
  int totalAngle=2*startAngle;
  int numDivisions=20;
  int divLength=totalAngle/numDivisions;
  int disUp[numDivisions+1];
  int disDown[numDivisions+1];
  int disDiff[numDivisions+1];
  int highest=0;
  int pos=0;
  int posBall;
  turn(60);
  for (int i = 0;i<=numDivisions ; i++){ 
    disUp[i]=SharpIR1.distance();
    disDown[i]=SharpIR2.distance();
    turn(-divLength);
  }
  for (int i = 0; i<=numDivisions;i++){
    disDiff[i]=disUp[i]-disDown[i];
    if (disDiff[i]>highest){
      disDiff[i]=highest;
      pos=i;
    }
  }
  posBall=startAngle-(pos*divLength);
  return posBall;
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
  delay(2000);   

  int dis=SharpIR1.distance();  // this returns the distance to the object you're measuring

  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
}