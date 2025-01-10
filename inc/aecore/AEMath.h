// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEMath.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	main header file for the math library
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_MATH_H
#define AE_MATH_H

// ---------------------------------------------------------------------------

#ifndef EPSILON
#define EPSILON 0.00001f
#endif

#ifndef FLOAT_ZERO
#define FLOAT_ZERO(x) (x > -EPSILON && x < EPSILON)
#endif

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

#define HALF_PI (PI * 0.5f)
#define TWO_PI  (PI * 2.0f)

// ---------------------------------------------------------------------------

void MathInit();

// ---------------------------------------------------------------------------

float AEDegToRad(float x);
float AERadToDeg(float x);

float AESin(float x);
float AECos(float x);
float AETan(float x);
float AEASin(float x);
float AEACos(float x);
float AEATan(float x);

#define AESinDeg(x)  AESin(AEDegToRad(x))
#define AECosDeg(x)  AECos(AEDegToRad(x))
#define AETanDeg(x)  AETan(AEDegToRad(x))
#define AEASinDeg(x) AERadToDeg(AEASin(x))
#define AEACosDeg(x) AERadToDeg(AEACos(x))
#define AEATanDeg(x) AERadToDeg(AEATan(x))

// ---------------------------------------------------------------------------

unsigned AEIsPowOf2(unsigned x);
unsigned AENextPowOf2(unsigned x);
unsigned AELogBase2(unsigned x);

float AEClamp(float x, float x0, float x1);
float AEWrap(float x, float x0, float x1);
float AEMin(float x, float y);
float AEMax(float x, float y);
bool  AEInRange(float x, float x0, float x1);

#include "AEVec2.h"
#include "AEVec3.h"
#include "AEMtx33.h"
#include "AEVec3.h"
#include "AEMtx44.h"
#include "AETransform2D.h"
#include "AECollision.h"

// ---------------------------------------------------------------------------

#endif // AE_MATH_H
