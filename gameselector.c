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

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
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

    // Skip the rest of the first line in the config file (newline after the number)
    fgetc(configFile);

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
            strtok(line, "\r\n"); // Remove any trailing newline or carriage return
            char *imageName = strtok(line, "|");
            char *command = strtok(NULL, "|");
            if (imageName && command) {
                imageNames[i] = strdup(imageName);
                imageCommands[i] = strdup(command);
            } else {
                printf("Error: Malformed line in config file: '%s'\n", line);
                fclose(configFile);
                return 1;
            }
        }
    }

    fclose(configFile);

    SDL_Texture *imageTextures[numImages];

    // Load images into textures
    for (int i = 0; i < numImages; i++) {
        char imagePath[256];
        snprintf(imagePath, sizeof(imagePath), "%s%s", IMAGE_DIR, imageNames[i]);
        SDL_Surface *imageSurface = IMG_Load(imagePath);
        if (imageSurface) {
            imageTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
            SDL_FreeSurface(imageSurface);
        } else {
            printf("Error loading image: %s\n", imagePath);
            imageTextures[i] = NULL;  // Assign NULL in case of an error
        }
    }

    int quit = false;
    SDL_Event e;
    int currentIndex = 0;

    // Initialize game controller
    SDL_GameController *controller = NULL;
    if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0)) {
        controller = SDL_GameControllerOpen(0);
        if (controller) {
            printf("Game controller connected: %s\n", SDL_GameControllerName(controller));
        } else {
            printf("Failed to open game controller.\n");
        }
    }

    // Main loop
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return 1;
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
                            int ret = system(imageCommands[currentIndex]);
                            if (ret == -1) {
                                printf("Failed to execute command: %s\n", imageCommands[currentIndex]);
                            }
                        } else {
                            printf("The config file does not exist.\n");
                        }
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        return 1;
                        break;
                }
            }

            // Game controller events
            if (controller) {
                if (e.type == SDL_CONTROLLERBUTTONDOWN) {
                    switch (e.cbutton.button) {
                        case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                            currentIndex = (currentIndex - 1 + numImages) % numImages;
                            break;
                        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                            currentIndex = (currentIndex + 1) % numImages;
                            break;
                        case SDL_CONTROLLER_BUTTON_A:
                        case SDL_CONTROLLER_BUTTON_B:
                        case SDL_CONTROLLER_BUTTON_START:
                            if (fileExists(imageCommands[currentIndex])) {
                                int ret = system(imageCommands[currentIndex]);
                                if (ret == -1) {
                                    printf("Failed to execute command: %s\n", imageCommands[currentIndex]);
                                }
                            } else {
                                printf("The config file does not exist.\n");
                            }
                            break;
                        case SDL_CONTROLLER_BUTTON_BACK:
                            quit = true;
                            return 1;
                            break;
                    }
                }
            }

        // Render current image
        SDL_RenderClear(renderer);
        if (imageTextures[currentIndex]) {
            SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        }
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

    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
