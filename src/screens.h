#ifndef SCREENS_H
#define SCREENS_H

#include "constants.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen {
    UNKNOWN = -1,
    GAMEPLAY = 0,
    LEVELEDITOR,
} GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Sound sfxShoot[sfxShootCount];
extern Sound sfxDead[sfxDeadCount];
extern Sound sfxHit[sfxHitCount];
extern Sound sfxDoorOpen[sfxDoorOpenCount];
extern Sound sfxDash[sfxDashCount];

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Util Functions Declaration
//----------------------------------------------------------------------------------
void shuffleArray(Music arr[], int n);
void loadSfxIntoArray(Sound sfxArray[], char filename[], int totalCount, float initVolume);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
bool GotoGameplayScreen(void);

void UpdateLevelEditorScreen(void);
void InitLevelEditorScreen(void);
void DrawLevelEditorScreen(void);
bool GotoLevelEditorScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H
