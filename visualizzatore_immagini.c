#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

#define NUM_IMAGES 8
#define COMMANDS_FILE "comandi.txt"
#define IMAGE_DIR "images/"

#define bool int
#define true 1
#define false 0

const char *imageNames[NUM_IMAGES];
char *imageCommands[NUM_IMAGES];
const char *imageSections[NUM_IMAGES];

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

    SDL_Texture *imageTextures[NUM_IMAGES];

    // Leggi i comandi da un file di testo
    FILE *commandsFile = fopen(COMMANDS_FILE, "r");
    if (!commandsFile) {
        printf("Impossibile aprire il file dei comandi: %s\n", COMMANDS_FILE);
        return 1;
    }

    for (int i = 0; i < NUM_IMAGES; i++) {
        char line[256];
        if (fgets(line, sizeof(line), commandsFile) != NULL) {
            // Rimuovi spazi bianchi e caratteri di nuova riga
            strtok(line, "\r\n");

            // Usa strtok per suddividere la riga nei due valori
            char *imageName = strtok(line, "|");
            char *command = strtok(NULL, "|");

            // Assegna i valori alle variabili appropriate
            imageNames[i] = strdup(imageName);
            imageCommands[i] = strdup(command);
        }
    }

    fclose(commandsFile);

    // Carica tutte le immagini in formato PNG dalla cartella "images"
    for (int i = 0; i < NUM_IMAGES; i++) {
        char imagePath[256];
        snprintf(imagePath, sizeof(imagePath), "%s%s", IMAGE_DIR, imageNames[i]);

        SDL_Surface *imageSurface = IMG_Load(imagePath);
        imageTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }

    // Loop principale
    int quit = false;
    SDL_Event e;
    int currentIndex = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        currentIndex = (currentIndex - 1 + NUM_IMAGES) % NUM_IMAGES;
                        break;
                    case SDLK_RIGHT:
                        currentIndex = (currentIndex + 1) % NUM_IMAGES;
                        break;
                    case SDLK_RETURN:
                        // Esegui il comando associato all'immagine corrente
                        if (fileExists(imageCommands[currentIndex])) {
                            system(imageCommands[currentIndex]);
                        } else {
                            printf("Il file di comando non esiste.\n");
                        }
                        break;
                }
            }
        }

        // Pulisci lo schermo e disegna l'immagine corrente
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        SDL_RenderPresent(renderer);
    }

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
