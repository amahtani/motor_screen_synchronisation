/** Projet Ecrans Agites : asservissement en vitesse des 3 moteurs selon un comportement **/

#ifndef VITESSE_HPP
#define VITESSE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <ctime>

#include "Modele.hpp"
#include "Comportements.hpp"
#include "monteeDescente.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "ros/ros.h"
//#include "std_msgs/String.h"

using namespace std;
using namespace cv;

void asservissementVitesses(double* mot, int delaisEntre2Comportements, int V1_MAX, int V2_MAX, int V3_MAX, int V1_MIN, int V2_MIN, int V3_MIN, 
	double M1_MAX, double M2_MAX, double M3_MAX, double M1_MIN, double M2_MIN, double M3_MIN, 
	double intervalleEntre2Impulsions, double intervalleRaffraichissementAngulaire);

#endif