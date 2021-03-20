/** Projet Ecrans Agites : fonctions de transition de la vitesse entre son état minimal et son état stationnaire **/

#include "monteeDescente.hpp"

/** 
Fonction de montee de vitesse et de mise a jour des angles moteurs
Arguments :
	double* vit : tableau des vitesses a envoyer aux 3 moteurs
	int vit_modifs : indice du moteur a deplacer
	int signe : +1 ou -1 selon le sens de rotation du moteur choisi
	double intervalleEntre2Impulsions : valeur relative d'augmentation de la vitesse a chaque tour de boucle
	int V1_MAX : valeur maximale de vitesse a envoyer au moteur 1
	int V2_MAX : valeur maximale de vitesse a envoyer au moteur 2
	int V3_MAX : valeur maximale de vitesse a envoyer au moteur 2
	int V1_MIN : valeur minimale de vitesse a envoyer au moteur 1
	int V2_MIN : valeur minimale de vitesse a envoyer au moteur 2
	int V3_MIN : valeur minimale de vitesse a envoyer au moteur 3
	int DELAIS_CHANGE_COMPORT : delais entre 2 comportements differents
	double* mot : tableau des positions angulaires des 3 moteurs
	double intervalleRaffraichissementAngulaire : valeur relative d'augmentation de la position angulaire a chaque tour de boucle
	int M1_MAX : valeur maximale de position a envoyer au moteur 1
	int M2_MAX : valeur maximale de position a envoyer au moteur 2
	int M3_MAX : valeur maximale de position a envoyer au moteur 3
	
	Paramètres d'affichage de la simulation :
		int i : indice de fermeture de la fenetre
		Point pointLiaisonBati : point du centre de la fenetre
		Point pointCentreEcran : point du centre de l'ecran (rectangle blanc dans la simulation)
		Point pointLiaisonBras : point de liaison entre les 2 bras 
		double alpha : angle du dernier moteur par rapport au referentiel de la base (somme des positions angulaires des 3 moteurs)
		Mat imageTest : image source avant la rotation
		Mat imageTestModifiee : image apres rotation d'un angle alpha
		Mat imageFondModifiee : image de fond actualisee

Renvoie : le tableau vit[2] contenant les 3 vitesses a envoyees aux moteurs
 **/
double* monteeVitesse(double* vit, int vit_modifs, int signe, double intervalleEntre2Impulsions, int V1_MAX, int V2_MAX, int V3_MAX,
					 	int V1_MIN, int V2_MIN, int V3_MIN, 
						int DELAIS_CHANGE_COMPORT, double* mot, double intervalleRaffraichissementAngulaire, int M1_MAX, int M2_MAX, int M3_MAX,
						int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha,
						Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee)
{
	if(vit_modifs == 2)																				// si on fait accelerer le 3eme moteur
	{
		if(vit[2] == 0 && signe > 0) vit[2] = intervalleEntre2Impulsions;							// si le moteur est eteint
		if(vit[2] == 0 && signe < 0) vit[2] = -intervalleEntre2Impulsions;							// on lui donne une implusion de depart

		/* Fonction d'affichage de la simulation */
		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);

		int j = 0;
		if(signe > 0)																				// si on veut monter a V3_MAX
		{
			while((vit[2] < V3_MAX) || (j < DELAIS_CHANGE_COMPORT))
			{
				if(vit[2] > 0) 
				{
					vit[2] = vit[2] + vit[2]*intervalleEntre2Impulsions; 
					if(mot[2] < M3_MAX) mot[2] += vit[2]*intervalleRaffraichissementAngulaire;
				}								// si la vitesse est positive
				else if(vit[2] < 0)
				{ 
					vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions; 
					if(mot[2] < M3_MAX) mot[2] -= vit[2]*intervalleRaffraichissementAngulaire;
				}						// si la vitesse est negative
				affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
				j++;
			}
		}
		if(signe < 0)																				// si on veut descendre a V3_MIN
		{
			while((vit[2] > V3_MIN) || (j < DELAIS_CHANGE_COMPORT))
			{
				if(vit[2] > 0) 
				{
					vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions; 
					if(mot[2] < M3_MAX) mot[2] -= vit[2]*intervalleRaffraichissementAngulaire;
				}								// si la vitesse est negative
				else if(vit[2] < 0) 
				{
					vit[2] = vit[2] + vit[2]*intervalleEntre2Impulsions; 
					if(mot[2] < M3_MAX) mot[2] += vit[2]*intervalleRaffraichissementAngulaire;
				}					// si la vitesse est negative
				affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
				j++;
			}
		}
	}
	else if(vit_modifs == 1)
	{
		if(vit[vit_modifs] == 0 && signe > 0) vit[vit_modifs] = intervalleEntre2Impulsions;
		if(vit[vit_modifs] == 0 && signe < 0) vit[vit_modifs] = -intervalleEntre2Impulsions;

		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		
		cout << "montee etape 1 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		int j = 0;
		if(signe > 0)																				// si on veut monter a 1
		{
			while((vit[vit_modifs] < V2_MAX) || (j < DELAIS_CHANGE_COMPORT))
			{
				if(vit[vit_modifs] > 0) {vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[1] < M2_MAX) mot[vit_modifs] += vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}								// si la vitesse est negative
				else if(vit[vit_modifs] < 0){vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[1] < M2_MAX) mot[vit_modifs] -= vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}						// si la vitesse est negative
				//cout << "montee etape 2 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
				j++;
			}
		}
		if(signe < 0)																				// si on veut descendre a -1
		{
			while((vit[vit_modifs] > V2_MIN) || (j < DELAIS_CHANGE_COMPORT))
			{
				if(vit[vit_modifs] > 0) {vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[1] < M2_MAX) mot[vit_modifs] -= vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}								// si la vitesse est negative
				else if(vit[vit_modifs] < 0) {vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[1] < M2_MAX) mot[vit_modifs] += vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}					// si la vitesse est negative
				//cout << "montee etape 2 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
				j++;
			}
		}
	}	

	else if(vit_modifs == 0)
	{
		if(vit[vit_modifs] == 0 && signe > 0) vit[vit_modifs] = intervalleEntre2Impulsions;
		if(vit[vit_modifs] == 0 && signe < 0) vit[vit_modifs] = -intervalleEntre2Impulsions;

		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		
		cout << "montee etape 1 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		int j = 0;
		if(signe > 0)																				// si on veut monter a 1
		{
			while((vit[vit_modifs] < V1_MAX) || (j < DELAIS_CHANGE_COMPORT))
			{
				if(vit[vit_modifs] > 0) {vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[0] < M1_MAX) mot[vit_modifs] += vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}								// si la vitesse est negative
				else if(vit[vit_modifs] < 0){vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[0] < M1_MAX) mot[vit_modifs] -= vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}						// si la vitesse est negative
				//cout << "montee etape 2 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
				j++;
			}
		}
		if(signe < 0)																				// si on veut descendre a -1
		{
			while((vit[vit_modifs] > V1_MIN) || (j < DELAIS_CHANGE_COMPORT))
			{
				if(vit[vit_modifs] > 0) {vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[0] < M1_MAX) mot[vit_modifs] -= vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}								// si la vitesse est negative
				else if(vit[vit_modifs] < 0) {vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*intervalleEntre2Impulsions; 
					if(mot[0] < M1_MAX) mot[vit_modifs] += vit[vit_modifs]*intervalleRaffraichissementAngulaire;
				}					// si la vitesse est negative
				//cout << "montee etape 2 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
				j++;
			}
		}
	}	
	cout << "montee etape finale => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
	affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
	return vit;
}


double* descenteVitesse(double* vit, double intervalleEntre2Impulsions, int V1_MAX, int V2_MAX, int V3_MAX, int V1_MIN, int V2_MIN, int V3_MIN, int DELAIS_CHANGE_COMPORT,
					double* mot, double intervalleRaffraichissementAngulaire, int M1_MAX, int M2_MAX, int M3_MAX,
					int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha,
					Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee)
{
	//cout << "descente etape 0 => vit 1 = " << vit[0] << "; vit 2 = " << vit[1] << "; vit 3 = " << vit[2] << endl;

	if((vit[0] < V1_MIN) && (vit[1] < V2_MIN) && (vit[2] < V3_MIN)) return vit;
	if((mot[0] < M1_MAX) && (mot[1] < M2_MAX) && (mot[2] < M3_MAX)) return vit;

	while(vit[0] > intervalleEntre2Impulsions || vit[0] < -intervalleEntre2Impulsions)
	{
		if(vit[0] > intervalleEntre2Impulsions) vit[0] = vit[0] - vit[0]*intervalleEntre2Impulsions;
		if(vit[0] < -intervalleEntre2Impulsions) vit[0] = vit[0] - vit[0]*intervalleEntre2Impulsions;

		if(mot[0] < M1_MAX) mot[0] += mot[0]*intervalleRaffraichissementAngulaire;

		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		//cout << "descente etape 1 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
	}

	while(vit[1] > intervalleEntre2Impulsions || vit[1] < -intervalleEntre2Impulsions)
	{
		if(vit[1] > intervalleEntre2Impulsions) vit[1] = vit[1] - vit[1]*intervalleEntre2Impulsions;
		if(vit[1] < -intervalleEntre2Impulsions) vit[1] = vit[1] - vit[1]*intervalleEntre2Impulsions;
		if(mot[1] < M2_MAX) mot[1] += mot[1]*intervalleRaffraichissementAngulaire;
		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		//cout << "descente etape 1 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
	}

	while(vit[2] > intervalleEntre2Impulsions || vit[2] < -intervalleEntre2Impulsions)
	{
		if(vit[2] > intervalleEntre2Impulsions) vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions;
		if(vit[2] < -intervalleEntre2Impulsions) vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions;
		if(mot[2] < M3_MAX) mot[2] += mot[2]*intervalleRaffraichissementAngulaire;
		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		//cout << "descente etape 1 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
	}

	affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
	cout << "descente etape finale => vit 1 = " << vit[0] << "; vit 2 = " << vit[1] << "; vit 3 = " << vit[2] << endl;

	return vit;
}