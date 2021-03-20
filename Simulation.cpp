#include "Simulation.hpp"


#define PI 3.14159265358979323846

// Parametres globaux a definir :
// Vitesses minimales et maximales de chaque moteur
#define V1_MAX 120
#define V1_MIN 80
#define V2_MAX 120
#define V2_MIN 80
#define V3_MAX 60
#define V3_MIN 10

// Positions angulaires en degrés minimales et maximales de chaque moteur
#define M1_MAX 180			
#define M2_MAX 180
#define M3_MAX 180
#define M1_MIN -180
#define M2_MIN -180
#define M3_MIN -180

// Delta d'augmentation de vitesse et d'angle
#define variationVitesse 0.005		
#define variationAngulaire 0.01

// Délais entre chaque changement de comportement
#define DELAIS_CHANGE_COMPORT 10




int main()
{
	double mot[3] = {0, 0, 0};
	
   	asservissementVitesses(mot, DELAIS_CHANGE_COMPORT, V1_MAX, V2_MAX, V3_MAX, V1_MIN, V2_MIN, V3_MIN, 
   			M1_MAX, M2_MAX, M3_MAX, M1_MIN, M2_MIN, M3_MIN, variationVitesse, variationAngulaire);

	return 1;
}
