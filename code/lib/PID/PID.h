#ifndef PID_H
#define PID_H

void PID_gauche(int VitesseC, long long Erreur, long long SommeErreurs, float VitesseV);
void PID_droite(int VitesseC, long long Erreur, long long SommeErreurs, float VitesseV);
void PID_capteur();
void Deceleration();

#endif