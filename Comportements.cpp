/** Projet Ecrans Agites : Choix d'un comportement **/

#include "Comportements.hpp"

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
            comportementActuel = EcranPivot_Positif;    // Rotation de l'ecran (moteur 3) dans un sens
            break;

        case 2 :
            comportementActuel = EcranPivot_Negatif;    // Rotation de l'ecran (moteur 3) dans l'autre sens
        	break;

        case 3 :
            comportementActuel = PetitsCercles_Positif; // Rotation du bras fixé à l'écran (moteur 2) dans un sens
            break;

        case 4 :
            comportementActuel = PetitsCercles_Negatif; // Rotation du bras fixé à l'écran (moteur 2) dans l'autre sens
            break;

        case 5 :
            comportementActuel = GrandsCercles_Positif; // Rotation du bras fixé au bati (moteur 1) dans un sens
            break;

        case 6 :
            comportementActuel = GrandsCercles_Negatif; // Rotation du bras fixé au bati (moteur 1) dans l'autre sens
            break;

        /*case 7 :
            comportementActuel = MouvementsAmples_PivotRapide_Positif;  // Rotation des 3 moteurs dans un sens
            break;

        case 8:
            comportementActuel = MouvementsAmples_PivotRapide_Negatif; // Rotation des 3 moteurs dans l'autre sens
            break;
        */
        default :
            return 0; // Pas de changement
            break;
    }

    cout << "\n comportement numéro : " << nbAleatoire << endl;
    return 1; // Il y a eu un changement

}