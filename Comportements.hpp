/** Projet Ecrans Agites : Choix d'un comportement **/

#ifndef COMPORTEMENTS_HPP
#define COMPORTEMENTS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <ctime>

using namespace std;

enum Comportement {Arret, EcranPivot_Positif, EcranPivot_Negatif, PetitsCercles_Positif, PetitsCercles_Negatif,
                   GrandsCercles_Positif, GrandsCercles_Negatif, MouvementsAmples_PivotRapide_Positif, MouvementsAmples_PivotRapide_Negatif,
                    PetitesOscillations, GrandesOscillations, MouvementsAmples_PivotOscillant};


/* tirageComportementAleatoire()
 * Tire aleatoirement un changement de comportement du système parmis ceux de la boite à état.
 * Renvoi 1 si il y a eu un changement, 0 sinon
 */
int tirageComportementAleatoire(enum Comportement& comportementActuel);

#endif