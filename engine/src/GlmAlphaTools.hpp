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

inline glm::vec2 FromAEX(AEVec2& v) {
  return glm::vec2 {v.x, v.y};
}

inline glm::vec3 FromAEX(AEVec3& v) {
  return glm::vec3 {v.x, v.y, v.z};
}

inline glm::mat3 FromAEX(AEMtx33& m) {
  AEVec2 col1, col2, col3, row3;
  // TF is this function @thomas -x
  AEMtx33GetCol(&col1, 0, &m);
  AEMtx33GetCol(&col2, 1, &m);
  AEMtx33GetCol(&col3, 2, &m);
  AEMtx33GetRow(&row3, 2, &m);

  // FUCKING ALPHA ENGINE -x
  // Just gamble and see if the last element is 1 or not -x
  
  return glm::mat3 {
    col1.x, col2.x, col3.x,
    col1.y, col2.y, col3.y,
    row3.x, row3.y, 1.0f
  };
}

inline glm::mat4 FromAEX(AEMtx44& m) {
  return glm::mat4 {
    m.RowCol(0,0), m.RowCol(0,1), m.RowCol(0,2), m.RowCol(0,3),
    m.RowCol(1,0), m.RowCol(1,1), m.RowCol(1,2), m.RowCol(1, 3),
    m.RowCol(2, 0), m.RowCol(2,1), m.RowCol(2,2), m.RowCol(2,3),
    m.RowCol(3, 0), m.RowCol(3,1), m.RowCol(3,2), m.RowCol(3, 3)
  };
}

}
