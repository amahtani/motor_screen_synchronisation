
double* descenteAngulaire(double* mot, double intervalleEntre2Impulsions, double M1_MIN, double M2_MIN, double M3_MIN,
	int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha,
	Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee)
{
	cout << "descente etape 0 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
	
	affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);

	while(mot[0] > intervalleEntre2Impulsions || mot[0] < -intervalleEntre2Impulsions)		// tant que m1 est plus grand ou plus petit que dt 
    {
        if(mot[0] > intervalleEntre2Impulsions) mot[0] = mot[0] - mot[0]*intervalleEntre2Impulsions;	// si m1 trop grand, on le diminue de m1*dt
        if(mot[0] < -intervalleEntre2Impulsions) mot[0] = mot[0] - mot[0]*intervalleEntre2Impulsions;	// si m1 trop petit, on l'augmente de m1*dt
       	cout << "descente 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
       	affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
    }

	while(mot[1] > intervalleEntre2Impulsions || mot[1] < -intervalleEntre2Impulsions)		// tant que m2 est plus grand ou plus petit que dt 
	{
		if(mot[1] > intervalleEntre2Impulsions) mot[1] = mot[1] - mot[1]*intervalleEntre2Impulsions;	// si m2 trop grand, on le diminue de m2*dt
		if(mot[1] < -intervalleEntre2Impulsions) mot[1] = mot[1] - mot[1]*intervalleEntre2Impulsions;	// si m2 trop petit, on l'augmente de m2*dt
		cout << "descente 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
	}

	while(mot[2] > intervalleEntre2Impulsions || mot[2] < -intervalleEntre2Impulsions)		// tant que m3 est plus grand ou plus petit que dt 
	{
		if(mot[2] > intervalleEntre2Impulsions) mot[2] = mot[2] - mot[2]*intervalleEntre2Impulsions;	// si m3 trop grand, on le diminue de m3*dt
		if(mot[2] < -intervalleEntre2Impulsions) mot[2] = mot[2] - mot[2]*intervalleEntre2Impulsions;	// si m3 trop petit, on l'augmente de m3*dt
		cout << "descente 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
		affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
	}

   	affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);

	cout << "descente etape finale => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
	return mot;
}

/*double* monteeAngulaire(double* mot, int mot_modifs, int signe, double intervalleEntre2Impulsions, double M1_MAX, double M2_MAX, double M3_MAX,
	double M1_MIN, double M2_MIN, double M3_MIN,  int delaisEntre2Comportements,
	int i, Point pointLiaisonBati, Point pointCentreEcran, Point pointLiaisonBras, double alpha,
	Mat imageTest, Mat imageTestModifiee, Mat imageFondModifiee)
{
	int cpt = 0;
	if(mot[mot_modifs] == 0 && signe > 0) mot[mot_modifs] = intervalleEntre2Impulsions;
    if(mot[mot_modifs] == 0 && signe < 0) mot[mot_modifs] = -intervalleEntre2Impulsions;

    affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);

    if(mot_modifs == 2)                 // si on fait accelerer le 3eme moteur
    {
  		cout << "montee etape 0 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;

        cpt = 0;
		if(signe > 0)                                                                               // si on veut monter a 1
		{
			while(mot[mot_modifs] < M3_MAX && cpt < delaisEntre2Comportements)
            {
				if(mot[mot_modifs] > 0) mot[mot_modifs] = mot[mot_modifs] + mot[mot_modifs]*intervalleEntre2Impulsions;                             // si la vitesse est negative
                else if(mot[mot_modifs] < 0) mot[mot_modifs] = mot[mot_modifs] - mot[mot_modifs]*intervalleEntre2Impulsions;                     // si la vitesse est negative
                cout << "montee etape 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
                affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
                cpt++;
            }
        }
        if(signe < 0)                                                                               // si on veut descendre a -1
        {
            while((mot[mot_modifs] > M3_MIN) && cpt < delaisEntre2Comportements)
            {
                if(mot[mot_modifs] > 0) mot[mot_modifs] = mot[mot_modifs] - mot[mot_modifs]*intervalleEntre2Impulsions;                             // si la vitesse est negative
                else if(mot[mot_modifs] < 0) mot[mot_modifs] = mot[mot_modifs] + mot[mot_modifs]*intervalleEntre2Impulsions;                    // si la vitesse est negative
                cout << "montee etape 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
                affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
                cpt++;
            }
        }
    }
    else if(mot_modifs == 1)
    {
        cpt = 0;
        if(signe > 0)                                                                               // si on veut monter a 1
        {
            while((mot[mot_modifs] < M2_MAX) && cpt < delaisEntre2Comportements)
            {
                if(mot[mot_modifs] > 0) mot[mot_modifs] = mot[mot_modifs] + mot[mot_modifs]*intervalleEntre2Impulsions;                             // si la vitesse est negative
				else if(mot[mot_modifs] < 0){ mot[mot_modifs] = mot[mot_modifs] - mot[mot_modifs]*intervalleEntre2Impulsions;}                      // si la vitesse est negative
				cout << "montee etape 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
                affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
                cpt++;
            }
        }
        if(signe < 0)                                                                               // si on veut descendre a -1
        {
            while(mot[mot_modifs] > M2_MIN && cpt < delaisEntre2Comportements)
            {
                if(mot[mot_modifs] > 0) mot[mot_modifs] = mot[mot_modifs] - mot[mot_modifs]*intervalleEntre2Impulsions;                             // si la vitesse est negative
                else if(mot[mot_modifs] < 0) mot[mot_modifs] = mot[mot_modifs] + mot[mot_modifs]*intervalleEntre2Impulsions;                    // si la vitesse est negative
                cout << "montee etape 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
                affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
                cpt++;
            }
        }
    }   
    else if(mot_modifs == 0)
    {
        cpt = 0;
        if(signe > 0)                                                                               // si on veut monter a 1
        {
            while(mot[mot_modifs] < M1_MAX && cpt < delaisEntre2Comportements)
            {
                if(mot[mot_modifs] > 0) mot[mot_modifs] = mot[mot_modifs] + mot[mot_modifs]*intervalleEntre2Impulsions;                             // si la vitesse est negative
                else if(mot[mot_modifs] < 0){ mot[mot_modifs] = mot[mot_modifs] - mot[mot_modifs]*intervalleEntre2Impulsions;}                      // si la vitesse est negative
                cout << "montee etape 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
                affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
                cpt++;
            }
        }
        if(signe < 0)                                                                               // si on veut descendre a -1
        {
            while(mot[mot_modifs] > M1_MIN && cpt < delaisEntre2Comportements)
            {
                if(mot[mot_modifs] > 0) mot[mot_modifs] = mot[mot_modifs] - mot[mot_modifs]*intervalleEntre2Impulsions;                             // si la vitesse est negative
                else if(mot[mot_modifs] < 0) mot[mot_modifs] = mot[mot_modifs] + mot[mot_modifs]*intervalleEntre2Impulsions;                    // si la vitesse est negative
                cout << "montee etape 1 => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
                affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);
                cpt++;
            }
        }
    }
    cout << "montee etape finale => mot 1 = " << mot[0] << "; mot 2 = " << mot[1] << "; mot 3 = " << mot[2] << endl;
    affichage(mot, i, pointLiaisonBati, pointCentreEcran, pointLiaisonBras, alpha, imageTest, imageTestModifiee, imageFondModifiee);   
    return mot;
}*/