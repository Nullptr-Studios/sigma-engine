// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEFrameRateController.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2007/04/26
// Purpose			:	header file for the frame rate controller
// History			:
// - 2007/04/26		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_FRAME_RATE_CONTROLLER_H
#define AE_FRAME_RATE_CONTROLLER_H

// ---------------------------------------------------------------------------
// Getters/Setters

// frame controller related variables
void     AESetFrameRateMax(double newMax); // set clamp the frame rate to at most this number
double   AEGetFrameRateMax();              // get clamp the frame rate to at most this number
double   AEGetFrameRate();                 // get the frame rate based on the last frame
double   AEGetFrameTime();                 // get time taken to process the last frame(in seconds)
unsigned AEGetFrameCounter();              // get number of frame since the last reset

// ---------------------------------------------------------------------------
// Function prototypes

// Frame management
bool AEFrameRateControllerInit();
void AEFrameRateControllerTerminate();
void AEFrameRateControllerReset();
void AEFrameRateControllerStart();
void AEFrameRateControllerEnd();

// ---------------------------------------------------------------------------

#endif // AE_FRAME_RATE_CONTROLLER_H
