#include <LibRobus.h>
#include "capteur.h"
#include "move.h"
#include <SharpIR.h>
//#include <ADJDS311.h>
#define model 1080
#define ir1 A8
#define ir2 A9
SharpIR SharpIR1(ir2, model);//up
SharpIR SharpIR2(ir1, model);//down

int scanBall(int numDivisions){
  int startAngle=30;
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
    Serial.println(disUp[i]);
    Serial.println(disDown[i]);
    turnAngle(-divLength*7/10);
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
  turnAngle(startAngle);
  turnAngle(posBall);
  return disDown[pos];
}

