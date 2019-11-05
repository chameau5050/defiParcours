#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <stdio.h>
#include <arduino.h>
#include <ADJDS311.h>
#include "capteurs.h"

ADJDS311 colorSensor(2);

void setup_couleur()
{
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
