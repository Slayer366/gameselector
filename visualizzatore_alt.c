#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMANDS_FILE "controlli.txt"
#define IMAGE_DIR "images/"

#define bool int
#define true 1
#define false 0

int countOptions(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0; // Il file non esiste o non può essere aperto
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG); // Inizializza SDL_image per il supporto PNG

    // Inizializza la finestra SDL
    SDL_Window *window = SDL_CreateWindow("Visualizzatore di Immagini SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    const int NUM_IMAGES = countOptions(COMMANDS_FILE);
    if (NUM_IMAGES == 0) {
        printf("Nessuna opzione trovata nel file dei comandi: %s\n", COMMANDS_FILE);
        return 1;
    }

    const char *imageNames[NUM_IMAGES];
    char *imageCommands[NUM_IMAGES];

    // ...

    // Libera le risorse
    for (int i = 0; i < NUM_IMAGES; i++) {
        SDL_DestroyTexture(imageTextures[i]);
        free(imageCommands[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); // Chiudi SDL_image
    SDL_Quit();

    return 0;
}
