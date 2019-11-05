
#include"move.h"
#include<LibRobus.h>

void turnAngle(int angle){ //pour tournee en multiple de 1 degre
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

void avancePID(int distance){//avance pour avancer selon une distance

  Serial.println((distance*3200));
  int32_t d = distance;
  int32_t tour = (d*3200)/24.5;
  const float KP = 0.0001;
  const float KI = 0.00002;

  float vitesseGauche=0.3,vitesseDroite=0.3 ;
  ENCODER_Reset(0);
  ENCODER_ReadReset(1);
  MOTOR_SetSpeed(1,vitesseDroite);
  MOTOR_SetSpeed(0,vitesseGauche);
  
   int tGauche=0, tDroite=0;
  
   int gauche = 0, droite=0;
    int lastD=0, lastG=0;
  while (tGauche < tour -225 || tDroite < tour-225 )
  {
    droite = ENCODER_Read(1);
    gauche = ENCODER_Read(0);
    ENCODER_Reset(1);
    ENCODER_Reset(0);
    Serial.println(tour);
    Serial.println(tGauche);
    Serial.println(tDroite);
    tGauche += gauche ;
    tDroite += droite ;

    //PID Proportionelle et intégral
    Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
    vitesseDroite += (gauche - droite)*KP + (tGauche-tDroite)*KI;
    MOTOR_SetSpeed(1,vitesseDroite);
    //Serial.println(gauche);
    lastD = droite;
    lastG = gauche;

    delay(50);

  }
  

  Serial.println("patate");
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  delay(100);
  
}


void turn3(int mult45)
{
  const int cycle = 600;
  const float KP = 0.0001;
  const float KI = 0.00002;
  const int pulse45 = 950 ;
  const int erreur_45 = 100;
  int tour = mult45*pulse45;
  
  if(tour<0)
    tour*=-1;
  Serial.println(tour);
  float vitesseDroite=-0.2, vitesseGauche=0.2;

  int tGauche=0, tDroite=0;
  int gauche = 0, droite=0;
  int lastG=0, lastD=0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);

  if (mult45>0)
  {
    //setup
    vitesseGauche =0.2;
    vitesseDroite=-0.2;
  
    //démarage moteur
    MOTOR_SetSpeed(1,vitesseDroite);
    MOTOR_SetSpeed(0,vitesseGauche);

    while (tGauche < tour-erreur_45 || tDroite < tour-erreur_45)
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

      if(tGauche > tour-erreur_45){
        MOTOR_SetSpeed(0,0);
      }

      if(tDroite > tour-erreur_45){
        MOTOR_SetSpeed(1,0);
      }

    }
    
  }
  else if (mult45<0)
  {

    vitesseDroite=0.2;
    vitesseGauche=-0.2;
    MOTOR_SetSpeed(1,vitesseDroite);
    MOTOR_SetSpeed(0,vitesseGauche);
   

    while (tGauche < tour-erreur_45  || tDroite < tour-erreur_45)
    {
       Serial.println("lol1");
      gauche=-1*ENCODER_Read(0);
      droite=ENCODER_Read(1);
      tGauche += gauche-lastG;
      tDroite += droite-lastD;
        Serial.println(tour-erreur_45);
        Serial.println(tGauche);
        Serial.println(tDroite);
      lastG = gauche;
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
      if(tGauche > tour-erreur_45){
        MOTOR_SetSpeed(0,0);
      }

      if(tDroite > tour-erreur_45){
        MOTOR_SetSpeed(1,0);
      }
    
    }
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  Serial.println(tGauche);
  Serial.println(tDroite);
  Serial.println("meu");
  Serial.println(tour-erreur_45);
  Serial.println("valeur--------------------------------------------------");
  Serial.println(gauche);
  Serial.println(droite);
  Serial.println(ENCODER_Read(0));
  Serial.println(ENCODER_Read(1));

delay(100);
}

void turn(int mult45){
  
  int signe;
  if(mult45<0)
      signe=-1;
  else
  {
    signe =1;
  }
  
  for(int x=0; x< mult45*signe;x++){
    turn3(signe);
  }
  
}