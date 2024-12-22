#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define CONFIG_FILE "gameselector.conf"
#define IMAGE_DIR "gsimages/"

#define bool int
#define true 1
#define false 0

bool quit = false;

// Function to handle SDL2 events
int handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
            return 1;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym;) {
                case SDLK_UP:
                    return SDLK_UP;
                case SDLK_DOWN:
                    return SDLK_DOWN;
                case SDLK_LEFT:
                    return SDLK_LEFT;
                case SDLK_RIGHT:
                    return SDLK_RIGHT;
                case SDLK_RETURN:
                    return SDLK_RETURN;
                case SDLK_ESCAPE:
                    return SDLK_ESCAPE;
            }
        } else if (e.type == SDL_CONTROLLERBUTTONDOWN) {
            switch (e.cbutton.button;) {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    return SDLK_UP;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    return SDLK_DOWN;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    return SDLK_LEFT;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    return SDLK_RIGHT;
                case SDL_CONTROLLER_BUTTON_A:
                case SDL_CONTROLLER_BUTTON_B:
                case SDL_CONTROLLER_BUTTON_START:
                    return SDLK_RETURN;
                case SDL_CONTROLLER_BUTTON_BACK:
                    return SDLK_ESCAPE;
            }
        }
    }
    return 0;
}


// Check if file exists
int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}


// The main program loop
int main() {

    const char *xdgDataHome = getenv("XDG_DATA_HOME");
    if (xdgDataHome != NULL) {
        setenv("XDG_DATA_HOME", xdgDataHome, 1);
        printf("GameSelector: XDG_DATA_HOME=%s\n", xdgDataHome);
    }

    const char *xdgConfigHome = getenv("XDG_CONFIG_HOME");
    if (xdgConfigHome != NULL) {
        setenv("XDG_CONFIG_HOME", xdgConfigHome, 1);
        printf("GameSelector: XDG_CONFIG_HOME=%s\n", xdgConfigHome);
    }

    const char *Home = getenv("HOME");
    if (Home != NULL) {
        setenv("HOME", Home, 1);
        printf("GameSelector: HOME=%s\n", Home);
    }

    const char *controlFolder = getenv("controlfolder");
    if (controlFolder != NULL) {
        setenv("controlfolder", controlFolder, 1);
        printf("GameSelector: controlfolder=%s\n", controlFolder);
    }

    const char *Directory = getenv("directory");
    if (Directory != NULL) {
        setenv("directory", Directory, 1);
        printf("GameSelector: directory=%s\n", Directory);
    }

    const char *eSudo = getenv("ESUDO");
    if (eSudo != NULL) {
        setenv("ESUDO", eSudo, 1);
        printf("GameSelector: ESUDO=%s\n", eSudo);
    }

    const char *eSudoKill = getenv("ESUDOKILL");
    if (eSudoKill != NULL) {
        setenv("ESUDOKILL", eSudoKill, 1);
        printf("GameSelector: ESUDOKILL=%s\n", eSudoKill);
    }

    const char *gameDir = getenv("GAMEDIR");
    if (gameDir != NULL) {
        setenv("GAMEDIR", gameDir, 1);
        printf("GameSelector: GAMEDIR=%s\n", gameDir);
    }

    const char *ldLibraryPath = getenv("LD_LIBRARY_PATH");
    if (ldLibraryPath != NULL) {
        setenv("LD_LIBRARY_PATH", ldLibraryPath, 1);
        printf("GameSelector: LD_LIBRARY_PATH=%s\n", ldLibraryPath);
    }

    const char *sdlControllerConfig = getenv("sdl_controllerconfig");
    if (sdlControllerConfig != NULL) {
        setenv("sdl_controllerconfig", sdlControllerConfig, 1);
        printf("GameSelector: sdl_controllerconfig=%s\n", sdlControllerConfig);
    }

    const char *sdlGameControllerConfig = getenv("SDL_GAMECONTROLLERCONFIG");
    if (sdlGameControllerConfig != NULL) {
        setenv("SDL_GAMECONTROLLERCONFIG", sdlGameControllerConfig, 1);
        printf("GameSelector: SDL_GAMECONTROLLERCONFIG=%s\n", sdlGameControllerConfig);
    }

    const char *sdlGameControllerConfigFile = getenv("SDL_GAMECONTROLLERCONFIG_FILE");
    if (sdlGameControllerConfigFile != NULL) {
        setenv("SDL_GAMECONTROLLERCONFIG_FILE", sdlGameControllerConfigFile, 1);
        printf("GameSelector: SDL_GAMECONTROLLERCONFIG_FILE=%s\n", sdlGameControllerConfigFile);
    }

    const char *gptoKeyb = getenv("GPTOKEYB");
    if (gptoKeyb != NULL) {
        setenv("GPTOKEYB", gptoKeyb, 1);
        printf("GameSelector: GPTOKEYB=%s\n", gptoKeyb);
    }

    const char *gptoKeyb2 = getenv("GPTOKEYB2");
    if (gptoKeyb2 != NULL) {
        setenv("GPTOKEYB2", gptoKeyb2, 1);
        printf("GameSelector: GPTOKEYB2=%s\n", gptoKeyb2);
    }

    const char *textInputPreset = getenv("TEXTINPUTPRESET");
    if (textInputPreset != NULL) {
        setenv("TEXTINPUTPRESET", textInputPreset, 1);
        printf("GameSelector: TEXTINPUTPRESET=%s\n", textInputPreset);
    }

    const char *textInputInteractive = getenv("TEXTINPUTINTERACTIVE");
    if (textInputInteractive != NULL) {
        setenv("TEXTINPUTINTERACTIVE", textInputInteractive, 1);
        printf("GameSelector: TEXTINPUTINTERACTIVE=%s\n", textInputInteractive);
    }

    const char *textInputNoAutoCapitals = getenv("TEXTINPUTNOAUTOCAPITALS");
    if (textInputNoAutoCapitals != NULL) {
        setenv("TEXTINPUTNOAUTOCAPITALS", textInputNoAutoCapitals, 1);
        printf("GameSelector: TEXTINPUTNOAUTOCAPITALS=%s\n", textInputNoAutoCapitals);
    }

    const char *textInputAddExtraSymbols = getenv("TEXTINPUTADDEXTRASYMBOLS");
    if (textInputAddExtraSymbols != NULL) {
        setenv("TEXTINPUTADDEXTRASYMBOLS", textInputAddExtraSymbols, 1);
        printf("GameSelector: TEXTINPUTADDEXTRASYMBOLS=%s\n", textInputAddExtraSymbols);
    }

    const char *Device = getenv("DEVICE");
    if (Device != NULL) {
        setenv("DEVICE", Device, 1);
        printf("GameSelector: DEVICE=%s\n", Device);
    }

    const char *osName = getenv("OS_NAME");
    if (osName != NULL) {
        setenv("OS_NAME", osName, 1);
        printf("GameSelector: OS_NAME=%s\n", osName);
    }

    const char *RaLoc = getenv("raloc");
    if (RaLoc != NULL) {
        setenv("raloc", RaLoc, 1);
        printf("GameSelector: raloc=%s\n", RaLoc);
    }

    const char *RaConf = getenv("raconf");
    if (RaConf != NULL) {
        setenv("raconf", RaConf, 1);
        printf("GameSelector: raconf=%s\n", RaConf);
    }

    const char *lowRes = getenv("LOWRES");
    if (lowRes != NULL) {
        setenv("LOWRES", lowRes, 1);
        printf("GameSelector: LOWRES=%s\n", lowRes);
    }

    const char *pmVersion = getenv("PM_VERSION");
    if (pmVersion != NULL) {
        setenv("PM_VERSION", pmVersion, 1);
        printf("GameSelector: PM_VERSION=%s\n", pmVersion);
    }

    const char *cfwName = getenv("CFW_NAME");
    if (cfwName != NULL) {
        setenv("CFW_NAME", cfwName, 1);
        printf("GameSelector: CFW_NAME=%s\n", cfwName);
    }

    const char *cfwVersion = getenv("CFW_VERSION");
    if (cfwVersion != NULL) {
        setenv("CFW_VERSION", cfwVersion, 1);
        printf("GameSelector: CFW_VERSION=%s\n", cfwVersion);
    }

    const char *deviceName = getenv("DEVICE_NAME");
    if (deviceName != NULL) {
        setenv("DEVICE_NAME", deviceName, 1);
        printf("GameSelector: DEVICE_NAME=%s\n", deviceName);
    }

    const char *deviceCpu = getenv("DEVICE_CPU");
    if (deviceCpu != NULL) {
        setenv("DEVICE_CPU", deviceCpu, 1);
        printf("GameSelector: DEVICE_CPU=%s\n", deviceCpu);
    }

    const char *deviceArch = getenv("DEVICE_ARCH");
    if (deviceArch != NULL) {
        setenv("DEVICE_ARCH", deviceArch, 1);
        printf("GameSelector: DEVICE_ARCH=%s\n", deviceArch);
    }

    const char *deviceRam = getenv("DEVICE_RAM");
    if (deviceRam != NULL) {
        setenv("DEVICE_RAM", deviceRam, 1);
        printf("GameSelector: DEVICE_RAM=%s\n", deviceRam);
    }

    const char *deviceHasARMHF = getenv("DEVICE_HAS_ARMHF");
    if (deviceHasARMHF != NULL) {
        setenv("DEVICE_HAS_ARMHF", deviceHasARMHF, 1);
        printf("GameSelector: DEVICE_HAS_ARMHF=%s\n", deviceHasARMHF);
    }

    const char *deviceHasAARCH64 = getenv("DEVICE_HAS_AARCH64");
    if (deviceHasAARCH64 != NULL) {
        setenv("DEVICE_HAS_AARCH64", deviceHasAARCH64, 1);
        printf("GameSelector: DEVICE_HAS_AARCH64=%s\n", deviceHasAARCH64);
    }

    const char *deviceHasX86 = getenv("DEVICE_HAS_X86");
    if (deviceHasX86 != NULL) {
        setenv("DEVICE_HAS_X86", deviceHasX86, 1);
        printf("GameSelector: DEVICE_HAS_X86=%s\n", deviceHasX86);
    }

    const char *deviceHasX86_64 = getenv("DEVICE_HAS_X86_64");
    if (deviceHasX86_64 != NULL) {
        setenv("DEVICE_HAS_X86_64", deviceHasX86_64, 1);
        printf("GameSelector: DEVICE_HAS_X86_64=%s\n", deviceHasX86_64);
    }

    const char *displayWidth = getenv("DISPLAY_WIDTH");
    if (displayWidth != NULL) {
        setenv("DISPLAY_WIDTH", displayWidth, 1);
        printf("GameSelector: DISPLAY_WIDTH=%s\n", displayWidth);
    }

    const char *displayHeight = getenv("DISPLAY_HEIGHT");
    if (displayHeight != NULL) {
        setenv("DISPLAY_HEIGHT", displayHeight, 1);
        printf("GameSelector: DISPLAY_HEIGHT=%s\n", displayHeight);
    }

    const char *aspectX = getenv("ASPECT_X");
    if (aspectX != NULL) {
        setenv("ASPECT_X", aspectX, 1);
        printf("GameSelector: ASPECT_X=%s\n", aspectX);
    }

    const char *aspectY = getenv("ASPECT_Y");
    if (aspectY != NULL) {
        setenv("ASPECT_Y", aspectY, 1);
        printf("GameSelector: ASPECT_Y=%s\n", aspectY);
    }

    const char *displayOrientation = getenv("DISPLAY_ORIENTATION");
    if (displayOrientation != NULL) {
        setenv("DISPLAY_ORIENTATION", displayOrientation, 1);
        printf("GameSelector: DISPLAY_ORIENTATION=%s\n", displayOrientation);
    }

    const char *analog_Sticks = getenv("ANALOG_STICKS");
    if (analog_Sticks != NULL) {
        setenv("ANALOG_STICKS", analog_Sticks, 1);
        printf("GameSelector: ANALOG_STICKS=%s\n", analog_Sticks);
    }

    const char *analogSticks = getenv("ANALOGSTICKS");
    if (analogSticks != NULL) {
        setenv("ANALOGSTICKS", analogSticks, 1);
        printf("GameSelector: ANALOGSTICKS=%s\n", analogSticks);
    }

    const char *spaPluginDir = getenv("SPA_PLUGIN_DIR");
    if (spaPluginDir != NULL) {
        setenv("SPA_PLUGIN_DIR", spaPluginDir, 1);
        printf("GameSelector: SPA_PLUGIN_DIR=%s\n", spaPluginDir);
    }

    const char *pipewireModuleDir = getenv("PIPEWIRE_MODULE_DIR");
    if (pipewireModuleDir != NULL) {
        setenv("PIPEWIRE_MODULE_DIR", pipewireModuleDir, 1);
        printf("GameSelector: PIPEWIRE_MODULE_DIR=%s\n", pipewireModuleDir);
    }

    const char *Term = getenv("TERM");
    if (Term != NULL) {
        setenv("TERM", Term, 1);
        printf("GameSelector: TERM=%s\n", Term);
    }

    const char *CurTTY = getenv("CUR_TTY");
    if (CurTTY != NULL) {
        setenv("CUR_TTY", CurTTY, 1);
        printf("GameSelector: CUR_TTY=%s\n", CurTTY);
    }

    const char *patcherFile = getenv("PATCHER_FILE");
    if (patcherFile != NULL) {
        setenv("PATCHER_FILE", patcherFile, 1);
        printf("GameSelector: PATCHER_FILE=%s\n", patcherFile);
    }

    const char *patcherGame = getenv("PATCHER_GAME");
    if (patcherGame != NULL) {
        setenv("PATCHER_GAME", patcherGame, 1);
        printf("GameSelector: PATCHER_GAME=%s\n", patcherGame);
    }

    const char *patcherTime = getenv("PATCHER_TIME");
    if (patcherTime != NULL) {
        setenv("PATCHER_TIME", patcherTime, 1);
        printf("GameSelector: PATCHER_TIME=%s\n", patcherTime);
    }

    const char *port32Bit = getenv("PORT_32BIT");
    if (port32Bit != NULL) {
        setenv("PORT_32BIT", port32Bit, 1);
        printf("GameSelector: PORT_32BIT=%s\n", port32Bit);
    }

    const char *splashFile = getenv("SPLASHFILE");
    if (splashFile != NULL) {
        setenv("SPLASHFILE", splashFile, 1);
        printf("GameSelector: SPLASHFILE=%s\n", splashFile);
    }

    const char *gmLoaderDepthDisable = getenv("GMLOADER_DEPTH_DISABLE");
    if (gmLoaderDepthDisable != NULL) {
        setenv("GMLOADER_DEPTH_DISABLE", gmLoaderDepthDisable, 1);
        printf("GameSelector: GMLOADER_DEPTH_DISABLE=%s\n", gmLoaderDepthDisable);
    }

    const char *gmLoaderSaveDir = getenv("GMLOADER_SAVEDIR");
    if (gmLoaderSaveDir != NULL) {
        setenv("GMLOADER_SAVEDIR", gmLoaderSaveDir, 1);
        printf("GameSelector: GMLOADER_SAVEDIR=%s\n", gmLoaderSaveDir);
    }

    const char *gmLoaderPlatform = getenv("GMLOADER_PLATFORM");
    if (gmLoaderPlatform != NULL) {
        setenv("GMLOADER_PLATFORM", gmLoaderPlatform, 1);
        printf("GameSelector: GMLOADER_PLATFORM=%s\n", gmLoaderPlatform);
    }

    const char *frtNoExitShortcuts = getenv("FRT_NO_EXIT_SHORTCUTS");
    if (frtNoExitShortcuts != NULL) {
        setenv("FRT_NO_EXIT_SHORTCUTS", frtNoExitShortcuts, 1);
        printf("GameSelector: FRT_NO_EXIT_SHORTCUTS=%s\n", frtNoExitShortcuts);
    }

    const char *Paff = getenv("PATH");
    if (Paff != NULL) {
        setenv("PATH", Paff, 1);
        printf("GameSelector: PATH=%s\n", Paff);
    }

    const char *confDir = getenv("CONFDIR");
    if (confDir != NULL) {
        setenv("CONFDIR", confDir, 1);
        printf("GameSelector: CONFDIR=%s\n", confDir);
    }

    const char *Binary = getenv("BINARY");
    if (Binary != NULL) {
        setenv("BINARY", Binary, 1);
        printf("GameSelector: BINARY=%s\n", Binary);
    }


    // Initialize SDL
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
      fprintf(stderr, "GameSelector: Failed to initialize SDL: %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }

    IMG_Init(IMG_INIT_PNG);

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Game Selector", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          640, 
                                          480, 
                                          SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == NULL) {
        fprintf(stderr, "GameSelector: Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "GameSelector: Failed to create SDL renderer: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    //SDL_RenderSetLogicalSize(renderer, 640, 480);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // Open the config file
    FILE *configFile = fopen(CONFIG_FILE, "r");
    if (!configFile) {
        printf("GameSelector: Unable to open config file: %s\n", CONFIG_FILE);
        return 1;
    }

    int numImages;
    if (fscanf(configFile, "%d", &numImages) != 1) {
        printf("GameSelector: Error reading number of selection entries from line 1 in gameselector.conf.\n");
        fclose(configFile);
        return 1;
    }

    // Skip the rest of the first line in the config file (newline after the number)
    fgetc(configFile);

    const char **imageNames = (const char **)malloc(numImages * sizeof(const char *));
    char **imageCommands = (char **)malloc(numImages * sizeof(char *));

    if (!imageNames || !imageCommands) {
        printf("GameSelector: Error allocating memory.\n");
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
                printf("GameSelector error: Malformed line in config file: '%s'\n", line);
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
            printf("GameSelector: Error loading image: %s\n", imagePath);
            imageTextures[i] = NULL;  // Assign NULL in case of an error
        }
    }

    int currentIndex = 0;

    // Initialize game controller
    SDL_GameController *controller = NULL;
    if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0)) {
        controller = SDL_GameControllerOpen(0);
        if (controller) {
            printf("GameSelector: Game controller connected: %s\n", SDL_GameControllerName(controller));
        } else {
            printf("GameSelector: Failed to open game controller.\n");
        }
    }

    while (!quit) {
        int event = handleEvents();

        if (event == SDLK_RETURN) {
            if (fileExists(imageCommands[currentIndex])) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                int ret = system(imageCommands[currentIndex]);
                if (ret == -1) {
                    printf("GameSelector: Failed to execute command: %s\n", imageCommands[currentIndex]);
                }
                exit(0);
            } else {
                printf("GameSelector: The command or file does not exist.\n");
            }
            break;
        }

        switch (event) {
            case SDLK_UP:
            case SDLK_LEFT:
                currentIndex = (currentIndex - 1 + numImages) % numImages;
                break;
            case SDLK_DOWN:
            case SDLK_RIGHT:
                currentIndex = (currentIndex + 1) % numImages;
                break;
            case SDLK_ESCAPE:
                quit = true;
                SDL_Quit();
                exit(0);
                return 1;
                break;
        }


        // Render current image
        SDL_RenderClear(renderer);
        if (imageTextures[currentIndex]) {
            SDL_RenderCopy(renderer, imageTextures[currentIndex], NULL, NULL);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
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
