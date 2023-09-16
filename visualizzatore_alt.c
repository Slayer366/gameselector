#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

#define CONTROLS_FILE "controlli.txt"
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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
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

    int numImages = 0; // Numero di immagini lette da "controlli.txt"

    // Leggi il numero di immagini da "controlli.txt"
    FILE *controlsFile = fopen(CONTROLS_FILE, "r");
    if (!controlsFile) {
        printf("Impossibile aprire il file dei controlli: %s\n", CONTROLS_FILE);
        return 1;
    }

    if (fscanf(controlsFile, "%d", &numImages) != 1) {
        printf("Errore nella lettura del numero di immagini dal file dei controlli.\n");
        fclose(controlsFile);
        return 1;
    }

    fclose(controlsFile);

    // Leggi le informazioni delle immagini da "controlli.txt"
    for (int i = 0; i < numImages; i++) {
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
    for (int i = 0; i < numImages; i++) {
        char imagePath[256];
        snprintf(imagePath, sizeof(imagePath), "%s%s", IMAGE_DIR, imageNames[i]);

        SDL_Surface *imageSurface = IMG_Load(imagePath);
        imageTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }

    // Inizializza il joystick
    if (SDL_NumJoysticks() < 1) {
        printf("Nessun joystick rilevato.\n");
        return 1;
    }

    SDL_Joystick *joystick = SDL_JoystickOpen(0);
    if (!joystick) {
        printf("Impossibile aprire il joystick: %s\n", SDL_GetError());
        return 1;
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

            // Gestisci gli eventi del joystick
            if (e.type == SDL_JOYBUTTONDOWN) {
                if (e.jbutton.button == 0) { // Esempio: pulsante 0 del joystick
                    // Esegui il comando associato all'immagine corrente
                    if (fileExists(imageCommands[currentIndex])) {
                        system(imageCommands[currentIndex]);
                    } else {
                        printf("Il file di comando non esiste.\n");
                    }
                }
            } else if (e.type == SDL_JOYAXISMOTION) {
                if (e.jaxis.axis == 0) { // Esempio: asse X del joystick
                    if (e.jaxis.value < -3200) {
                        currentIndex = (currentIndex - 1 + numImages) % numImages;
                    } else if (e.jaxis.value > 3200) {
                        currentIndex = (currentIndex + 1) % numImages;
                    }
                }
            }
        }

        // Pulisci lo schermo e disegna l'immagine corrente
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Chiudi il joystick
    SDL_JoystickClose(joystick);

    // Libera le risorse
    for (int i = 0; i < numImages; i++) {
        SDL_DestroyTexture(imageTextures[i]);
        free(imageCommands[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); // Chiudi SDL_image
    SDL_Quit();

    return 0;
}
