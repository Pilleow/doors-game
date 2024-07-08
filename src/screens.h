#ifndef SCREENS_H
#define SCREENS_H

#include "constants.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, GAMEPLAY = 0 } GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Music bgMusic[bgMusicCount];
extern Sound sfxShoot[sfxShootCount];
extern Sound sfxDead[sfxDeadCount];
extern Sound sfxHit[sfxHitCount];

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H