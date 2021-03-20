#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define L1 135
#define L2 28
#define PI 3.14159265358979323846

#define INC_ANGLE_1_2 PI/150 // unité d'incrément d'angle pour les moteurs M1 et M2
#define INC_ANGLE_3 PI // unité d'incrément d'angle pour le moteur M3

#define DELAIS_OSCI_COURT 20
#define DELAIS_OSCI_LONG 40
#define DELAIS_MOUV_AMPLES 40

#define DELAIS_CHANGE_COMPORT 60

enum MoteurNum {M1, M2, M3};
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
    int nbAleatoire = rand()% 110;

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

        case 9:
            comportementActuel = PetitesOscillations;
            break;

         case 10:
            comportementActuel = GrandesOscillations;
            break;

         case 11:
           comportementActuel = MouvementsAmples_PivotOscillant;
           break;

        default :
            return 0; // Pas de changement
            break;
    }

    cout << "comportement : " << nbAleatoire << endl;
    return 1; // Il y a eu un changement

}


int main ()
{
    /* Paramètres */
  Mat imageTest;
  Mat imageTestModifiee;
  Mat imageFond;
  Mat imageFondModifiee;

  const int largeurFenetre(600);
  const int hauteurFenetre(600);

  double m1(0), m2(-PI/2), m3(0); // Angles moteurs
  double alpha (0); // Angle de l'ecran par rapport a R0

  int delaisOscillations (0);
  int delaisMouvContinu (DELAIS_MOUV_AMPLES);
  int delaisChangementComportement (0);
  bool autorisationChangementComportement(true);
  int indIncrementMouvAmples1, indIncrementMouvAmples2;

  Point pointCentreEcran, pointLiaisonBras, pointLiaisonBati;

  const char* titreFenetre = "Test openCV";
  const char* adresseImageTest = "/home/mahtani/Bureau/Projet_indus/Simu_Johann/V2/screencut.jpg";
  const char* adresseImageFond = "/home/mahtani/Bureau/Projet_indus/Simu_Johann/V2/fondNoir.png";

  enum Comportement comportementActuel(Arret);


    /* Initialisations */
  imageTest = imread(adresseImageTest, CV_LOAD_IMAGE_COLOR);
  if(! imageTest.data )
  {
       cout <<  "Could not open or find the image 1" << endl ;
       return -1;
  }

  imageFond = imread(adresseImageFond, CV_LOAD_IMAGE_COLOR);
  if(! imageFond.data )
  {
       cout <<  "Could not open or find the image" << endl ;
       return -1;
  }


  namedWindow(titreFenetre, WINDOW_NORMAL ); // WINDOW_NORMAL = taille de fenetre modifiable
  resizeWindow(titreFenetre, largeurFenetre, hauteurFenetre);
  moveWindow(titreFenetre, 100, 100);


  // Position des points de liaisons du systeme
  pointLiaisonBati = Point (imageFond.cols/2,imageFond.rows/2); // Attache au bati
  modelGeometriqueDirect(m1, m2, m3, pointLiaisonBati, pointLiaisonBras, pointCentreEcran, alpha); // Position de depart du systeme

  // Initialisation image temporaire
  imageFondModifiee = imageFond.clone();

  for(int i(0); i < 10000 ; i++)
  {
    imageFond.copyTo(imageFondModifiee(Rect(0, 0, imageFond.cols, imageFond.rows)));

    // Changement de comportement aléatoire
    if( autorisationChangementComportement )
    {
        if( tirageComportementAleatoire(comportementActuel) == 1 )
        {
            autorisationChangementComportement = false;
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
            break;

        case EcranPivot_Positif:
            m3 += INC_ANGLE_3;
            break;

        case EcranPivot_Negatif:
            m3 -= INC_ANGLE_3;
            break;

        case PetitsCercles_Positif:
            m2 += 5*INC_ANGLE_1_2;
            break;

        case PetitsCercles_Negatif:
            m2 -= 5*INC_ANGLE_1_2;
            break;

        case GrandsCercles_Positif:
            m1 += 1.5*INC_ANGLE_1_2;
            break;

        case GrandsCercles_Negatif:
            m1 -= 1.5*INC_ANGLE_1_2;
            break;

        case MouvementsAmples_PivotRapide_Positif:
            m1 += INC_ANGLE_1_2;
            m2 += INC_ANGLE_1_2;
            m3 -= INC_ANGLE_3;
            break;

        case MouvementsAmples_PivotRapide_Negatif:
            m1 -= INC_ANGLE_1_2;
            m2 -= INC_ANGLE_1_2;
            m3 += INC_ANGLE_3;
            break;

        case PetitesOscillations:
            if ( delaisOscillations < DELAIS_OSCI_COURT && delaisOscillations >= 0 )
            {
                 m2 += INC_ANGLE_1_2;
                 delaisOscillations++;
            }
            if( delaisOscillations > - DELAIS_OSCI_COURT && delaisOscillations < 0 )
            {
                m2 -= INC_ANGLE_1_2;
                delaisOscillations--;
            }
            if (delaisOscillations == DELAIS_OSCI_COURT) delaisOscillations = -1;
            if (delaisOscillations == -DELAIS_OSCI_COURT) delaisOscillations = 1;

            break;

        case GrandesOscillations:
            if ( delaisOscillations < DELAIS_OSCI_LONG && delaisOscillations >= 0 )
            {
                 m1 += 1.5*INC_ANGLE_1_2;
                 delaisOscillations++;
            }
            if( delaisOscillations > - DELAIS_OSCI_LONG && delaisOscillations < 0 )
            {
                m1 -= 1.5*INC_ANGLE_1_2;
                delaisOscillations--;
            }
            if (delaisOscillations == DELAIS_OSCI_LONG) delaisOscillations = -1;
            if (delaisOscillations == -DELAIS_OSCI_LONG) delaisOscillations = 1;

            break;

        case MouvementsAmples_PivotOscillant:

            // Pivot
            if ( delaisOscillations < DELAIS_OSCI_COURT && delaisOscillations >= 0 )
            {
                 m3 += INC_ANGLE_3;;
                 delaisOscillations++;
            }
            if( delaisOscillations > - DELAIS_OSCI_COURT && delaisOscillations < 0 )
            {
                m3 -= INC_ANGLE_3;
                delaisOscillations--;
            }
            if (delaisOscillations == DELAIS_OSCI_COURT) delaisOscillations = -1;
            if (delaisOscillations == -DELAIS_OSCI_COURT) delaisOscillations = 1;

            // Bras
            if(delaisMouvContinu == DELAIS_MOUV_AMPLES)
            { // Changement de mouvement amples
                indIncrementMouvAmples1 = (rand()%3) - 1; // Tirage aleatoire increments des moteurs
                indIncrementMouvAmples2 = (rand()%3) - 1;

                delaisMouvContinu = 0; // Réinitialisation
            }
            else
            {
                m1 += indIncrementMouvAmples1*INC_ANGLE_1_2;
                m2 += indIncrementMouvAmples2*INC_ANGLE_1_2;

                delaisMouvContinu++;
            }

            break;

        default:
            break;
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
    imshow(titreFenetre, imageFondModifiee );
    if( cvWaitKey(10) != -1)
        i = 10000;

    }


  return EXIT_SUCCESS;

}
