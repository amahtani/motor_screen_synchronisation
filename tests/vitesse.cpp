#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "ros/ros.h"
//#include "std_msgs/String.h"

#define L1 135
#define L2 28
#define PI 3.14159265358979323846

#define INC_ANGLE_1_2 PI/150 // unité d'incrément d'angle pour les moteurs M1 et M2
#define INC_ANGLE_3 PI // unité d'incrément d'angle pour le moteur M3

#define V1V2_MAX 120
#define V1V2_MIN 80
#define V3_MAX 60
#define V3_MIN 10
#define dt 0.00001

#define DELAIS_OSCI_COURT 20
#define DELAIS_OSCI_LONG 40
#define DELAIS_MOUV_AMPLES 40

#define DELAIS_CHANGE_COMPORT 10

enum MoteurNum {M1, M2, M3};    // positions angulaires
enum Comportement {Arret, EcranPivot_Positif, EcranPivot_Negatif, PetitsCercles_Positif, PetitsCercles_Negatif,
                   GrandsCercles_Positif, GrandsCercles_Negatif, MouvementsAmples_PivotRapide_Positif, MouvementsAmples_PivotRapide_Negatif,
                    PetitesOscillations, GrandesOscillations, MouvementsAmples_PivotOscillant};

using namespace std;
using namespace cv;


void modelGeometriqueDirect(double m1, double m2, double m3,
                                Point pointLiaisonBati, Point& pointLiaisonBras, Point& pointCentreEcran, double& alpha)
{
    // Point liaison bras
    pointLiaisonBras.x = pointLiaisonBati.x + L1*cos(m1);
    pointLiaisonBras.y = pointLiaisonBati.y + L1*sin(m1);

    // Point centre ecran
    pointCentreEcran.x = pointLiaisonBati.x + L1*cos(m1) + L2*cos(m1+m2);
    pointCentreEcran.y = pointLiaisonBati.y + L1*sin(m1) + L2*sin(m1+m2);
    alpha = m1+m2+m3;
}

void rotate(Mat& src, double angle, Mat& dst){
    Point2f ptCp(src.cols*0.5, src.rows*0.5);
    Mat M = getRotationMatrix2D(ptCp, angle, 1.0);
    warpAffine(src, dst, M, src.size(), INTER_CUBIC); //Nearest is too rough,
}


/* tirageComportementAleatoire()
 * Tire aleatoirement un changement de comportement du système parmis ceux de la boite à état.
 * Renvoi 1 si il y a eu un changement, 0 sinon
 */

int tirageComportementAleatoire(enum Comportement& comportementActuel)
{
    int nbAleatoire = rand()% 130;

    switch (nbAleatoire)
    {

        case 1 :
            comportementActuel = EcranPivot_Positif;
            break;

        case 2 :
            comportementActuel = EcranPivot_Negatif;
        	break;

        case 3 :
            comportementActuel = PetitsCercles_Positif;
            break;

        case 4 :
            comportementActuel = PetitsCercles_Negatif;
            break;

        case 5 :
            comportementActuel = GrandsCercles_Positif;
            break;

        case 6 :
            comportementActuel = GrandsCercles_Negatif;
            break;

        case 7 :
            comportementActuel = MouvementsAmples_PivotRapide_Positif;
            break;

        case 8:
            comportementActuel = MouvementsAmples_PivotRapide_Negatif;
            break;

        /*case 9:
            comportementActuel = PetitesOscillations;
            break;

        case 10:
            comportementActuel = GrandesOscillations;
            break;

        case 11:
           comportementActuel = MouvementsAmples_PivotOscillant;
           break;*/

        default :
            return 0; // Pas de changement
            break;
    }

    cout << "\n comportement : " << nbAleatoire << endl;
    return 1; // Il y a eu un changement

}

double* montee(double* vit, int vit_modifs, int signe)
{
	if(vit_modifs == 2)					// si on fait accelerer le 3eme moteur
	{
		if(vit[2] == 0 && signe > 0) vit[2] = dt;
		if(vit[2] == 0 && signe < 0) vit[2] = -dt;

		int i = 0;
		if(signe > 0)																				// si on veut monter a 1
		{
			while((vit[2] < V3_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[2] > 0) vit[2] = vit[2] + vit[2]*dt;								// si la vitesse est negative
				else if(vit[2] < 0){ vit[2] = vit[2] - vit[2]*dt;}						// si la vitesse est negative
				i++;
			}
		}
		if(signe < 0)																				// si on veut descendre a -1
		{
			while((vit[2] > -V3_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[2] > 0) vit[2] = vit[2] - vit[2]*dt;								// si la vitesse est negative
				else if(vit[2] < 0) vit[2] = vit[2] + vit[2]*dt;					// si la vitesse est negative
				i++;
			}
		}
	}
	else
	{
		if(vit[vit_modifs] == 0 && signe > 0) vit[vit_modifs] = dt;
		if(vit[vit_modifs] == 0 && signe < 0) vit[vit_modifs] = -dt;
		
		int i = 0;
		if(signe > 0)																				// si on veut monter a 1
		{
			while((vit[vit_modifs] < V1V2_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[vit_modifs] > 0) vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*dt;								// si la vitesse est negative
				else if(vit[vit_modifs] < 0){ vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*dt;}						// si la vitesse est negative
				i++;
			}
		}
		if(signe < 0)																				// si on veut descendre a -1
		{
			while((vit[vit_modifs] > -V1V2_MAX) || (i < DELAIS_CHANGE_COMPORT/4))
			{
				if(vit[vit_modifs] > 0) vit[vit_modifs] = vit[vit_modifs] - vit[vit_modifs]*dt;								// si la vitesse est negative
				else if(vit[vit_modifs] < 0) vit[vit_modifs] = vit[vit_modifs] + vit[vit_modifs]*dt;					// si la vitesse est negative
				i++;
			}
		}
	}	
		cout << "montee => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		return vit;
}

double* descente(double* vit)
{
	cout << "descente => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;

	if((vit[0] < V1V2_MIN) && (vit[1] < V1V2_MIN) && (vit[2] < V3_MIN)) return vit;

	while(vit[0] > dt || vit[0] < -dt)
	{
		if(vit[0] > dt) vit[0] = vit[0] - vit[0]*dt;
		if(vit[0] < -dt) vit[0] = vit[0] - vit[0]*dt;
	}

	while(vit[1] > dt || vit[1] < -dt)
	{
		if(vit[1] > dt) vit[1] = vit[1] - vit[1]*dt;
		if(vit[1] < -dt) vit[1] = vit[1] - vit[1]*dt;
	}

	while(vit[2] > dt || vit[2] < -dt)
	{
		if(vit[2] > dt) vit[2] = vit[2] - vit[2]*dt;
		if(vit[2] < -dt) vit[2] = vit[2] - vit[2]*dt;

	}

	cout << "descente => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;

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


int main (int argc, char *argv[])
{
		/* Paramètres */
	Mat imageTest;
	Mat imageTestModifiee;
	Mat imageFond;
	Mat imageFondModifiee;

	const int largeurFenetre(600);
	const int hauteurFenetre(600);

	double m1(0), m2(-PI/2), m3(0); // Angles moteurs
	double vit[3] = {0, 0, 0}; 		  // vitesses moteurs
	double alpha (0); // Angle de l'ecran par rapport a R0

	int delaisOscillations (0);
	int delaisMouvContinu (DELAIS_MOUV_AMPLES);
	int delaisChangementComportement (0);
	bool autorisationChangementComportement(true);
	int indIncrementMouvAmples1, indIncrementMouvAmples2;

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
		return -1;
	}

	imageFond = imread(adresseImageFond, 1);
	if(!imageFond.data)
	{
		cout <<  "Could not open or find the image" << endl ;
		return -1;
	}


	namedWindow(titreFenetre, WINDOW_NORMAL); // WINDOW_NORMAL = taille de fenetre modifiable
	resizeWindow(titreFenetre, largeurFenetre, hauteurFenetre);
	moveWindow(titreFenetre, 100, 100);


	// Position des points de liaisons du systeme
	pointLiaisonBati = Point(imageFond.cols/2,imageFond.rows/2); // Attache au bati
	modelGeometriqueDirect(m1, m2, m3, pointLiaisonBati, pointLiaisonBras, pointCentreEcran, alpha); // Position de depart du systeme

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
				descente(vit);

		    	vit[0] = 0;
				vit[1] = 0;
				vit[2] = 0;

		        cout << "arret => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		    }
		}
		else
		{ // Boucle d'attente
		    delaisChangementComportement++;

			if(delaisChangementComportement == DELAIS_CHANGE_COMPORT)
		    {
		        delaisChangementComportement = 0;
		        autorisationChangementComportement = true;
		    }
		}

		// Comportement
		switch (comportementActuel)
		{
		    case Arret:
				vit[0] = 0;
				vit[1] = 0;
				vit[2] = 0;
				break;

			case EcranPivot_Positif:

		    	if(vit[2] < V3_MAX) 
		        	{
		        		montee(vit, 2, 1);
			           	m3 += vit[2]*dt;
			        }
					if(vit[2] > 1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		    	
		    	break;

		    case EcranPivot_Negatif:

		        if(vit[2] > -V3_MAX) 
		          	{	
		          		montee(vit, 2, -1);
		           		m3 -= vit[2]*dt;
		           	}
				if(vit[2] < -1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
				break;

		    case PetitsCercles_Positif:

				if(vit[1] < V1V2_MAX) 
		       		{	
		       			montee(vit, 1, 1);
		           		m2 += vit[1]*dt;}
				if(vit[1] > 1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case PetitsCercles_Negatif:

		        if(vit[1] > -V1V2_MAX) 
		          	{	montee(vit, 1, -1);
		           		m2 -= vit[1]*dt;
		           	}
		        if(vit[1] < -1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandsCercles_Positif:

		        if(vit[0] < V1V2_MAX) 
		         	{	
		         		montee(vit, 0, 1);
		           		m1 += vit[0]*dt;}
		        if(vit[0] > 1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandsCercles_Negatif:
		        
		        if(vit[0] > -V1V2_MAX) 
		          	{	
		          		montee(vit, 0, -1);
		           		m1 -= vit[0]*dt;
		           	}
		        if(vit[0] < -1) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case MouvementsAmples_PivotRapide_Positif:

		        if(vit[0] < V1V2_MAX) 
		          	{	
		          		montee(vit, 0, 1);
		           		m1 += vit[0]*dt;}
		        if(vit[1] < V1V2_MAX) 
		           	{	
		           		montee(vit, 1, 1);
		           		m2 += vit[1]*dt;}
		        if(vit[2] < V3_MAX) 
		           	{	
		           		montee(vit, 2, 1);
		           		m3 += vit[2]*dt;}
		        if((vit[0] > 1) || (vit[1] > 1) || (vit[2] > 1)) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case MouvementsAmples_PivotRapide_Negatif:

		        if(vit[0] > -V1V2_MAX) 
		          	{	
		          		montee(vit, 0, -1);
		           		m1 -= vit[0]*dt;
		           	}
		        if(vit[1] > -V1V2_MAX) 
		           	{	
		           		montee(vit, 1, -1);
		           		m2 -= vit[1]*dt;
		           	}
		        if(vit[2] > -V3_MAX) 
		           	{	
		           		montee(vit, 2, -1);
		           		m3 -= vit[2]*dt;
		           	}
		       	if((vit[0] < -1) || (vit[1] < -1) || (vit[2] < -1)) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;	
		        break;

		    case PetitesOscillations:

		        if (delaisOscillations < DELAIS_OSCI_COURT && delaisOscillations >= 0)
		        {
		        	//descente(vit);
		            if(vit[1] < V1V2_MAX) 
		              	{
		              		montee(vit, 1, 1);
		               		m2 += vit[1]*dt;
		               	}
		            delaisOscillations++;
	            }
		        if(delaisOscillations > - DELAIS_OSCI_COURT && delaisOscillations < 0)
		        {
		        	descente(vit);
		            if(vit[1] > -V1V2_MAX) 
		              	{	
		              		cout << "error" << endl;
		              		montee(vit, 1, -1);
		               		m2 -= vit[1]*dt;
		               	}
		            delaisOscillations--;
		        }
		        if (delaisOscillations == DELAIS_OSCI_COURT) delaisOscillations = -1;
		        if (delaisOscillations == -DELAIS_OSCI_COURT) delaisOscillations = 1;

	           	cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    case GrandesOscillations:

		        if ( delaisOscillations < DELAIS_OSCI_LONG && delaisOscillations >= 0 )
		        {
		        	descente(vit);
		            if(vit[0] < V1V2_MAX) 
		              	{	
		              		montee(vit, 0, 1);
		               		m1 += vit[0]*dt;
		               	}

		            delaisOscillations++;
		        }
		        if( delaisOscillations > - DELAIS_OSCI_LONG && delaisOscillations < 0 )
		        {
		        	descente(vit);
		            if(vit[0] > -V1V2_MAX) 
		             	{	
		             		montee(vit, 0, -1);
		           			m1 -= vit[0]*dt;
		          		}

		            delaisOscillations--;
		        }
		        if (delaisOscillations == DELAIS_OSCI_LONG) delaisOscillations = -1;
		        if (delaisOscillations == -DELAIS_OSCI_LONG) delaisOscillations = 1;

	          	if((vit[0] > 1) || (vit[0] < -1)) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
			    break;

		    case MouvementsAmples_PivotOscillant:

		    // Pivot
		    	if ( delaisOscillations < DELAIS_OSCI_COURT && delaisOscillations >= 0 )
		    	{
			        if(vit[2] < V3_MAX) 
			           	{	
			           		montee(vit, 2, 1);
			       			m3 += vit[2]*dt;}
			        delaisOscillations++;
			    }
		        if( delaisOscillations > - DELAIS_OSCI_COURT && delaisOscillations < 0 )
		        {
		            if(vit[2] > -V3_MAX) 
		              	{	
		              		montee(vit, 2, -1);
		           			m3 -= vit[2]*dt;
		           		}
		            delaisOscillations--;
		        }
		        if (delaisOscillations == DELAIS_OSCI_COURT) delaisOscillations = -1;
		        if (delaisOscillations == -DELAIS_OSCI_COURT) delaisOscillations = 1;

		        // Bras
		        if(delaisMouvContinu == DELAIS_MOUV_AMPLES)
		        { // Changement de mouvement amples
		            delaisMouvContinu = 0; // Réinitialisation
		        }
		        else
		        {
   			        descente(vit);
		            if(vit[0] < V1V2_MAX) 
		              	{	montee(vit, 0, 1);
		          
		           			m1 += vit[0]*dt;}

		           	if(vit[1] < V1V2_MAX) 
		           		{	montee(vit, 1, 1);
		        
		           			m2 += vit[1]*dt;}

		            delaisMouvContinu++;
		        }

	          	if((vit[2] > 1) || (vit[2] < -1) || (vit[0] > 1) || (vit[1] > 1)) cout << " stationnaire => vit1 = " << vit[0] << "; vit2 = " << vit[1] << "; vit3 = " << vit[2] << endl;
		        break;

		    default : break;

		    }

		    // Application
		    modelGeometriqueDirect(m1, m2, m3, pointLiaisonBati, pointLiaisonBras, pointCentreEcran, alpha);

		    // Rotation de l'image de l'ecran
		    rotate(imageTest, alpha, imageTestModifiee);
		    imageTestModifiee.copyTo(imageFondModifiee(Rect(pointCentreEcran.x - imageTestModifiee.cols/2  , pointCentreEcran.y - imageTestModifiee.rows/2, imageTestModifiee.cols, imageTestModifiee.rows)));

		    // Dessin : bras du systeme
		    line(imageFondModifiee, pointLiaisonBati, pointLiaisonBras, Scalar( 255, 255, 255 ), 2, 8);
		    line(imageFondModifiee, pointLiaisonBras, pointCentreEcran, Scalar( 255, 255, 255 ), 2, 8);

		    // Affichage
		    imshow(titreFenetre, imageFondModifiee);

		    if(waitKey(10) != -1) i = 10000;
		    
		    }
	return EXIT_SUCCESS;
}
