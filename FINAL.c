//Library SDL
// sudo apt install build-essential pkg-config libsdl2-dev
#include <SDL2/SDL.h>

#include <time.h>

#include <stdio.h>
#include <stdbool.h>

// Library SDL_TTF
// sudo apt install libsdl2-ttf-dev libsdl-ttf2.0-dev
#include "SDL_ttf.h"

// Library SDL_image
// sudo apt-get install libsdl2-image-dev
#include <SDL_image.h>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 600

// Define rocket dimensions
#define FUSEE_X 300
#define FUSEE_Y 700
#define LONGUEUR_FUSEE 20
#define HAUTEUR_FUSEE 50

// Define Aste dimensions
#define LONGUEUR_ASTE 50
#define HAUTEUR_ASTE 50

// Define rocket movement
#define GAUCHE 5
#define DROITE 5

// Define Aste on the screen
#define NUM_ROWS 10     // Nombre de rangées ou lignes
#define NUM_ASTEROIDS 5 // Nombre d'astéroïdes par rangée
#define VERTICAL_ESPACE 150

int main(int argc, char* argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    srand(time(NULL));
    SDL_Event event;
    int quit = 0;
    int score = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error SDL_INIT\n");
        return 1;
    }

    window = SDL_CreateWindow("BETA 1.0 : Vertical Scrolling", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("SDL window error");
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        printf("SDL renderer error");
        return 1;
    }

    SDL_Rect asteroids[NUM_ROWS][NUM_ASTEROIDS];

        printf("Game start in 10 seconds\n"
                "COMMANDS : \n"
                "<LEFT>/<RIGHT> to move the rocket\n"
                "<q> to quit\n"
                "RULES : \n"
                "STAY INSIDE THE WINDOW\n"
                "Survive as longer as possible.\n"
                "Good luck and have fun!\n");
        SDL_Delay(1000);
    // Initialisation des astéroïdes dans chaque rangée/ligne
    for (size_t row = 0; row < NUM_ROWS; row++) {
        for (size_t i = 0; i < NUM_ASTEROIDS; i++) {
            asteroids[row][i].y = row * VERTICAL_ESPACE; // Espacement vertical entre les lignes
            asteroids[row][i].x = rand() % SCREEN_WIDTH;
            asteroids[row][i].w = rand() % LONGUEUR_ASTE;
            asteroids[row][i].h = rand() % HAUTEUR_ASTE;

        }
    }

    // Declare a square as Rect
        SDL_Rect square;
    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    square.w = LONGUEUR_FUSEE;
    square.h = HAUTEUR_FUSEE;
    // Square position: In the middle of the screen
    square.x = FUSEE_X;
    square.y = FUSEE_Y;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                    printf("Game Over.\n"
                           "Score : %d\n",score);
                    printf("Try Again\n");
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_q) {
                    printf("Game Over.\n"
                           "Score : %d\n",score);
                    printf("Try Again\n");
                    quit = 1;
                }
                 switch(event.key.keysym.sym) {
                        case SDLK_LEFT:                //SDLK_LEFT représente la touche "flèche gauche"
                                square.x -= GAUCHE;    //cela déplace la fusée (qu'on nomme ici carré, square en anglais) à gauche
                                break;
                        case SDLK_RIGHT:
                                square.x += DROITE;    //et il en est de même à droite
                                break;
                        }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);

        // Déplacement des astéroïdes et rendu
        for (size_t row = 0; row < NUM_ROWS; row++) {
            for (size_t i = 0; i < NUM_ASTEROIDS; i++) {
                asteroids[row][i].y++;
                asteroids[row][i].y%=SCREEN_HEIGHT;
                SDL_RenderFillRect(renderer, &asteroids[row][i]);
            }
        }

        for (int row=1; row<NUM_ROWS; row++) {
                for (int i=1; i<NUM_ASTEROIDS; i++) {
                        SDL_bool collision = SDL_HasIntersection(&asteroids[row][i], &square); //L'intersection de la fusée
                        if (collision){							       //et d'un astéroïde, soit quand
                               printf("Game Over.\n"                                         //la fusée touche un astéroïde,
				      "Score : %d\n"				       //définit la collision et donc
                                      "Try again.\n",score);					       //le Game Over. En perdant, la
                                quit=1;							       //la fenêtre se ferme, d'où quit=1
                    	}
                        else if (square.x == SCREEN_WIDTH){                                    //De même, il est interdit
                        	printf("Game Over.\n"	                                               //de dépasser la fenêtre,
                        	       "Score : %d\n"					         //cela mène donc également au
                                       "Try again.\n",score);					       //Game over.
                                quit=1;
                        }
                        else if (square.x == 0){
                                printf("Game Over.\n"
                                       "Score : %d\n"
                                       "Try again.\n",score);
                                quit=1;
                        }
                }
		score+=1;                              //Tant que le joueur ou la joueuse n'a pas encore perdu, le score augmente chaque ms.
        }

        // Set renderer color for the square
        SDL_SetRenderDrawColor(renderer,220, 140, 200, 200);
        // Draw filled square
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);

        SDL_Delay(5);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
