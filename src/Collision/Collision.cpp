/**
 * @file Collision.cpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief [Brief description of the file's purpose]
 */
#include "Collision.hpp"

#include <aecore/AEGraphics.h>
#include <aecore/AEVec2.h>
#include <iostream>


bool RectOnRect(AEVec3 &pos_a, AEVec3 &scale_a, AEVec3 &pos_b, AEVec3 &scale_b) {
  AEVec3 distance = (pos_a - pos_b) * 2;
  if ((fabs(distance.x) <= scale_a.x + scale_b.x) && (fabs(distance.y) <= scale_a.y + scale_b.y) &&
      (fabs(distance.z) <= scale_a.z + scale_b.z)) {
    return true;
  }
  return false;
}


#pragma region TEST

void TestRect() {
  AEVec3 pos_a = {0, 0, 0};
  MousePositionData data = AEGetMouseData();
  AEVec3 pos_b = {data.position.x, data.position.y, 0};
  AEVec3 scale_a = {100, 200, 0};
  AEVec3 scale_b = {200, 100, 0};
  unsigned color = AE_COLORS_BLUE;
  if (RectOnRect(pos_a, scale_a, pos_b, scale_b)) {
    color = AE_COLORS_RED;
  }
  DrawRectangleAt({pos_a.x, pos_a.y}, {scale_a.x, scale_a.y}, color);
  DrawRectangleAt({pos_b.x, pos_b.y}, {scale_b.x, scale_b.y}, color);
}

void Print(CollisionType type) {
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
void DrawRectangleAt(AEVec2 pos, AEVec2 scale, unsigned color) {
  pos.x -= scale.x / 2;
  pos.y += scale.y / 2;
  AEGfxLine(pos.x, pos.y, 0, color, pos.x + scale.x, pos.y, 0, color);
  AEGfxLine(pos.x, pos.y, 0, color, pos.x, pos.y - scale.y, 0, color);
  AEGfxLine(pos.x + scale.x, pos.y, 0, color, pos.x + scale.x, pos.y - scale.y, 0, color);
  AEGfxLine(pos.x, pos.y - scale.y, 0, color, pos.x + scale.x, pos.y - scale.y, 0, color);
}

void DrawRectangleAt(AEVec3 pos, AEVec3 scale, unsigned color) {
  DrawRectangleAt({pos.x, pos.y}, {scale.x, scale.y}, color);
}
#pragma endregion
