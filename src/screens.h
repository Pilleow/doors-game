#ifndef SCREENS_H
#define SCREENS_H

#include "constants.h"
#include "entities/player.h"
#include "entities/level.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen {
    UNKNOWN = -1,
    GAMEPLAY = 0,
    LEVELEDITOR
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
extern Sound sfxWin;
extern Music startMusic;
extern Music bgMusic[bgMusicCount];
extern int currentMusicIndex;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Util Functions Declaration
//----------------------------------------------------------------------------------
void shuffleMusicArray(Music arr[], int n);

void shuffleLevelArray(struct Level arr[], int n);

void loadSfxIntoArray(Sound sfxArray[], char filename[], int totalCount, float initVolume);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);

void UpdateGameplayScreen(void);

void DrawGameplayScreen(bool overrideMode);

bool GotoGameplayScreen(void);

void UnloadGameplayScreen(void);

void resetLevel();

void UpdateLevelEditorScreen(void);

void InitLevelEditorScreen(void);

void DrawLevelEditorScreen(void);

bool GotoLevelEditorScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H
