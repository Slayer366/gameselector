#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // Aggiunto per isspace()

#define COMMANDS_FILE "controlli.txt"
#define IMAGE_DIR "images/"

// Prototipi delle funzioni
int countOptions(const char *filename);
bool readImageGifAndCommandFromControlliTxt(const char *filename, char *imageName, char *gifName, char *command);
void trimWhitespace(char *str);
void executeGifAndCommand(const char *gifName, const char *command);

int main(int argc, char *argv[]) {
    // Dichiarazione della variabile "running" e inizializzazione a true
    bool running = true;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Visualizzatore di Immagini SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    const int NUM_IMAGES = countOptions(COMMANDS_FILE);
    if (NUM_IMAGES == 0) {
        printf("Nessuna opzione trovata nel file dei comandi: %s\n", COMMANDS_FILE);
        return 1;
    }

    const char *imageNames[NUM_IMAGES];
    char *imageCommands[NUM_IMAGES];
    char gifName[256];
    char imageName[256];
    char command[256];

    Uint32 startTime = SDL_GetTicks();
    bool gifActivated = false;

    while (running) {
        // ...

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        if (elapsedTime >= 3000 && !gifActivated) {
            // Esegui la gif e il comando associato dal file controlli.txt.
            gifActivated = true;

            // Leggi il nome dell'immagine PNG, il nome della gif e il comando dal file controlli.txt.
            if (readImageGifAndCommandFromControlliTxt(COMMANDS_FILE, imageName, gifName, command)) {
                // Carica e visualizza l'immagine PNG da IMAGE_DIR
                char imagePath[512];
                snprintf(imagePath, sizeof(imagePath), "%s%s", IMAGE_DIR, imageName);
                SDL_Surface *imageSurface = IMG_Load(imagePath);
                SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

                // Disegna l'immagine PNG sul renderer
                SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
                SDL_RenderPresent(renderer);

                // Attendi un po' prima di eseguire la gif
                SDL_Delay(500);

                // Esegui la gif e il comando associato
                executeGifAndCommand(gifName, command);

                // Libera le risorse dell'immagine PNG
                SDL_DestroyTexture(imageTexture);
                SDL_FreeSurface(imageSurface);
            }
        }

        // ...

        if (gifActivated) {
            // Reimposta il tempo di inizio.
            startTime = SDL_GetTicks();
            gifActivated = false;
        }
    }

    // ...

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

// Funzione per contare le opzioni nel file dei comandi
int countOptions(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}

// Funzione per leggere il nome dell'immagine PNG, la gif e il comando da controlli.txt
bool readImageGifAndCommandFromControlliTxt(const char *filename, char *imageName, char *gifName, char *command) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }

    while (fscanf(file, "%255[^|] | %255[^|] | %255[^\n]", imageName, gifName, command) == 3) {
        // Rimuovi gli spazi in eccesso
        trimWhitespace(imageName);
        trimWhitespace(gifName);
        trimWhitespace(command);

        // Restituisci il primo match valido trovato nel file
        fclose(file);
        return true;
    }

    fclose(file);
    return false;
}

// Funzione per rimuovere gli spazi in eccesso da una stringa
void trimWhitespace(char *str) {
    char *end;

    // Rimuovi gli spazi iniziali
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // Se la stringa è diventata tutta spazi, restituisci una stringa vuota
    if (*str == '\0') {
        return;
    }

    // Trova l'ultimo carattere non spazio
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Aggiungi il terminatore di stringa dopo l'ultimo carattere non spazio
    *(end + 1) = '\0';
}

// Funzione per eseguire la gif e il comando
void executeGifAndCommand(const char *gifName, const char *command) {
    // Comando per eseguire la gif e il comando specificati
    char commandBuffer[512];
    snprintf(commandBuffer, sizeof(commandBuffer), "start %s && %s", gifName, command);
    system(commandBuffer);
}
