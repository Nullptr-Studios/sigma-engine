// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEVector2.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the 2D vector library
// History			:
// - 2008/01/31		:	- initial implementation
// - 2012/02/15		:	- added a point2d typedef
// ---------------------------------------------------------------------------

#ifndef AE_VEC2_H
#define AE_VEC2_H
// ---------------------------------------------------------------------------

#pragma warning(disable : 4201) // nameless struct warning
struct AEVec2
{
    union
    {
        struct
        {
            float x, y;
        };
        float v[2];
    };

    // ------------------------------------------------------------------------
    AEVec2();
    AEVec2(const AEVec2 & rhs);
    AEVec2(float xx, float yy);
    explicit AEVec2(float xx); //! flag as explicit to prevent implicit conversion from float to AEVec2

    // add or substract
    AEVec2 operator+(const AEVec2 & rhs) const;
    AEVec2 operator+=(const AEVec2 & rhs);
    AEVec2 operator-(const AEVec2 & rhs) const;
    AEVec2 operator-=(const AEVec2 & rhs);

    // multiply or divide by a scalar
    AEVec2 operator*(float s) const;
    AEVec2 operator*=(float s);
    AEVec2 operator/(float s) const;
    AEVec2 operator/=(float s);

    // unitary negation
    AEVec2 operator-() const;

    // vector length
    float Length() const;
    float LengthSq() const;

    // Distance
    float Distance(const AEVec2 & rhs);
    float DistanceSq(const AEVec2 & rhs);

    // Normalize
    AEVec2         Normalize() const;
    const AEVec2 & NormalizeThis();

    // dot product
    float Dot(const AEVec2 & rhs) const;
    float operator*(const AEVec2 & rhs) const;

    // perpendicular, projection & perpendiculat projection
    AEVec2 Perp() const;
    AEVec2 Project(const AEVec2 & rhs) const;
    AEVec2 ProjectPerp(const AEVec2 & rhs) const;

    // cross product magnitude
    float CrossMag(const AEVec2 & rhs);

    // To and From Angle
    float GetAngle();
    void  FromAngle(float angle);
    void  RotateBy(float angle_rad);
    void  RotateByDeg(float angle_deg);

    // Static interface
    static AEVec2 Random(float min_x, float max_x, float min_y, float max_y);
    static AEVec2 Random01();
    static AEVec2 Lerp(AEVec2 start, AEVec2 end, float tn);

    static const AEVec2 ZERO;
};

// ---------------------------------------------------------------------------

void AEVector2Zero(AEVec2 * pResult);
void AEVector2Set(AEVec2 * pResult, float x, float y);

void AEVector2Neg(AEVec2 * pResult, AEVec2 * pVec0);
void AEVector2Add(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1);
void AEVector2Sub(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1);
void AEVector2Normalize(AEVec2 * pResult, AEVec2 * pVec0);
void AEVector2Perp(AEVec2 * pResult, AEVec2 * pVec0);

void AEVector2Scale(AEVec2 * pResult, AEVec2 * pVec0, float c);
void AEVector2ScaleAdd(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1, float s);
void AEVector2ScaleSub(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1, float s);

void AEVector2Project(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1);
void AEVector2ProjectPerp(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1);

void AEVector2Lerp(AEVec2 * pResult, AEVec2 * pVec0, AEVec2 * pVec1, float t);

float AEVector2Length(AEVec2 * pVec0);
float AEVector2SquareLength(AEVec2 * pVec0);
float AEVector2Distance(AEVec2 * pVec0, AEVec2 * pVec1);
float AEVector2SquareDistance(AEVec2 * pVec0, AEVec2 * pVec1);

float AEVector2DotProduct(AEVec2 * pVec0, AEVec2 * pVec1);
float AEVector2CrossProductMag(AEVec2 * pVec0, AEVec2 * pVec1);

void  AEVector2FromAngle(AEVec2 * pResult, float angle);
float AEVector2ToAngle(AEVec2 * pVec);

#pragma warning(default : 4201) // nameless struct warning

// ---------------------------------------------------------------------------

#endif // VEC2_H