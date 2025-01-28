/**
 * @file GlmAlphaTools.hpp
 * @author Xein
 * @date 1/27/2025
 *
 * @brief Interface to use the GLM library with Alpha Engine
 */

#pragma once
#include <aecore/AEMtx33.h>
#include <aecore/AEMtx44.h>
#include <aecore/AEVec2.h>
#include <aecore/AEVec3.h>

#include <glm/glm.hpp>

namespace glm {

inline AEMtx33 ToAEX(mat3 &m) {
  return AEMtx33 {
    m[0][0], m[0][1], m[0][2], 
    m[1][0], m[1][1], m[1][2],
    m[2][0], m[2][1], m[2][2]
  };
}

inline AEMtx44 ToAEX(mat4 &m) {
  return AEMtx44 {
    m[0][0], m[0][1], m[0][2], m[0][3],
    m[1][0], m[1][1], m[1][2], m[1][3],
    m[2][0], m[2][1], m[2][2], m[2][3],
    m[3][0], m[3][1], m[3][2], m[3][3]
  };
}

inline AEVec2 ToAEX(vec2 &v) {
  return AEVec2 {v.x, v.y};
}

inline AEVec3 ToAEX(vec3& v) {
  return AEVec3 {v.x, v.y, v.z};
}


}
