// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEGameStateMgr.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2007/10/26
// Purpose			:	header file for the game state manager
// History			:
// - 2007/10/26		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_GAME_STATE_MGR_H
#define AE_GAME_STATE_MGR_H

// ---------------------------------------------------------------------------

#include "AEEngine.h"

// ---------------------------------------------------------------------------
// defines and enums

#define AE_GS_RESTART 0xFFFFFFFE
#define AE_GS_QUIT    0xFFFFFFFF

// ---------------------------------------------------------------------------
// Getters/Setters

unsigned AEGetGameStateInit();
unsigned AEGetGameStateCurr();
void     AESetGameStateCurr(unsigned state);
unsigned AEGetGameStatePrev();
void     AESetGameStatePrev(unsigned state);
unsigned AEGetGameStateNext();
void     AESetGameStateNext(unsigned state);

// ---------------------------------------------------------------------------
// Function pointer type definition for game state internal functions
typedef void (*AEGameStateFP)();

// ---------------------------------------------------------------------------
// Function prototypes

// invoke current state actions
void AEGameStateMgrCurrentLoad();
void AEGameStateMgrCurrentInit();
void AEGameStateMgrCurrentUpdate();
void AEGameStateMgrCurrentDraw();
void AEGameStateMgrCurrentFree();
void AEGameStateMgrCurrentUnload();

// function to add new game state to the manager
void AEGameStateMgrAdd(unsigned gameStateIdx, void (*pLoad)(), void (*pInit)(), void (*pUpdate)(), void (*pDraw)(), void (*pFree)(), void (*pUnload)());

// call this at the beginning and AFTER all game states are added to the manager
void AEGameStateMgrInit(unsigned gameStateInit);

// update is used to set the function pointers
void AEGameStateMgrUpdate();

// ---------------------------------------------------------------------------

#endif // AE_GAME_STATE_MGR_H