// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEMtx33.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the 3x3 matrix library
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AEX_VEC3_H
#define AEX_VEC3_H
// ---------------------------------------------------------------------------

#pragma warning(disable : 4201) // nameless struct warning
struct AEVec3
{
    union
    {
        struct
        {
            float x, y, z;
        };
        float v[3];
    };

    [[deprecated("Use glm::vec3 instead")]]
    AEVec3();
    [[deprecated("Use glm::vec3 instead")]]
    AEVec3(const AEVec3 & rhs);
    [[deprecated("Use glm::vec3 instead")]]
    AEVec3(float xx, float yy, float zz);
    [[deprecated("Use glm::vec3 instead")]]
    explicit AEVec3(float xx);
    ;
    // Operators
    // add or substract										;
    AEVec3 operator+(const AEVec3 & rhs) const;
    AEVec3 operator+=(const AEVec3 & rhs);
    AEVec3 operator-(const AEVec3 & rhs) const;
    AEVec3 operator-=(const AEVec3 & rhs);
    ;
    // multiply or divide by a scalar						;
    AEVec3 operator*(float s) const;
    AEVec3 operator/(float s) const;
    ;
    // comparison
    // fucking alpha engine didn't have this -x
    bool operator==(const AEVec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const AEVec3& other) const {
        return x != other.x || y != other.y || z != other.z;
    }
    ;
    // get vector length									;
    float Length() const;
    ;
    // calculate normalized vector							;
    AEVec3         Normalize() const;
    const AEVec3 & NormalizeThis();
    ;
    // dot product											;
    float operator*(const AEVec3 & rhs) const;
    ;
    // cross product										;
    AEVec3 Cross(const AEVec3 & rhs) const;

    static const AEVec3 ZERO;
};

// ---------------------------------------------------------------------------

template <typename T>
AEVec3 operator*(T s, const AEVec3 & rhs)
{
    return (rhs * s);
}

#pragma warning(default : 4201) // nameless struct warning
// ---------------------------------------------------------------------------
#endif