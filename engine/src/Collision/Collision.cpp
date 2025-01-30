#include "Collision.hpp"
#include <aecore/AEGraphics.h>
#include <iostream>

namespace FNFE {
bool Collision::RectOnRect(glm::vec3 &posA, glm::vec3 &scaleA, glm::vec3 &posB, glm::vec3 &scaleB) {
  glm::vec3 distance = (posA - posB) * 2.0f;
  if ((fabs(distance.x) <= scaleA.x + scaleB.x) && (fabs(distance.y) <= scaleA.y + scaleB.y) &&
      (fabs(distance.z) <= scaleA.z + scaleB.z)) {
    return true;
  }
  return false;
}

#pragma region TEST

void Collision::TestRect() {
  glm::vec3 posA = {0, 0, 0};
  MousePositionData data = AEGetMouseData();
  glm::vec3 posB = {data.position.x, data.position.y, 0};
  glm::vec3 scaleA = {100, 200, 0};
  glm::vec3 scaleB = {200, 100, 0};
  unsigned color = AE_COLORS_BLUE;
  if (RectOnRect(posA, scaleA, posB, scaleB)) {
    color = AE_COLORS_RED;
  }
  DrawRectangleAt({posA.x, posA.y}, {scaleA.x, scaleA.y}, color);
  DrawRectangleAt({posB.x, posB.y}, {scaleB.x, scaleB.y}, color);
}

void Collision::Print(CollisionType type) {
  switch (type) {
    case CollisionType::ENTER:
      std::cout << "[Collision] Type enter\n";
      break;
    case CollisionType::EXIT:
      std::cout << "[Collision] Type exit\n";
      break;
    case CollisionType::STAY:
      std::cout << "[Collision] Type stay\n";
      break;
  }
}

// Debug drawing
void Collision::DrawRectangleAt(glm::vec2 pos, glm::vec2 scale, unsigned color) {
  pos.x -= scale.x / 2;
  pos.y += scale.y / 2;
  AEGfxRect(pos.x,pos.y,0,scale.x,scale.y,color);
}

void Collision::DrawRectangleAt(glm::vec3 pos, glm::vec3 scale, unsigned color) {
  DrawRectangleAt({pos.x, pos.y}, {scale.x, scale.y}, color);
}

#pragma endregion

} // namespace FNFE
