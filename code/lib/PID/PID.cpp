#include <LibRobus.h>
#include <stdio.h>
#include <arduino.h>

void PID_capteur()
{
  int capteur_couleur=1;//digitalRead(2);
  float VitesseG=0, VitesseD=0;
  long long ErreurCapt=0, SommeErreursCapt=0, ErreurCaptPrec=0;
  const float KP = 0.0001, KI = 0.00001, KD=0.0001;//a travailler
  const int BUMPER_ARRIERE=3, MoteurGauche=0, MoteurDroit=1;

  while(capteur_couleur==0 || ROBUS_IsBumper(BUMPER_ARRIERE)==0)
  {
    int gauche_3=analogRead(A7), gauche_2=analogRead(A6), gauche_1=analogRead(A5), gauche_0=analogRead(A4);
    int droite_3=analogRead(A0), droite_2=analogRead(A1), droite_1=analogRead(A2), droite_0=analogRead(A3);

    ErreurCaptPrec=(gauche_2*20+gauche_1*10+gauche_0)-(droite_0+droite_1*10+droite_2*20)-ErreurCapt;

    ErreurCapt=(gauche_2*15+gauche_1*10+gauche_0)-(droite_0+droite_1*10+droite_2*15);

/*
    if(ErreurCapt>-200 || ErreurCapt<200)
    {
      SommeErreursCapt+=ErreurCapt;
    }
*/

    if(ErreurCapt<-200 || ErreurCapt>200)
    {
      VitesseD=KP*(ErreurCapt)+KI*(SommeErreursCapt)+KD*(ErreurCaptPrec);
      VitesseG=KP*(-ErreurCapt)+KI*(-SommeErreursCapt)+KD*(-ErreurCaptPrec);
    }

    MOTOR_SetSpeed(MoteurGauche, 0.5+VitesseG);
    MOTOR_SetSpeed(MoteurDroit, 0.5+VitesseD);

    delay(10);
  }
}