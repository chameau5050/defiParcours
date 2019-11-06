#ifndef CAPTEURS_H
#define CAPTEURS_H
/*
fonction: trouver la balle dans la zone
entree: nombre de donnee a prendre dans le scan
sortie: distance de la balle par rapport au capteur
*/
void scanBall(int numDivisions);
//parametre d'entree est l'angle totale
void scan(int angle);

void PID_capteur();
void setup_couleur();
int LireCouleur();
#endif

