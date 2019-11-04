#include "servomoteur.h"
#include "LibRobus.h"


void servomoteur(int angle)
{
   SERVO_Enable(0);
  if (angle==0)
  {
     SERVO_SetAngle(0,50);
  }
  

  if (angle==1)
  {
     SERVO_SetAngle(0,150);
  }
  
}