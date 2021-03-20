/** Projet Ecrans Agites : fonctions de transition de la vitesse entre son état minimal et son état stationnaire **/

#ifndef MONTEE_DESCENTE_HPP
#define MONTEE_DESCENTE_HPP

#include <iostream>

using namespace std;

#include "Modele.hpp"

double* monteeVitesse(double* vit, int vit_modifs, int signe, double intervalleEntre2Impulsions, int V1_MAX, int V2_MAX, int V3_MAX, 
					int V1_MIN, int V2_MIN, int V3_MIN,
					int DELAIS_CHANGE_COMPORT, double* mot, double intervalleRaffraichissementAngulaire, int M1_MAX, int M2_MAX, int M3_MAX,
					int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha,
					Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee);

double* descenteVitesse(double* vit, double intervalleEntre2Impulsions, int V1_MAX, int V2_MAX, int V3_MAX, int V1_MIN, int V2_MIN, int V3_MIN, int DELAIS_CHANGE_COMPORT,
					double* mot, double intervalleRaffraichissementAngulaire, int M1_MAX, int M2_MAX, int M3_MAX,
					int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha,
					Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee);

#endif