#include <LibRobus.h>
#include "capteur.h"
#include "move.h"
#include <SharpIR.h>
#include <stdio.h>
#include <arduino.h>
#include <ADJDS311.h>
#define model 1080
#define ir1 A8
#define ir2 A12
SharpIR SharpIR1(ir1, model);//up
SharpIR SharpIR2(ir2, model);//down
ADJDS311 colorSensor(2);
void setup_couleur(){
  Serial.begin(9600);
  colorSensor.init();
  colorSensor.ledOn();  //Allume la led blanche du capteur de couleur
  Serial.println("ledon");
  colorSensor.calibrate();  //Calibre le capteur (doit être sur une surface blanche)
  Serial.println("fin calibrate");
}
int LireCouleur()
//Fonction qui retourne la couleur
//La fonction lit la couleur 5 fois afin d'éviter les erreurs de lecture
{
  int couleur[6] = {0, 0, 0, 0, 0, 0};  //Sert à avoir la récurrence de chaque couleurs lors des 5 lectures
  int PlusGrande = 0; //Sert a stocker la récurence la plus élevée

  //Vérifie la couleur 5 fois et incrémente la valeur respective dans couleur[]
  for(int i = 0; i < 3; i++)
    {
        RGBC color = colorSensor.read();

        if(color.red > color.green && color.red > color.blue && color.red < 1000 && color.red > 500)
        {
        couleur[0]++;
        }
        if(color.green > color.red && color.green > color.blue)
        {
        couleur[1]++;
        }
        if(color.blue > color.red && color.blue > color.green)
        {
        couleur[2]++;
        }
        if(color.red > 900 && color.green > 900 && color.blue < 800)
        {
        couleur[3]++;
        }
        if(color.red > 500 && color.green > 500 && color.blue > 500)
        {
        couleur[4]++;
        }
        if(color.red < 500 && color.green < 500 && color.blue < 500)
        {
        couleur[5]++;
        }
        delay(10);
    }

    //Trouve la plus forte récurrence lors des lectures de couleur
    for(int i = 0; i < 6; i++)
    {
        if(couleur[i] > PlusGrande)
        {
        PlusGrande = couleur[i];
        }
    }

    //Retourne la couleur la plus récurente lors des 5 lectures
    if(PlusGrande == couleur[0])
    {
        return 1; //rouge
    }
    else if(PlusGrande == couleur[1])
    {
        return 1; //vert
    }
    else if(PlusGrande == couleur[2])
    {
        return 1; //bleu
    }
    else if(PlusGrande == couleur[3])
    {
        return 1; //jaune
    }
    else if(PlusGrande == couleur[4])
    {
        return 0; //blanc
    }
    else if(PlusGrande == couleur[5])
    {
        return 2; //noir
    }
    else
    {
        return -1;
    }
    
}
void scan(int angle){
  int disUp[angle+1];
  int disDown[angle+1];
  int disDiff[angle+1];
  int highest=0;
  int lowest=80;
  int pos=0;
  int i=0;
  while(ENCODER_Read(0)/21<angle/2||ENCODER_Read(1)/21>angle/2){
    MOTOR_SetSpeed(0,0.1);
    MOTOR_SetSpeed(1,-0.1);
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while(ENCODER_Read(0)/21>angle||ENCODER_Read(1)/21<angle){
    MOTOR_SetSpeed(0,-0.1);
    MOTOR_SetSpeed(1,0.1);
    while(i<angle){ 
      if(ENCODER_Read(1)%21==0){
        disUp[i]=SharpIR1.distance();
        disDown[i]=SharpIR2.distance();
     //Serial.println(disUp[i]);
      //Serial.println(disDown[i]);
     // Serial.println(disUp[i]-disDown[i]);
      /*Serial.println(i);
      Serial.println("up");
      Serial.println(disUp[i]);
      Serial.println("down");
      Serial.println(disDown[i]);
      */
        disDiff[i]=disUp[i]-disDown[i];
          if ((disDiff[i]>highest) && (disDown[i]<80)&& disDiff[i]<70){
            highest=disDiff[i];
            pos=i;
            Serial.println(disDiff[i]);
            Serial.println("looooollll");
          }
       /*else if ((disUp[i]>70) && (disDown[i]<lowest)&&(disDown[i]<70)){
         lowest=disDown[i];
         pos=i;
       }*/
        i+=1;
        }
    }
  }
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  Serial.println("end");
  Serial.println(pos);
  turnAngle((angle-pos));
  spikePID(1000);
  turnAngle(pos-(angle/2));
}
void scanBall(int numDivisions){
  int startAngle=45;
  int totalAngle=2*startAngle;
  int divLength=totalAngle/numDivisions;
  int disUp[numDivisions+1];
  int disDown[numDivisions+1];
  int disDiff[numDivisions+1];
  int highest=0;
  int pos=0;
  int posBall;
  turnAngle(startAngle);
  for (int i = 0;i<=numDivisions ; i++){ 
    delay(500);

    disUp[i]=SharpIR1.distance();
    disDown[i]=SharpIR2.distance();
    //Serial.println(disUp[i]);
    //Serial.println(disUp[i]-disDown[i]);
    turnAngle(-divLength*7/10);
  }
  for (int i = 0; i<=numDivisions;i++){
    disDiff[i]=disUp[i]-disDown[i];
    Serial.println(disDiff[i]);
    if (disDiff[i]>highest){
      if (disDiff[i]<70 && disDown[i]<80){
      highest=disDiff[i];
      pos=i;
      //Serial.println(highest);
      }
    }
  }
  posBall=startAngle-(pos*divLength);
  turnAngle(startAngle);
  turnAngle(posBall);
  spikePID(1000);
  turnAngle((-posBall));
}

