#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <SDL2/SDL.h>        
#include <SDL2/SDL_image.h>      
#include <unistd.h>
//#include <SDL2/SDL/SDL_getenv.h>



int main()
{
    srand(time(NULL));
    int m1 = 1;
    int m2 = 1; 
    int m3 = 1;                 //positions angulaires des 3 moteurs
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Modélisation du mouvement de l'écran",1100,300, 700, 700, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    int x = 1;
    int y = 1;
    int alpha = 1;
    int dx =  1;
    int dy = 1;
    int tmpx, tmpy;
    int l1 = 12; int l2 = 30; // longueurs des 2 axes derriere l'ecran
    int rot_visible;
    int inc_alpha;
    //int p_stop = rand()%26 - 1;

    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); 
    
    //sleep(1);


for(int i = 0; i<1000; i++)

    {
    // Reinitialisation de l'ecran en blanc pour enlever les carres
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer); 

    
    // increments aleatoires des positions moteurs
    int incm1 = rand()%7 - 3;
    int incm2 = rand()%7 - 3;
    int incm3 = rand()%7 - 3;
    //int p_stop = rand()%26 - 1;
  
    // positions angulaires envoyees aux 3 moteurs
    m1 = m1 + incm1;
    m2 = m2 + incm2;
    m3 = m3 + incm3;

    // Affichage des positions moteurs
    //std::cout << "\tBoucle : " << i << " : m1 = " << m1 << " m2 = " << m2 << " m3 = " << m3 << "\n"; 
    //std::cout << "==========================================================\n"; 

    // Ancien modele geometrique
    //tmpx = int(x);
    //tmpy = int(y);
    //std::cout << "\t Boucle : " << i << " : tmpx = " << tmpx << " tmpy = " << tmpy  << "\n"; 
    //std::cout << "==========================================================\n";

    // Modele geometrique direct
    x = l1*cos(m1) + l2*cos(m1+m2);
    y = l1*sin(m1) + l2*sin(m1+m2);
    alpha = m1 + m2 + m3;

    // Affichage des parametres geometriques
    std::cout << "\t Boucle : " << i << " :  x = " << x << " y = " << y << " alpha = " << alpha << "\n"; 
    //std::cout << "==========================================================\n"; 

    // en cas de positions a atteindre = 0
    //if(x==0) x = 1;       // Evite les divisions par 0
    //if(y==0) y = 1;


    // tests
    int inc = 3;
    if(x>0){dx=inc;}//x+=500;}
    else if(x<0) {dx = -inc;}//x-=600;}
    else if(x==0){dx = 0;}


    if(y>0) {dy = inc;}//y+=600;}
    else if(y<0) {dy = -inc;}//y-=600;}
    else if(y==0){dy = 0;}


    std::cout << "\t Boucle : " << i << " :  dx = " << dx << " dy = " << dy  << "\n"; 
    std::cout << "===================================================\n"; 

    inc_alpha = 5;


    if(alpha>0){rot_visible=inc_alpha;} //alpha+=100;}
    else if (alpha<0) {rot_visible=-inc_alpha;} //alpha-=100;}
    else if (alpha == 0) rot_visible = 0;

    for(int k = 0; k < abs(alpha); k++){
			
        		//while(p_stop > 0){
            		
            		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            		SDL_RenderClear(renderer);

            		//rot_visible = alpha+30;

            		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            		SDL_RenderDrawLine(renderer, 200+rot_visible,200,500+rot_visible,200+rot_visible);
            		SDL_RenderDrawLine(renderer, 200+rot_visible,200,200,400);
            		SDL_RenderDrawLine(renderer, 500+rot_visible,200+rot_visible,500,400+rot_visible);
            		SDL_RenderDrawLine(renderer, 200,400,500,400+rot_visible);
            
            		SDL_RenderPresent(renderer);
            		usleep(10000);
            		//p_stop = rand()%26 - 1;
	
        		//}

        	}



    int p_stop = rand()%1000 - 1;

    // Si on fait une rotation de l'ecran avec un angle alpha positif
    //while(p_stop > 0){
        for(int j = 0; j < abs(x); j++){

        	//while(p_stop > 0){
        	if(p_stop > 0){

            	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            	SDL_RenderClear(renderer);

            	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
           		SDL_RenderDrawLine(renderer, 200+dx,200,500+dx,200);
            	SDL_RenderDrawLine(renderer, 200+dx,200,200+dx,400);
            	SDL_RenderDrawLine(renderer, 500+dx,200,500+dx,400);
            	SDL_RenderDrawLine(renderer, 200+dx,400,500+dx,400);

            	SDL_RenderPresent(renderer);
            	usleep(5000);
            	//p_stop = rand()%1000 - 1;
            	}
            p_stop = rand()%1000 - 1;
        	}
        //}

	//while(p_stop > 0){
        for(int l = 0; l < abs(y); l++){
		
        	//while(p_stop > 0){

        		if(p_stop > 0){
            	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            	SDL_RenderClear(renderer);

            	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            	SDL_RenderDrawLine(renderer, 200,200+dy,500,200+dy);
            	SDL_RenderDrawLine(renderer, 200,200+dy,200,400+dy);
            	SDL_RenderDrawLine(renderer, 500,200+dy,500,400+dy);
            	SDL_RenderDrawLine(renderer, 200,400+dy,500,400+dy);
            
            	SDL_RenderPresent(renderer);
            	usleep(5000);
            	//p_stop = rand()%1000 - 1;
            	}
            p_stop = rand()%1000 - 1;
        	}

        //}
    //p_stop = rand()%1000 - 1;
 	
            


           /* for(int k = 0; k < abs(alpha); k++){
			
        		//while(p_stop > 0){
            		
            		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            		SDL_RenderClear(renderer);

            		//rot_visible = alpha+30;

            		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            		SDL_RenderDrawLine(renderer, 200+rot_visible,200,500+rot_visible,200+rot_visible);
            		SDL_RenderDrawLine(renderer, 200+rot_visible,200,200,400);
            		SDL_RenderDrawLine(renderer, 500+rot_visible,200+rot_visible,500,400+rot_visible);
            		SDL_RenderDrawLine(renderer, 200,400,500,400+rot_visible);
            
            		SDL_RenderPresent(renderer);
            		//usleep(2000);
            		//p_stop = rand()%26 - 1;
	
        		//}

        	}*/
    
    
    }
    
    //usleep(10000);
    // On quitte SDL
    SDL_Quit();


    return 0;
}

