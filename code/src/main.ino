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
#define model 1080
#define ir1 A8
#define ir2 A9
SharpIR SharpIR1(ir2, model);//up
SharpIR SharpIR2(ir1, model);//down
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces
double cirRoue = 24.35;
double rayonRot = 9.75;
double cirRot = 61.261;
int target=1;

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
  float vitesseDroite=-0.1, vitesseGauche=0.1;

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
 
}
void avancePID(int32_t distance){  //en cm

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
    vitesseDroite += (gauche - droite)*KP + (tGauche-tDroite)*KI;//+0.0025;
    MOTOR_SetSpeed(1,vitesseDroite);
    //Serial.println(gauche);
    delay(50);
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  delay(100);
  
}
int scanBall(int numDivisions){
  int startAngle=60;
  int totalAngle=2*startAngle;
  int divLength=totalAngle/numDivisions;
  int disUp[numDivisions+1];
  int disDown[numDivisions+1];
  int disDiff[numDivisions+1];
  int highest=0;
  int pos=0;
  int posBall;
  turn(startAngle);
  for (int i = 0;i<=numDivisions ; i++){ 
    delay(500);
    disUp[i]=SharpIR1.distance();
    disDown[i]=SharpIR2.distance();
    Serial.println(disUp[i]);
    Serial.println(disDown[i]);
    turn(-divLength*8/10);
  }
  for (int i = 0; i<=numDivisions;i++){
    disDiff[i]=disUp[i]-disDown[i];
    if (disDiff[i]>highest){
      if (disDiff[i]<70){
      highest=disDiff[i];
      pos=i;
      }
    }
  }
  posBall=startAngle-(pos*divLength);
  turn(startAngle);
  turn(posBall);
  return disDown[pos];
}
int presligne(){//fonction qui voit si le suiveur a vu une ligne noir
  return 0;
}
void PID_capteur(){

}
int couleur(){//red =0 vert=1 bleu=2 jaune=3

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
  int dis=scanBall(20);
  delay(1000);
  avancePID(dis*3);
  delay(500);
}