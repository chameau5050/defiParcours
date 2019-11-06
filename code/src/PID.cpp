#include <LibRobus.h>
#include <stdio.h>
#include <arduino.h>
#include "capteur.h"

void PID_gauche(int VitesseC, int Erreur, int SommeErreurs, float VitesseV)
{
  const int MoteurGauche=0;
  const float KP = 0.0001, KI = 0.00002;//a travailler
    
    Erreur=VitesseC-ENCODER_Read(MoteurGauche);

    SommeErreurs+=Erreur;

    VitesseV+=KP*(Erreur)+KI*(SommeErreurs);
    MOTOR_SetSpeed(MoteurGauche, VitesseV);

    ENCODER_Reset(MoteurGauche);
}

void PID_droite(int VitesseC, long long Erreur, long long SommeErreurs, float VitesseV)
{
  const int MoteurDroite=1;
  const float KP = 0.0001, KI = 0.00002;//a travailler

  Erreur=VitesseC-ENCODER_Read(MoteurDroite);

  SommeErreurs+=Erreur;

  VitesseV+=KP*(Erreur)+KI*(SommeErreurs);
  MOTOR_SetSpeed(MoteurDroite, VitesseV);

  ENCODER_Reset(MoteurDroite);
}

void PID_capteur()
{
  int gauche_3=analogRead(A7), gauche_2=analogRead(A6), gauche_1=analogRead(A5), gauche_0=analogRead(A4);
  int droite_3=analogRead(A0), droite_2=analogRead(A1), droite_1=analogRead(A2), droite_0=analogRead(A3);
  float VitesseG=0, VitesseD=0;
  long long ErreurCaptP=0, ErreurCaptI=0, ErreurCaptD=0;
  const float KP = 0.0001, KI = 0.000001, KD=0.0001;//a travailler
  const int BUMPER_ARRIERE=3, MoteurGauche=0, MoteurDroit=1;

  while(LireCouleur()==0 && ROBUS_IsBumper(BUMPER_ARRIERE)==0) //|| (analogRead(A7)<500 && analogRead(A0)<500))
  {
    Serial.print("couleur: ");
    Serial.println(LireCouleur());

    gauche_2=analogRead(A6);
    gauche_1=analogRead(A5);
    gauche_0=analogRead(A4);
    droite_0=analogRead(A3);
    droite_1=analogRead(A2);
    droite_2=analogRead(A1);

    ErreurCaptD=(gauche_2*20+gauche_1*10+gauche_0)-(droite_0+droite_1*10+droite_2*20)-ErreurCaptP;

    ErreurCaptP=(gauche_2*20+gauche_1*10+gauche_0)-(droite_0+droite_1*10+droite_2*20); //Noir=Plus haut

    if(ErreurCaptP>-25/KI || ErreurCaptP<25/KI)
    {
      ErreurCaptI+=ErreurCaptP;
    }

    VitesseD=KP*(ErreurCaptP)+KI*(ErreurCaptI)+KD*(ErreurCaptD);
    VitesseG=KP*(-ErreurCaptP)+KI*(-ErreurCaptI)+KD*(-ErreurCaptD);

    MOTOR_SetSpeed(MoteurGauche, 0.5+VitesseG);
    MOTOR_SetSpeed(MoteurDroit, 0.5+VitesseD);

    delay(10);
  }
}

void Deceleration()
{
  long long ErreurP=0.001, ErreurI=0, ErreurD=0, EncodeurDroit=0, EncodeurGauche=0;
  float VitDroit=0.5, VitGauche=0.5;
  const int PulsesArret=225, MoteurGauche=0, MoteurDroit=1, NbPulses=(460/(2*PI*1.5*25.4))*3200;//Premiere valeur de NbPulses=avance en mm;//a travailler
  const float KP = 0.0001, KI = 0., KD=0.;//a travailler

  ENCODER_Reset(MoteurDroit);
  ENCODER_Reset(MoteurGauche);

  while (EncodeurDroit<NbPulses-PulsesArret && EncodeurGauche<NbPulses-PulsesArret)
  {
    if(VitGauche>0.2)
    {
      VitGauche-=0.0001;
    }

    EncodeurDroit+=ENCODER_Read(MoteurDroit);
    EncodeurGauche+=ENCODER_Read(MoteurGauche);

    ErreurD=(ENCODER_Read(MoteurGauche)-ENCODER_Read(MoteurDroit))-ErreurP;

    ErreurP=(ENCODER_Read(MoteurGauche)-ENCODER_Read(MoteurDroit));

    if((ErreurP>-25/KI)&&(ErreurP<25/KI))
    {
      ErreurI+=ErreurP;
    }

    VitDroit+=KP*ErreurP+KI*ErreurI+KD*ErreurD;

    MOTOR_SetSpeed(MoteurDroit, VitGauche);
    MOTOR_SetSpeed(MoteurDroit, VitDroit);

    ENCODER_Reset(MoteurDroit);
    ENCODER_Reset(MoteurGauche);

    Serial.println("dans decel");

    delay(10);
  }
  MOTOR_SetSpeed(MoteurDroit, 0);
  MOTOR_SetSpeed(MoteurGauche, 0);
}