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
void maFonction(){
  // code
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

int stopG=0, stopD = 0;
float Vg=0.2, Vd=0.2; 

void calibration(){

  avancePIDCalib(15);
  
}

void avancePIDCalib(int32_t distance){

  Serial.println((distance*3200));
  int32_t tour = (distance*3200)/24.5;
  const float KP = 0.0001;
  const float KI = 0.00002;

  float vitesseGauche=0.2,vitesseDroite=0.2;
  ENCODER_Reset(0);
  ENCODER_ReadReset(1);
  MOTOR_SetSpeed(0,Vg);
  MOTOR_SetSpeed(1,Vd);
  
   int tGauche=0, tDroite=0;

   int gauche = 0, droite=0;

  while (tGauche < tour-stopG || tDroite < tour-stopD)
  {
    droite =ENCODER_Read(1);
    gauche =ENCODER_Read(0);
    ENCODER_Reset(1);
    ENCODER_Reset(0);
   
    tGauche += gauche;
    tDroite += droite;
    //PID Proportionelle et intégral
    Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
    Vd += (gauche - droite)*KP + (tGauche-tDroite)*KI;
    MOTOR_SetSpeed(1,Vd);
    //Serial.println(gauche);
    delay(100);

  }
  
  stopG = tGauche + ENCODER_Read(0)-tour ;
  stopD = tDroite + ENCODER_Read(1)-tour;


  Serial.println(stopG);
  Serial.println(stopD);
  Serial.println(Vd);
  Serial.println(Vg);
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"


void avancePID(int32_t distance){

  Serial.println((distance*3200));
  int32_t tour = (distance*3200)/24.5;
  const float KP = 0.0001;
  const float KI = 0.00002;

  float vitesseGauche=0.3,vitesseDroite=0.3 ;
  ENCODER_Reset(0);
  ENCODER_ReadReset(1);
  MOTOR_SetSpeed(1,vitesseDroite);
  MOTOR_SetSpeed(0,vitesseGauche);
  
   int tGauche=0, tDroite=0;
  
   int gauche = 0, droite=0;

  while (tGauche < tour -225 || tDroite < tour-225 )
  {
    droite =ENCODER_Read(1);
    gauche =ENCODER_Read(0);
    ENCODER_Reset(1);
    ENCODER_Reset(0);
   
    tGauche += gauche;
    tDroite += droite;
    //PID Proportionelle et intégral
    Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
    vitesseDroite += (gauche - droite)*KP + (tGauche-tDroite)*KI;
    MOTOR_SetSpeed(1,vitesseDroite);
    //Serial.println(gauche);
    delay(50);

  }
  

  Serial.println("patate");
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  delay(100);
  
}

void avancePID2(int32_t distance){
  int32_t tour = (distance*3200)/24.5;
  const float KP = 0.0001;
  const float KI = 0.00002;

  float vitesseGauche=0.2,vitesseDroite=0.2;
  ENCODER_Reset(0);
  ENCODER_ReadReset(1);
  MOTOR_SetSpeed(0,vitesseGauche);
  MOTOR_SetSpeed(1,vitesseDroite);
  
   int tGauche=0, tDroite=0;
   int gauche = 0, droite=0;
  int lastG=0, lastD=0;

  

  while (tGauche < tour -225 || tDroite < tour-225 )
  {
    droite =ENCODER_Read(1);
    gauche =ENCODER_Read(0);
    tGauche += gauche;
    tDroite += droite;

    lastG= gauche;
    lastD = droite;
    if(gauche>600){
      ENCODER_Reset(1);
      ENCODER_Reset(0);

      //PID Proportionelle et intégral
      Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
      vitesseDroite += (gauche - droite)*KP + (tGauche-tDroite)*KI;
      MOTOR_SetSpeed(1,vitesseDroite);
      lastG=0;
      lastD=0;
    }
  
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
}


void turn4(int mult45){
  
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

void turn3(int mult45)
{
  const int cycle = 600;
  const float KP = 0.0001;
  const float KI = 0.00002;
  const int pulse45 = 950 ;
  const int erreur_45 = 3;
  int tour = mult45*pulse45;
  
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
  Serial.println("valeur");
  Serial.println(gauche);
  Serial.println(droite);
  Serial.println(ENCODER_Read(0));
  Serial.println(ENCODER_Read(1));

delay(100);
}

void turn(int mult45)
{
  

  
  int pulse0, pulse1, pulse45;
  pulse45=1012;
  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);
  pulse0=ENCODER_Read(0);
  pulse1=ENCODER_Read(1);
  if (mult45>0)
  {
    while (pulse0<(mult45*pulse45) && pulse1>-1*(mult45*pulse45))
    {
      
      MOTOR_SetSpeed(0,0.2);
      MOTOR_SetSpeed(1,-0.2);
      pulse0=ENCODER_Read(0);
      pulse1=ENCODER_Read(1);
    

    }
    
  }
  else if (mult45<0)
  {
    while (pulse0>(mult45*pulse45)  && pulse1<-1*(mult45*pulse45))
    {
     
      MOTOR_SetSpeed(0,-0.2);
      MOTOR_SetSpeed(1,0.2);
      pulse0=ENCODER_Read(0);
      pulse1=ENCODER_Read(1);

    }
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  

delay(100);

}

void turn2(int mult45)
{
  const float KP = 0.0001;
  const float KI = 0.00002;

  int pulse0, pulse1, pulse45,erreur_45;
  pulse45=1024;
  erreur_45=210;
  int tGauche=0, tDroite=0;
  
   ENCODER_ReadReset(0);
   ENCODER_ReadReset(1);
    pulse0=ENCODER_Read(0);
    pulse1=ENCODER_Read(1);
  if (mult45>0)
  {
    MOTOR_SetSpeed(0,0.2);
    MOTOR_SetSpeed(1,-0.2);

    float vitesseDroite=-0.2;
    tGauche=0; 
    tDroite=0;

    int gauche = 0, droite=0;

    while (tGauche<((mult45*pulse45)-erreur_45) && tDroite>-1*((mult45*pulse45)-erreur_45))
    {
    Serial.println(tGauche);
    Serial.println((mult45*pulse45)-erreur_45);
    droite =-1*ENCODER_Read(1);
    gauche =ENCODER_Read(0);
    ENCODER_Reset(1);
    ENCODER_Reset(0);
   
    tGauche += gauche;
    tDroite += droite;
    //PID Proportionelle et intégral
    vitesseDroite += -1*((gauche - droite)*KP + (tGauche-tDroite)*KI);
    MOTOR_SetSpeed(1,vitesseDroite);
    delay(100);
    

    }
    
  }
  else if (mult45<0)
  {

    float vitesseDroite=0.2;
    tGauche=0; 
    tDroite=0;

    int gauche = 0, droite=0;

    MOTOR_SetSpeed(0,-0.2);
    MOTOR_SetSpeed(1,0.2);
    while (tGauche>((mult45*pulse45)-erreur_45)  && tDroite<-1*((mult45*pulse45)-erreur_45))
    {
     
      gauche=-1*ENCODER_Read(0);
      droite=ENCODER_Read(1);
      ENCODER_Reset(1);
      ENCODER_Reset(0);
   
      tGauche += gauche;
      tDroite += droite;
      Serial.println("a");
      Serial.println(vitesseDroite);

      //PID Proportionelle et intégral
      Serial.println((gauche - droite)*KP + (tGauche-tDroite)*KI);
      vitesseDroite += ((gauche - droite)*KP + (tGauche-tDroite)*KI);
      MOTOR_SetSpeed(1,vitesseDroite);
      delay(100);

    }
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);

delay(100);
Serial.println(tGauche);
Serial.println(tDroite);
}





void loop() {
  
  
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels

if(ROBUS_IsBumper(2)){
  calibration();
}else if(ROBUS_IsBumper(3)){
 avancePID(124);//122.5
  turn4(-2);
  avancePID(100);
  turn4(2);
  avancePID(45);
  turn4(2);
  avancePID(55);
  turn4(-2);
  avancePID(210);
  turn4(2);
  avancePID(45);
  turn4(-2);
  avancePID(140);
  turn4(-4);
  //ballon
  avancePID(250);
  turn2(1000);
  /*
  avancePID(45);
  turn4(-2);
  avancePID(210);
  turn4(2);
  avancePID(55);
  turn4(-2);
  avancePID(45);
  turn4(-2);
  avancePID(124);

  avancePID(100);
  turn4(2);
  avancePID(245);
  turn4(4);
  avancePID(102.5);
  turn4(-2);
  avancePID(45);
  turn4(-2);
  avancePID(210);
  turn4(2);
  avancePID(55);
   turn4(-2);
  avancePID(50);
  turn4(-2);
  avancePID(100);
  turn4(2);
  avancePID(122.5);
  */  
}



//MOTOR_SetSpeed(0,-0.2);
//MOTOR_SetSpeed(1,0.2);


// Delais pour décharger le CPU
/*
float nbpulsegauche;
nbpulsegauche= ENCODER_Read(0);

float distancecourt1;
distancecourt1=100;
distancelongue1=145;
distancecourt2=67.5;
distancelongue2=112.5;
distancecourt3=87.5;
distancelongue3=106.5;
distancecourt4=172;
distancelongue4=196;
distancecourt5=66.5;
distancelongue5=48.5;
distancecourt6=100;
distancelongue6=109;



while (nbpulsegauche<)
MOTOR_SetSpeed(0,0,2)
MOTOR_SetSpeed(1,0,2)*/

}