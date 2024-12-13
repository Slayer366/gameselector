#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "gameselector.conf"
#define IMAGE_DIR "images/"

#define bool int
#define true 1
#define false 0

// Check if file exists
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
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("SDL Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Open the config file
    FILE *configFile = fopen(CONFIG_FILE, "r");
    if (!configFile) {
        printf("Unable to open config file: %s\n", CONFIG_FILE);
        return 1;
    }

    int numImages;
    if (fscanf(configFile, "%d", &numImages) != 1) {
        printf("Error reading number of selection entries from line 1 in gameselector.conf.\n");
        fclose(configFile);
        return 1;
    }

    const char **imageNames = (const char **)malloc(numImages * sizeof(const char *));
    char **imageCommands = (char **)malloc(numImages * sizeof(char *));

    if (!imageNames || !imageCommands) {
        printf("Error allocating memory.\n");
        fclose(configFile);
        return 1;
    }

    // Read image names and commands from the file
    for (int i = 0; i < numImages; i++) {
        char line[256];
        if (fgets(line, sizeof(line), configFile) != NULL) {
            strtok(line, "\r\n");
            char *imageName = strtok(line, "|");
            char *command = strtok(NULL, "|");
            imageNames[i] = strdup(imageName);
            imageCommands[i] = strdup(command);
        }
    }

    fclose(configFile);

    SDL_Texture *imageTextures[numImages];

    // Load images into textures
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

    // Initialize joystick
    SDL_Joystick *joystick = NULL;
    if (SDL_NumJoysticks() > 0) {
        joystick = SDL_JoystickOpen(0);
        if (joystick) {
            printf("Joystick connected: %s\n", SDL_JoystickName(joystick));
        } else {
            printf("Failed to open joystick.\n");
        }
    }

    // Main loop
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // Keyboard events
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_UP:
                        currentIndex = (currentIndex - 1 + numImages) % numImages;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_DOWN:
                        currentIndex = (currentIndex + 1) % numImages;
                        break;
                    case SDLK_RETURN:
                        if (fileExists(imageCommands[currentIndex])) {
                            system(imageCommands[currentIndex]);
                        } else {
                            printf("The config file does not exist.\n");
                        }
                        break;
                }
            }

            // Joystick events
            if (e.type == SDL_JOYHATMOTION) {
                switch (e.jhat.value) {
                    case SDL_HAT_LEFT:
                    case SDL_HAT_UP:
                        currentIndex = (currentIndex - 1 + numImages) % numImages;
                        break;
                    case SDL_HAT_RIGHT:
                    case SDL_HAT_DOWN:
                        currentIndex = (currentIndex + 1) % numImages;
                        break;
                }
            }

            if (e.type == SDL_JOYBUTTONDOWN) {
                if (e.jbutton.button == 0 || e.jbutton.button == 7) {  // "A" button or "Start"
                    if (fileExists(imageCommands[currentIndex])) {
                        system(imageCommands[currentIndex]);
                    } else {
                        printf("The config file does not exist.\n");
                    }
                }
            }
        }

        // Render current image
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Free resources
    for (int i = 0; i < numImages; i++) {
        SDL_DestroyTexture(imageTextures[i]);
        free((void *)imageNames[i]);
        free(imageCommands[i]);
    }

    free(imageNames);
    free(imageCommands);

    if (joystick) {
        SDL_JoystickClose(joystick);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
