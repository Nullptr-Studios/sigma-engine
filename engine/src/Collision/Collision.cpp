#include "Collision.hpp"
#include <aecore/AEGraphics.h>
#include <aecore/AEVec2.h>
#include <iostream>

namespace FNFE {
bool Collision::RectOnRect(AEVec3 &posA, AEVec3 &scaleA, AEVec3 &posB, AEVec3 &scaleB) {
  AEVec3 distance = (posA - posB) * 2;
  if ((fabs(distance.x) <= scaleA.x + scaleB.x) && (fabs(distance.y) <= scaleA.y + scaleB.y) &&
      (fabs(distance.z) <= scaleA.z + scaleB.z)) {
    return true;
  }
  return false;
}


#pragma region TEST

void Collision::TestRect() {
  AEVec3 posA = {0, 0, 0};
  MousePositionData data = AEGetMouseData();
  AEVec3 posB = {data.position.x, data.position.y, 0};
  AEVec3 scaleA = {100, 200, 0};
  AEVec3 scaleB = {200, 100, 0};
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
      std::cout << "Enter\n";
      break;
    case CollisionType::EXIT:
      std::cout << "Exit\n";
      break;
    case CollisionType::STAY:
      std::cout << "Stay\n";
      break;
  }
}

// Debug drawing
void Collision::DrawRectangleAt(AEVec2 pos, AEVec2 scale, unsigned color) {
  pos.x -= scale.x / 2;
  pos.y += scale.y / 2;
  AEGfxRect(pos.x,pos.y,0,scale.x,scale.y,color);
}

void Collision::DrawRectangleAt(AEVec3 pos, AEVec3 scale, unsigned color) {
  DrawRectangleAt({pos.x, pos.y}, {scale.x, scale.y}, color);
}
#pragma endregion
} // namespace FNFE
