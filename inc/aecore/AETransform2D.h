// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AETransform2D.h
// Author			:	Thomas Komair
// Creation Date	:	02/18/2016
// Purpose			:	Define Transform 2D class
// ---------------------------------------------------------------------------
#ifndef AE_TRANSFORM_2D_H_
#define AE_TRANSFORM_2D_H_
// ---------------------------------------------------------------------------

struct AETransform2D
{
    AEVec2 mPosition    = {0.0f, 0.0f}; // Position
    AEVec2 mScale       = {1.0f, 1.0f}; // Scale
    float  mOrientation = 0.0f;         // Orientation in radians!
};

// Matrix computation
AEMtx33 AEToMatrix(const AETransform2D * pTransform);
AEMtx33 AEToInvMatrix(const AETransform2D * pTransform);

AETransform2D AEConcat(const AETransform2D * pTransform1, const AETransform2D * pTransform2);
AETransform2D AEInverseConcat(const AETransform2D * pTransform1, const AETransform2D * pTransform2);

// ---------------------------------------------------------------------------
#endif