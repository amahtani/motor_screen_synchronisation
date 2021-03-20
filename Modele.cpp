/** Projet Ecrans Agites : modèle géométrique et affichage **/

#define L1 135
#define L2 28
#define PI 3.14159265358979323846

#include "Modele.hpp"

/**
Effectue la rotation de l'image en fonction de l'angle alpha envoye
Arguments :
    Mat& src : image originale a modifier
    double angle : angle en degres 
    Mat& dst : image de destination
 **/
void rotate(Mat& src, double angle, Mat& dst)
{
    Point2f ptCp(src.cols*0.5, src.rows*0.5);
    Mat M = getRotationMatrix2D(ptCp, angle, 1.0);
    warpAffine(src, dst, M, src.size(), INTER_CUBIC);
}


/**
Calcul du modèle géométrique direct, fonction des angles envoyes aux moteurs   
Arguments :
    double m1 : position angulaire envoyee au moteur 1
    double m2 : position angulaire envoyee au moteur 2
    double m3 : position angulaire envoyee au moteur 3
    Point pointLiaisonBati : point du centre de la fenetre
    Point pointCentreEcran : point du centre de l'ecran (rectangle blanc dans la simulation)
    Point pointLiaisonBras : point de liaison entre les 2 bras 
    double alpha : angle du dernier moteur par rapport au referentiel de la base (somme des positions angulaires des 3 moteurs)
 **/
void modelGeometriqueDirect(double m1, double m2, double m3, Point pointLiaisonBati, Point& pointLiaisonBras, Point& pointCentreEcran, double& alpha)
{
    // Point liaison bras
    pointLiaisonBras.x = pointLiaisonBati.x + L1*cos(m1);
    pointLiaisonBras.y = pointLiaisonBati.y + L1*sin(m1);

    // Point centre ecran
    pointCentreEcran.x = pointLiaisonBati.x + L1*cos(m1) + L2*cos(m1+m2);
    pointCentreEcran.y = pointLiaisonBati.y + L1*sin(m1) + L2*sin(m1+m2);
    alpha = (m1+m2+m3);
}

/**
Fonction d'affichage de la simulation sous OpenCV, fonction des angles envoyes aux moteurs   
Arguments :
    int i : indice de fermeture de la fenetre
    Point pointLiaisonBati : point du centre de la fenetre
    Point pointCentreEcran : point du centre de l'ecran (rectangle blanc dans la simulation)
    Point pointLiaisonBras : point de liaison entre les 2 bras 
    double alpha : angle du dernier moteur par rapport au referentiel de la base (somme des positions angulaires des 3 moteurs)
    Mat imageTest : image source avant la rotation
    Mat imageTestModifiee : image apres rotation d'un angle alpha
    Mat imageFondModifiee : image de fond actualisee
 **/
void affichage(double* mot, int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha, Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee)
{
    const char* titreFenetre = "Test openCV";
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

    if(waitKey(1) != -1) i = 10000;
}