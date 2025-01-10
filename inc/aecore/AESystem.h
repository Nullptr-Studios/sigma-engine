// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AESystem.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the system module
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_SYSTEM_H
#define AE_SYSTEM_H

// ---------------------------------------------------------------------------

// window related variables getters/setters
HWND     AEGetWindowHandler();
bool     AESysWindowExists();
unsigned AEGetSysWindowWidth();
unsigned AEGetSysWindowHeight();
void     AESetSysWindowWidth(unsigned w);
void     AESetSysWindowHeight(unsigned h);

// ---------------------------------------------------------------------------

bool AESysInit(const char * title = NULL, int win_width = 800, int win_height = 600);
void AESysReset();
void AESysUpdate();
void AESysExit();
void AESysShowConsole();
void AESysHideConsole();
void AESysGameLoop();

// ---------------------------------------------------------------------------

void AESysFrameStart();
void AESysFrameEnd();

// ---------------------------------------------------------------------------

#endif // AE_SYSTEM_H
