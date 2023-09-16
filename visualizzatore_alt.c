#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define COMMANDS_FILE "controlli.txt"
#define IMAGE_DIR "images/"
#define GAMEDATA_DIR "gamedata/"

#define bool int
#define true 1
#define false 0

struct ImageInfo {
    const char *imageName;
    char *imageCommand;
    const char *imageSection;
};

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

int countSubdirectories(const char *dirPath) {
    int count = 0;
    DIR *dir = opendir(dirPath);
    if (!dir) {
        return count;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
        }
    }

    closedir(dir);
    return count;
}

void assignCommandsToImages(struct ImageInfo *imageInfo, int numImages) {
    DIR *gamedataDir = opendir(GAMEDATA_DIR);
    if (!gamedataDir) {
        printf("Impossibile aprire la cartella gamedata: %s\n", GAMEDATA_DIR);
        return;
    }

    struct dirent *entry;
    int imageIndex = 0;
    while ((entry = readdir(gamedataDir)) != NULL && imageIndex < numImages) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char imageName[256];
            snprintf(imageName, sizeof(imageName), "%s.png", entry->d_name);

            imageInfo[imageIndex].imageName = strdup(imageName);

            char command[256];
            snprintf(command, sizeof(command), "./%s/%s/script.sh", GAMEDATA_DIR, entry->d_name);
            imageInfo[imageIndex].imageCommand = strdup(command);

            imageIndex++;
        }
    }

    closedir(gamedataDir);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Visualizzatore di Immagini SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Conta il numero di cartelle dirette in gamedata
    int numImages = countSubdirectories(GAMEDATA_DIR);
    struct ImageInfo *imageInfo = malloc(numImages * sizeof(struct ImageInfo));

    assignCommandsToImages(imageInfo, numImages);

    SDL_Texture **imageTextures = malloc(numImages * sizeof(SDL_Texture *));
    for (int i = 0; i < numImages; i++) {
        char imagePath[256];
        snprintf(imagePath, sizeof(imagePath), "%s%s", IMAGE_DIR, imageInfo[i].imageName);

        SDL_Surface *imageSurface = IMG_Load(imagePath);
        imageTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }

    int quit = false;
    SDL_Event e;
    int currentIndex = 0;

    while (!quit) {
        // Resto del tuo loop principale come prima
        // ...

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < numImages; i++) {
        free(imageInfo[i].imageCommand);
    }
    free(imageInfo);
    for (int i = 0; i < numImages; i++) {
        SDL_DestroyTexture(imageTextures[i]);
    }
    free(imageTextures);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
