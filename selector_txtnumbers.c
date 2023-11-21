#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMANDS_FILE "comandi.txt"
#define IMAGE_DIR "images/"

#define bool int
#define true 1
#define false 0

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
    IMG_Init(IMG_INIT_PNG); 

    SDL_Window *window = SDL_CreateWindow("Visualizzatore di Immagini SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    FILE *commandsFile = fopen(COMMANDS_FILE, "r");
    if (!commandsFile) {
        printf("Impossibile aprire il file dei comandi: %s\n", COMMANDS_FILE);
        return 1;
    }

    int numImages;
    if (fscanf(commandsFile, "%d", &numImages) != 1) {
        printf("Errore nella lettura del numero di immagini.\n");
        fclose(commandsFile);
        return 1;
    }

    const char **imageNames = (const char **)malloc(numImages * sizeof(const char *));
    char **imageCommands = (char **)malloc(numImages * sizeof(char *));

    if (!imageNames || !imageCommands) {
        printf("Errore nell'allocazione della memoria.\n");
        fclose(commandsFile);
        return 1;
    }

    for (int i = 0; i < numImages; i++) {
        char line[256];
        if (fgets(line, sizeof(line), commandsFile) != NULL) {
            strtok(line, "\r\n");
            char *imageName = strtok(line, "|");
            char *command = strtok(NULL, "|");
            imageNames[i] = strdup(imageName);
            imageCommands[i] = strdup(command);
        }
    }

    fclose(commandsFile);

    SDL_Texture *imageTextures[numImages];

    for (int i = 0; i < numImages; i++) {
        char imagePath[256];
        snprintf(imagePath, sizeof(imagePath), "%s%s", IMAGE_DIR, imageNames[i]);
        SDL_Surface *imageSurface = IMG_Load(imagePath);
        imageTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }

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
                        currentIndex = (currentIndex - 1 + numImages) % numImages;
                        break;
                    case SDLK_RIGHT:
                        currentIndex = (currentIndex + 1) % numImages;
                        break;
                    case SDLK_RETURN:
                        if (fileExists(imageCommands[currentIndex])) {
                            system(imageCommands[currentIndex]);
                        } else {
                            printf("Il file di comando non esiste.\n");
                        }
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < numImages; i++) {
        SDL_DestroyTexture(imageTextures[i]);
        free((void *)imageNames[i]);
        free(imageCommands[i]);
    }

    free(imageNames);
    free(imageCommands);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
