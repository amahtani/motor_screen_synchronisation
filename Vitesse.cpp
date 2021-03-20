/** Projet Ecrans Agites : asservissement en vitesse des 3 moteurs selon un comportement **/

#include "Vitesse.hpp"

#define PI 3.14159265358979323846

/**
	Fonction d'envoi de messages sur ROS
**/
/*void writeI2C()
{
	while (ros::OK())
	{
		i2c_chatter_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
}*/


/**
Fonction d'asservissement des vitesses selon le comportement choisi aléatoirement
	Arguments :
	double* mot : tableau des angles a envoyer aux 3 moteurs
	int delaisEntre2Comportements : delais entre 2 comportements differents
	int V1_MAX : valeur maximale de vitesse a envoyer au moteur 1
	int V2_MAX : valeur maximale de vitesse a envoyer au moteur 2
	int V3_MAX : valeur maximale de vitesse a envoyer au moteur 2
	int V1_MIN : valeur minimale de vitesse a envoyer au moteur 1
	int V2_MIN : valeur minimale de vitesse a envoyer au moteur 2
	int V3_MIN : valeur minimale de vitesse a envoyer au moteur 3
	double M1_MAX : valeur maximale de position a envoyer au moteur 1
	double M2_MAX : valeur maximale de position a envoyer au moteur 2
	double M3_MAX : valeur maximale de position a envoyer au moteur 3
	double M1_MIN : valeur minimale de position a envoyer au moteur 1
	double M2_MIN : valeur minimale de position a envoyer au moteur 2
	double M3_MIN : valeur minimale de position a envoyer au moteur 3
	double intervalleEntre2Impulsions : valeur relative d'augmentation de la vitesse a chaque tour de boucle
	double intervalleRaffraichissementAngulaire : valeur relative d'augmentation de la position angulaire a chaque tour de boucle
**/
void asservissementVitesses(double* mot, int delaisEntre2Comportements, int V1_MAX, int V2_MAX, int V3_MAX, int V1_MIN, int V2_MIN, int V3_MIN,
							double M1_MAX, double M2_MAX, double M3_MAX, double M1_MIN, double M2_MIN, double M3_MIN, 
							double intervalleEntre2Impulsions, double intervalleRaffraichissementAngulaire)
{		
	/* Paramètres */
	Mat imageTest;
	Mat imageTestModifiee;
	Mat imageFond;
	Mat imageFondModifiee;

	const int largeurFenetre(600);
	const int hauteurFenetre(600);

	double vit[3] = {0, 0, 0}; 		  // vitesses moteurs
	double alpha (0); // Angle de l'ecran par rapport a R0

	int delaisChangementComportement (0);
	bool autorisationChangementComportement(true);

	Point pointCentreEcran, pointLiaisonBras, pointLiaisonBati;

	////////////////////////////////// init ROS  ////////////////////////////////////////////////
	//ros::init(argc, "i2c_talker");
	//ros::NodeHandle n;
	//ros::Publisher i2c_chatter_pub = n.advertise<std_msgs::String>("i2c_chatter", 1000);
	//ros::Rate loop_rate(10);


	const char* titreFenetre = "Test openCV";
	const char* adresseImageTest = "screencut.jpg";
	const char* adresseImageFond = "fondNoir.png";

	enum Comportement comportementActuel(Arret);


	/* Initialisations */
	imageTest = imread(adresseImageTest, 1);
	if(!imageTest.data)
	{
		cout <<  "Could not open or find the image 1" << endl ;
		return;
	}

	imageFond = imread(adresseImageFond, 1);
	if(!imageFond.data)
	{
		cout <<  "Could not open or find the image" << endl ;
		return;
	}


	namedWindow(titreFenetre, WINDOW_NORMAL);
	resizeWindow(titreFenetre, largeurFenetre, hauteurFenetre);
	moveWindow(titreFenetre, 100, 100);


	// Position des points de liaisons du systeme
	pointLiaisonBati = Point(imageFond.cols/2,imageFond.rows/2); // Attache au bati
	modelGeometriqueDirect(mot[0], mot[1], mot[2], pointLiaisonBati, pointLiaisonBras, pointCentreEcran, alpha); // Position de depart du systeme

	// Initialisation image temporaire
	imageFondModifiee = imageFond.clone();

	for(int i(0); i < 10000; i++)
	{
		
		imageFond.copyTo(imageFondModifiee(Rect(0, 0, imageFond.cols, imageFond.rows)));

		// Changement de comportement aléatoire
		if(autorisationChangementComportement)
		{	
			// Fonction de changement de comportement
			if(tirageComportementAleatoire(comportementActuel) == 1)
			{
				autorisationChangementComportement = false;
				// Si on change de comportement, on remet les vitesses à 0 progressivement
				descenteVitesse(vit, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements,
					mot, intervalleRaffraichissementAngulaire, M1_MAX, M2_MAX, M3_MAX,
					i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha,
					imageTest, imageTestModifiee, imageFondModifiee);
				// Lorsqu'on est très proche de 0, on impose 0 aux 3 moteurs
				vit[0] = 0;
				vit[1] = 0;
				vit[2] = 0;
		    }
		}
		else
		{ // Boucle d'attente
		    delaisChangementComportement++;

			if(delaisChangementComportement == delaisEntre2Comportements)
		    {
		        delaisChangementComportement = 0;
		        autorisationChangementComportement = true;
			}
		}

		// Comportements
		switch (comportementActuel)
		{
		    case Arret:
		    	descenteVitesse(vit, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements,
					mot, intervalleRaffraichissementAngulaire, M1_MAX, M2_MAX, M3_MAX,
					i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha,
					imageTest, imageTestModifiee, imageFondModifiee);
				break;

			case EcranPivot_Positif:
			
		    	if(vit[2] < V3_MAX || mot[2] < M3_MAX) 
		        	{
		        		monteeVitesse(vit, 2, 1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire, 
		        			 M1_MAX, M2_MAX, M3_MAX, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
			        }
					//cout << " Comportement 1 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		    	
		    	break;

		    case EcranPivot_Negatif:

				if(vit[2] > V3_MIN || mot[2] > M3_MIN) 
		          	{	
		          		monteeVitesse(vit, 2, -1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		          			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		           	}
				//cout << " Comportement 2 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				break;

		    case PetitsCercles_Positif:

				if(vit[1] < V2_MAX || mot[1] < M2_MAX) 
		       		{	
		       			monteeVitesse(vit, 1, 1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN,delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		       				 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		       		}
				//cout << " Comportement 3 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case PetitsCercles_Negatif:

		        if(vit[1] > V2_MIN || mot[1] > M2_MIN) 
		          	{	
		          		monteeVitesse(vit, 1, -1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN,delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		          			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		           	}
		        //cout << " Comportement 4 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandsCercles_Positif:

		        if(vit[0] < V1_MAX) 
		         	{	
		         		monteeVitesse(vit, 0, 1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		         			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);        
		           	}
		        //cout << " Comportement 5 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandsCercles_Negatif:
		        
		        if(vit[0] > V1_MIN || mot[0] > M1_MIN) 
		          	{	
		          		monteeVitesse(vit, 0, -1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		          			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		           	}
		        //cout << " Comportement 6 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case MouvementsAmples_PivotRapide_Positif:

		        if(vit[0] < V1_MAX || mot[0] < M1_MAX) 
		          	{	
		          		monteeVitesse(vit, 0, 1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		          			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
	           		}
		        if(vit[1] < V2_MAX || mot[1] < M2_MAX) 
		           	{	
		           		monteeVitesse(vit, 1, 1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		           			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
			        }
		        if(vit[2] < V3_MAX || mot[2] < M3_MAX) 
		           	{	
		           		monteeVitesse(vit, 2, 1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		           			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
			        }
		        //cout << " Comportement 7 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case MouvementsAmples_PivotRapide_Negatif:
 
		        if(vit[0] > V1_MIN || mot[0] > M1_MIN) 
		          	{	
		          		monteeVitesse(vit, 0, -1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		          			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		           	}
		        if(vit[1] > V2_MIN || mot[1] > M2_MIN) 
		           	{	
		           		monteeVitesse(vit, 1, -1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		           			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		           	}
		        if(vit[2] > V3_MIN || mot[2] > M3_MIN) 
		           	{	
		           		monteeVitesse(vit, 2, -1, intervalleEntre2Impulsions, V1_MAX, V2_MAX, V3_MAX,  V1_MIN, V2_MIN, V3_MIN, delaisEntre2Comportements, mot, intervalleRaffraichissementAngulaire,
		           			 M1_MAX-5, M2_MAX-5, M3_MAX-5, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		           	}
		       	//cout << " Comportement 8 => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;	
		        break;

		    default : break;

		    }

		    affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
		    
		    }
	return;
}
