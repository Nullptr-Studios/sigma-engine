// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEUtil.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/02/01
// Purpose			:	header file for utility library
// History			:
// - 2008/02/01		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_UTIL_H
#define AE_UTIL_H

// ---------------------------------------------------------------------------

// Time functions
double AEGetTime(double * pTime = 0);

// random number generator
float AERandFloat();
float AERandFloat(float min, float max);

// ---------------------------------------------------------------------------

#endif // VEC2_H