#include "Vitesse.hpp"

#define PI 3.14159265358979323846

//#define INC_ANGLE_1_2 PI/150 // unité d'incrément d'angle pour les moteurs mot[0] et mot[1]
//#define INC_ANGLE_3 PI // unité d'incrément d'angle pour le moteur mot[2]


double* montee(double* vit, int vit_modifs, int signe, double intervalleEntre2Impulsions, int V1V2_MAX, int V3_MAX, int DELAIS_CHANGE_COMPORT)
{
	if(vit_modifs == 2)					// si on fait accelerer le 3eme moteur
	{
		if(vit[2] == 0 && signe > 0) vit[2] = intervalleEntre2Impulsions;
		if(vit[2] == 0 && signe < 0) vit[2] = -intervalleEntre2Impulsions;

		int i = 0;
		if(signe > 0)																				// si on veut monter a 1
		{
			while((vit[2] < V3_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[2] > 0) vit[2] = vit[2] + vit[2]*intervalleEntre2Impulsions;								// si la vitesse est negative
				else if(vit[2] < 0){ vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions;}						// si la vitesse est negative
				i++;
			}
		}
		if(signe < 0)																				// si on veut descendre a -1
		{
			while((vit[2] > -V3_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[2] > 0) vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions;								// si la vitesse est negative
				else if(vit[2] < 0) vit[2] = vit[2] + vit[2]*intervalleEntre2Impulsions;					// si la vitesse est negative
				i++;
			}
		}
	}
	else
	{
		if(vit[vit_modifs] == 0 && signe > 0) vit[vit_modifs] = intervalleEntre2Impulsions;
		if(vit[vit_modifs] == 0 && signe < 0) vit[vit_modifs] = -intervalleEntre2Impulsions;
		
		int i = 0;
		if(signe > 0)																				// si on veut monter a 1
		{
			while((vit[vit_modifs] < V1V2_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[vit_modifs] > 0) vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*intervalleEntre2Impulsions;								// si la vitesse est negative
				else if(vit[vit_modifs] < 0){ vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*intervalleEntre2Impulsions;}						// si la vitesse est negative
				i++;
			}
		}
		if(signe < 0)																				// si on veut descendre a -1
		{
			while((vit[vit_modifs] > -V1V2_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[vit_modifs] > 0) vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*intervalleEntre2Impulsions;								// si la vitesse est negative
				else if(vit[vit_modifs] < 0) vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*intervalleEntre2Impulsions;					// si la vitesse est negative
				i++;
			}
		}
	}	
		cout << "montee => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		return vit;
}



double* descente(double* vit, double intervalleEntre2Impulsions, int V1V2_MAX, int V3_MAX, int V1V2_MIN, int V3_MIN, int DELAIS_CHANGE_COMPORT)
{
	cout << "descente => vit 1 = " << vit[0] << "; vit 2 = " << vit[1] << "; vit 3 = " << vit[2] << endl;

	if((vit[0] < V1V2_MIN) && (vit[1] < V1V2_MIN) && (vit[2] < V3_MIN)) return vit;

	while(vit[0] > intervalleEntre2Impulsions || vit[0] < -intervalleEntre2Impulsions)
	{
		if(vit[0] > intervalleEntre2Impulsions) vit[0] = vit[0] - vit[0]*intervalleEntre2Impulsions;
		if(vit[0] < -intervalleEntre2Impulsions) vit[0] = vit[0] - vit[0]*intervalleEntre2Impulsions;
	}

	while(vit[1] > intervalleEntre2Impulsions || vit[1] < -intervalleEntre2Impulsions)
	{
		if(vit[1] > intervalleEntre2Impulsions) vit[1] = vit[1] - vit[1]*intervalleEntre2Impulsions;
		if(vit[1] < -intervalleEntre2Impulsions) vit[1] = vit[1] - vit[1]*intervalleEntre2Impulsions;
	}

	while(vit[2] > intervalleEntre2Impulsions || vit[2] < -intervalleEntre2Impulsions)
	{
		if(vit[2] > intervalleEntre2Impulsions) vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions;
		if(vit[2] < -intervalleEntre2Impulsions) vit[2] = vit[2] - vit[2]*intervalleEntre2Impulsions;
	}

	cout << "descente => vit 1 = " << vit[0] << "; vit 2 = " << vit[1] << "; vit 3 = " << vit[2] << endl;

	return vit;
}

/*void writeI2C()
{
	while (ros::OK())
	{
		i2c_chatter_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
}*/


void gestionVitesses(int delaisEntre2Comportements, int V1V2_MAX, int V3_MAX, int V1V2_MIN, int V3_MIN,
double M1_MAX, double M2_MAX, double M3_MAX, double intervalleEntre2Impulsions)
{		/* Paramètres */
	Mat imageTest;
	Mat imageTestModifiee;
	Mat imageFond;
	Mat imageFondModifiee;

	const int largeurFenetre(600);
	const int hauteurFenetre(600);

	//double mot[0](0), mot[1](-PI/2), mot[2](0); // Angles moteurs
	double mot[3] = {0, -PI/2, 0};
	double vit[3] = {0, 0, 0}; 		  // vitesses moteurs
	double alpha (0); // Angle de l'ecran par rapport a R0

	int delaisOscillations (0);
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


	namedWindow(titreFenetre, WINDOW_NORMAL); // WINDOW_NORMAL = taille de fenetre modifiable
	resizeWindow(titreFenetre, largeurFenetre, hauteurFenetre);
	moveWindow(titreFenetre, 100, 100);


	// Position des points de liaisons du systeme
	pointLiaisonBati = Point(imageFond.cols/2,imageFond.rows/2); // Attache au bati
	modelGeometriqueDirect(mot[0], mot[1], mot[2], pointLiaisonBati, pointLiaisonBras, pointCentreEcran, alpha); // Position de depart du systeme

	// Initialisation image temporaire
	imageFondModifiee = imageFond.clone();

    cout << "debut => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;

	for(int i(0); i < 10000; i++)
	{
		
		imageFond.copyTo(imageFondModifiee(Rect(0, 0, imageFond.cols, imageFond.rows)));

		// Changement de comportement aléatoire
		if(autorisationChangementComportement)
		{
			if(tirageComportementAleatoire(comportementActuel) == 1)
			{
				autorisationChangementComportement = false;
				descente(vit, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, V1V2_MIN, V3_MIN, delaisEntre2Comportements);
				descenteAngulaire(mot, intervalleEntre2Impulsions);

		    	vit[0] = 0;
				vit[1] = 0;
				vit[2] = 0;
				mot[0] = 0;
				mot[1] = -PI/2;
				mot[2] =  0;

		        cout << "arret => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
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

		// Comportement
		switch (comportementActuel)
		{
		    case Arret:
				descente(vit, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, V1V2_MIN, V3_MIN, delaisEntre2Comportements);
				descenteAngulaire(mot, intervalleEntre2Impulsions);
				//descente(mot, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, V1V2_MIN, V3_MIN, delaisEntre2Comportements);
				break;

			case EcranPivot_Positif:

		    	if(vit[2] < V3_MAX) 
		        	{
		        		montee(vit, 2, 1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		        		monteeAngulaire(mot, 2, 1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
			           	//mot[2] += vit[2]*intervalleEntre2Impulsions;
			        }
					if(vit[2] > 1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		    	
		    	break;

		    case EcranPivot_Negatif:

		        if(vit[2] > -V3_MAX) 
		          	{	
		          		montee(vit, 2, -1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		          		monteeAngulaire(mot, 2, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[2] -= vit[2]*intervalleEntre2Impulsions;
		           	}
				cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				break;

		    case PetitsCercles_Positif:

				if(vit[1] < V1V2_MAX) 
		       		{	
		       			montee(vit, 1, 1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		       			monteeAngulaire(mot, 1, 1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[1] += vit[1]*intervalleEntre2Impulsions;}
				cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case PetitsCercles_Negatif:

		        if(vit[1] > -V1V2_MAX) 
		          	{	montee(vit, 1, -1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		          		monteeAngulaire(mot, 1, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[1] -= vit[1]*intervalleEntre2Impulsions;
		           	}
		        cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandsCercles_Positif:

		        if(vit[0] < V1V2_MAX) 
		         	{	
		         		montee(vit, 0, 1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 0, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[0] += vit[0]*intervalleEntre2Impulsions;
		           	}
		        cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandsCercles_Negatif:
		        
		        if(vit[0] > -V1V2_MAX) 
		          	{	
		          		montee(vit, 0, -1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		          		monteeAngulaire(mot, 0, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[0] -= vit[0]*intervalleEntre2Impulsions;
		           	}
		        cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case MouvementsAmples_PivotRapide_Positif:

		        if(vit[0] < V1V2_MAX) 
		          	{	
		          		montee(vit, 0, 1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 0, 1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[0] += vit[0]*intervalleEntre2Impulsions;
		           	}
		        if(vit[1] < V1V2_MAX) 
		           	{	
		           		montee(vit, 1, 1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 1, 1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[1] += vit[1]*intervalleEntre2Impulsions;
		           	}
		        if(vit[2] < V3_MAX) 
		           	{	
		           		montee(vit, 2, 1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 2, 1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);}
		           		//mot[2] += vit[2]*intervalleEntre2Impulsions;
		           	}
		        cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case MouvementsAmples_PivotRapide_Negatif:

		        if(vit[0] > -V1V2_MAX) 
		          	{	
		          		montee(vit, 0, -1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 0, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[0] -= vit[0]*intervalleEntre2Impulsions;
		           	}
		        if(vit[1] > -V1V2_MAX) 
		           	{	
		           		montee(vit, 1, -1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 1, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[1] -= vit[1]*intervalleEntre2Impulsions;
		           	}
		        if(vit[2] > -V3_MAX) 
		           	{	
		           		montee(vit, 2, -1, intervalleEntre2Impulsions, V1V2_MAX, V3_MAX, delaisEntre2Comportements);
		           		monteeAngulaire(mot, 2, -1, intervalleEntre2Impulsions, M1_MAX, M2_MAX, M3_MAX);
		           		//mot[2] -= vit[2]*intervalleEntre2Impulsions;
		           	}
		       	cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;	
		        break;

		    default : break;

		    }

		    // Application
		    modelGeometriqueDirect(mot[0], mot[1], mot[2], pointLiaisonBati, pointLiaisonBras, pointCentreEcran, alpha);

		    // Rotation de l'image de l'ecran
		    rotate(imageTest, alpha, imageTestModifiee);
		    imageTestModifiee.copyTo(imageFondModifiee(Rect(pointCentreEcran.x - imageTestModifiee.cols/2, pointCentreEcran.y - imageTestModifiee.rows/2, imageTestModifiee.cols, imageTestModifiee.rows)));

		    // Dessin : bras du systeme
		    line(imageFondModifiee, pointLiaisonBati, pointLiaisonBras, Scalar( 255, 255, 255 ), 2, 8);
		    line(imageFondModifiee, pointLiaisonBras, pointCentreEcran, Scalar( 255, 255, 255 ), 2, 8);

		    // Affichage
		    imshow(titreFenetre, imageFondModifiee);

		    if(waitKey(10) != -1) i = 10000;
		    
		    }
	return;
}
