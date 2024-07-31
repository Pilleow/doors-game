/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "screens.h"
#include "constants.h"
#include "entities/playerEffect.h"

#define SUPPORT_FILEFORMAT_WAV
#define SUPPORT_FILEFORMAT_MP3

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = MAINMENU;

Sound sfxDoorOpen[sfxDoorOpenCount];
Sound sfxShoot[sfxShootCount];
Sound sfxDash[sfxDashCount];
Sound sfxDead[sfxDeadCount];
Sound sfxHit[sfxHitCount];
Sound sfxWin;
Music startMusic = {0};
Music bgMusic[bgMusicCount];
int currentMusicIndex = -1;
float masterVolume = 0.0f;
float bgMusicVolume = 0.5f;
float sfxDoorOpenVolume = 0.5f;
float sfxShootVolume = 0.5f;
float sfxDeadVolume = 0.5f;
float sfxDashVolume = 0.5f;
float sfxHitVolume = 0.5f;

struct Level levels[levelCount];

RenderTexture2D screenRenderTexture;
Texture2D texBackground;
Texture2D texMainMenu;
Texture2D texGoofyWarning;

float recoilScale = 0.8f;
float playerEffectMultiplier = 1.5;
int winsNeededToWinGame = 10;
bool showFPS = false;
enum InputState inputState = MIXED;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Required variables to manage screen transitions (fade-in, fade-out)
static float transProgress = 0.0f;
static bool onTransition = false;
static int transFromScreen = -1;
static float bgmPreviousTime = -1;
static GameScreen transToScreen = UNKNOWN;
bool randomizeEffectsEveryRound = false;
bool playersUseTheSameWeapon = false;
Texture2D playerEffectSprites[17];

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect
static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)
static void UpdateDrawFrame(void);          // Update and draw one frame


void shuffleMusicArray(Music arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Music temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void shuffleLevelArray(struct Level arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Level temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void loadSfxIntoArray(Sound sfxArray[], char filename[], int totalCount, float initVolume) {
    for (int i = 0; i < totalCount; ++i) {
        char fn[64];
        sprintf(fn, filename, i);
        sfxArray[i] = LoadSound(fn);
        SetSoundVolume(sfxArray[i], initVolume);
    }
}

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //---------------------------------------------------------

    InitWindow(1, 1, "Door Game");
    int monitor = GetCurrentMonitor();
    finalWidth = GetMonitorWidth(monitor);
    finalHeight = GetMonitorHeight(monitor);
    CloseWindow();

    InitWindow(finalWidth, finalHeight, "Door Game");
    ToggleFullscreen();

    texBackground = LoadTexture("resources/sprites/bg.png");
    texMainMenu = LoadTexture("resources/sprites/mainmenu.png");
    texGoofyWarning = LoadTexture("resources/sprites/goofy_warning.png");

    playerEffectSprites[MORE_PLAYER_SPEED] = LoadTexture("resources/sprites/effects/more_player_speed.png");
    playerEffectSprites[LESS_PLAYER_SPEED] = LoadTexture("resources/sprites/effects/less_player_speed.png");
    playerEffectSprites[MORE_BOOLET_SPEED] = LoadTexture("resources/sprites/effects/more_boolet_speed.png");
    playerEffectSprites[LESS_BOOLET_SPEED] = LoadTexture("resources/sprites/effects/less_boolet_speed.png");
    playerEffectSprites[SMALLER_BODY] = LoadTexture("resources/sprites/effects/smaller_body.png");
    playerEffectSprites[LARGER_BODY] = LoadTexture("resources/sprites/effects/larger_body.png");
    playerEffectSprites[MORE_BOOLET_AMPLITUDE] = LoadTexture("resources/sprites/effects/more_boolet_amplitude.png");
    playerEffectSprites[LESS_BOOLET_AMPLITUDE] = LoadTexture("resources/sprites/effects/less_boolet_amplitude.png");
    playerEffectSprites[MORE_MAX_HEALTH] = LoadTexture("resources/sprites/effects/more_max_health.png");
    playerEffectSprites[LESS_MAX_HEALTH] = LoadTexture("resources/sprites/effects/less_max_health.png");
    playerEffectSprites[MORE_FRICTION] = LoadTexture("resources/sprites/effects/more_friction.png");
    playerEffectSprites[LESS_FRICTION] = LoadTexture("resources/sprites/effects/less_friction.png");
    playerEffectSprites[SHORTER_SHOOT_COOLDOWN] = LoadTexture("resources/sprites/effects/shorter_shoot_cooldown.png");
    playerEffectSprites[LONGER_SHOOT_COOLDOWN] = LoadTexture("resources/sprites/effects/longer_shoot_cooldown.png");
    playerEffectSprites[SHORTER_DASH_COOLDOWN] = LoadTexture("resources/sprites/effects/shorter_dash_cooldown.png");
    playerEffectSprites[LONGER_DASH_COOLDOWN] = LoadTexture("resources/sprites/effects/longer_dash_cooldown.png");
    playerEffectSprites[RANDOM_EFFECT_TO_EVERYONE] = LoadTexture("resources/sprites/effects/more_player_speed.png");

    screenRenderTexture = LoadRenderTexture(screenWidth, screenHeight);

    srand(time(NULL));      // Initialize random module
    InitAudioDevice();      // Initialize audio device
    SetMasterVolume(masterVolume);

    // Load sfx here individually
    loadSfxIntoArray(sfxShoot, "resources/sfx/shoot/shoot%d.wav", sfxShootCount, sfxShootVolume);
    loadSfxIntoArray(sfxDead, "resources/sfx/dead/dead%d.wav", sfxDeadCount, sfxDeadVolume);
    loadSfxIntoArray(sfxHit, "resources/sfx/hit/hit%d.wav", sfxHitCount, sfxHitVolume);
    loadSfxIntoArray(sfxDoorOpen, "resources/sfx/doorOpen/doorOpen%d.wav", sfxDoorOpenCount, sfxDoorOpenVolume);
    loadSfxIntoArray(sfxDash, "resources/sfx/dash/dash%d.wav", sfxDashCount, sfxDashVolume);
    sfxWin = LoadSound("resources/sfx/other/sfxWin.mp3");
    SetSoundVolume(sfxWin, bgMusicVolume);

    // Load bg music files here individually
    startMusic = LoadMusicStream("resources/bgm/gnosisHardware.mp3");
    bgMusic[0] = LoadMusicStream("resources/bgm/comeTrue.mp3");
    bgMusic[1] = LoadMusicStream("resources/bgm/akihabara.mp3");
    bgMusic[2] = LoadMusicStream("resources/bgm/outHereAtTheAtmosphere.mp3");
    bgMusic[3] = LoadMusicStream("resources/bgm/passportsOne.mp3");
    bgMusic[4] = LoadMusicStream("resources/bgm/chaos.mp3");
    bgMusic[5] = LoadMusicStream("resources/bgm/chicago.mp3");
    bgMusic[6] = LoadMusicStream("resources/bgm/nothingStopsDetroit.mp3");
    bgMusic[7] = LoadMusicStream("resources/bgm/passportsTwo.mp3");
    SetMusicVolume(startMusic, bgMusicVolume);
    for (int i = 0; i < bgMusicCount; ++i) SetMusicVolume(bgMusic[i], bgMusicVolume);
    shuffleMusicArray(bgMusic, bgMusicCount);

    PlayMusicStream(startMusic);

    // Load global data (assets that must be available in all screens, i.e. font)
//    font = LoadFont("resources/font/BebasNeue-Regular.ttf");
//    fxCoin = LoadSound("resources/coin.wav");

    // Setup and init first screen
    SetExitKey(KEY_NULL);
    currentScreen = MAINMENU;
    LoadAllLevels(levels);
    InitMainMenuScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    switch (currentScreen) {
        case GAMEPLAY:
            UnloadGameplayScreen();
            break;
        default:
            break;
    }

    // Unload global data loaded
    for (int i = 0; i < bgMusicCount; ++i) UnloadMusicStream(bgMusic[i]);
    for (int i = 0; i < sfxShootCount; ++i) UnloadSound(sfxShoot[i]);
    for (int i = 0; i < playerEffectCount; ++i) UnloadTexture(playerEffectSprites[i]);
    UnloadTexture(texBackground);
    UnloadTexture(texMainMenu);
    UnloadTexture(texGoofyWarning);
    UnloadRenderTexture(screenRenderTexture);
    CloseAudioDevice();     // Close audio context
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen) {

    // Init next screen
    switch (screen) {
        case GAMEPLAY:
            InitGameplayScreen();
            break;
        case LEVELEDITOR:
            InitLevelEditorScreen();
            break;
        case MAINMENU:
            InitMainMenuScreen();
            break;
        default:
            break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(GameScreen screen) {
    onTransition = true;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transProgress = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void) {
    const float a = 0.04;
    transProgress += -a * (transProgress - 0.5) * (transProgress - 0.5) + a - 0.02;

    if (transProgress > 0.5f && currentScreen != transToScreen) {

        // Unload current screen
        switch (transFromScreen) {
            case GAMEPLAY:
                UnloadGameplayScreen();
                break;
            default:
                break;
        }

        // Load next screen
        switch (transToScreen) {
            case GAMEPLAY:
                InitGameplayScreen();
                break;
            case LEVELEDITOR:
                InitLevelEditorScreen();
                break;
            case MAINMENU:
                InitMainMenuScreen();
                break;
            default:
                break;
        }

        currentScreen = transToScreen;
    }
    if (transProgress > 1.01f) {
        onTransition = false;
        transFromScreen = -1;
        transToScreen = UNKNOWN;
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void) {
    Vector2 topleft, topright, bottomleft, bottomright;
    const float slide = 1.08f;
    float t = (transProgress * 4 - 2) * slide;
    float x = -finalWidth * t;
    float delta = finalWidth * slide;

    topleft = (Vector2) {x, 0};
    topright = (Vector2) {x + delta + finalWidth, 0};
    bottomleft = (Vector2) {x - delta, finalHeight};
    bottomright = (Vector2) {x + finalWidth, finalHeight};

    DrawTriangle(bottomleft, bottomright, topleft, BLACK);
    DrawTriangle(topright, topleft, bottomright, BLACK);
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    // Update
    //----------------------------------------------------------------------------------
    if (currentMusicIndex == -1) {
        UpdateMusicStream(startMusic);
        if (bgmPreviousTime > GetMusicTimePlayed(startMusic)) {
            PauseMusicStream(startMusic);
            StopMusicStream(startMusic);
            currentMusicIndex = 0;
            PlayMusicStream(bgMusic[currentMusicIndex]);
            bgmPreviousTime = -1;
        }
//        else if (GetMusicTimePlayed(startMusic) < 5)
//        SeekMusicStream(startMusic, GetMusicTimeLength(startMusic) - 3);
        bgmPreviousTime = GetMusicTimePlayed(startMusic);
    } else {
        UpdateMusicStream(bgMusic[currentMusicIndex]);
        if (bgmPreviousTime > GetMusicTimePlayed(bgMusic[currentMusicIndex])) {
            PauseMusicStream(bgMusic[currentMusicIndex]);
            StopMusicStream(bgMusic[currentMusicIndex]);
            currentMusicIndex++;
            bgmPreviousTime = -1;
            if (currentMusicIndex >= bgMusicCount) {
                currentMusicIndex = 0;
                shuffleMusicArray(bgMusic, bgMusicCount);
            }
            PlayMusicStream(bgMusic[currentMusicIndex]);
        }
//        else if (GetMusicTimePlayed(bgMusic[currentMusicIndex]) < 5)
//        SeekMusicStream(bgMusic[currentMusicIndex], GetMusicTimeLength(bgMusic[currentMusicIndex]) - 3);
        bgmPreviousTime = GetMusicTimePlayed(bgMusic[currentMusicIndex]);
    }

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    if (!onTransition) {
        switch (currentScreen) {
            case GAMEPLAY:
                UpdateGameplayScreen();
                if (GotoLevelEditorScreen()) TransitionToScreen(LEVELEDITOR);
                if (GotoMainMenuScreen()) TransitionToScreen(MAINMENU);
                break;
            case LEVELEDITOR:
                UpdateLevelEditorScreen();
                if (GotoGameplayScreenFromLevelEditor()) TransitionToScreen(GAMEPLAY);
                break;
            case MAINMENU:
                UpdateMainMenuScreen();
                if (GotoGameplayScreenFromMainMenu()) TransitionToScreen(GAMEPLAY);
                break;
            default:
                break;
        }
    } else UpdateTransition();    // Update transition (fade-in, fade-out)

    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(screenRenderTexture);
    ClearBackground(BLACK);
    switch (currentScreen) {
        case GAMEPLAY:
            DrawGameplayScreen(false);
            break;
        case LEVELEDITOR:
            DrawLevelEditorScreen();
            break;
        case MAINMENU:
            DrawMainMenuScreen();
            break;
        default:
            break;
    }
    if (onTransition) DrawTransition();
    EndTextureMode();

    BeginDrawing();
    DrawTexturePro(
            screenRenderTexture.texture,
            (Rectangle) {0, 0, (float) (screenRenderTexture.texture.width),
                         (float) (-screenRenderTexture.texture.height)},
            (Rectangle) {0, 0, (float) (finalWidth), (float) (finalHeight)},
            (Vector2) {0, 0},
            0,
            WHITE);
    EndDrawing();
}
