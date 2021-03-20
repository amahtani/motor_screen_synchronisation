/** Projet Ecrans Agites : modèle géométrique et affichage **/

#ifndef MODELE_HPP
#define MODELE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//#include "Vitesse.hpp"
//#include "Comportements.hpp"

void rotate(Mat& src, double angle, Mat& dst);
void modelGeometriqueDirect(double m1, double m2, double m3, Point pointLiaisonBati, Point& pointLiaisonBras, Point& pointCentreEcran, double& alpha);

void affichage(double* mot, int i, Point LiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha, 
				Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee);

#endif