#include "Collision.hpp"
#include <aecore/AEGraphics.h>
#include <aecore/AEVec2.h>
#include "../Object.hpp"



Collision *RectOnRect(AEVec3 &pos_a, AEVec3 &scale_a, AEVec3 &pos_b, AEVec3 &scale_b) {
  AEVec3 distance = (pos_a - pos_b) * 2;
  if ((fabs(distance.x) <= scale_a.x + scale_b.x) && (fabs(distance.y) <= scale_a.y + scale_b.y) &&
      (fabs(distance.z) <= scale_a.z + scale_b.z)) {
    return new Collision(Collision::STAY);
  }
  return nullptr;
}


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
  DrawRectangleAt({pos_a.x, pos_a.y}, scale_a.x, scale_a.y, color);
  DrawRectangleAt({pos_b.x, pos_b.y}, scale_b.x, scale_b.y, color);
}


// Debug drawing
void DrawRectangleAt(AEVec2 pos, float width, float height, unsigned color) {
  pos.x -= width / 2;
  pos.y += height / 2;
  AEGfxLine(pos.x, pos.y, 0, color, pos.x + width, pos.y, 0, color);
  AEGfxLine(pos.x, pos.y, 0, color, pos.x, pos.y - height, 0, color);
  AEGfxLine(pos.x + width, pos.y, 0, color, pos.x + width, pos.y - height, 0, color);
  AEGfxLine(pos.x, pos.y - height, 0, color, pos.x + width, pos.y - height, 0, color);
}
void DrawCircleAt(AEVec2 pos, int precision, float diameter, unsigned color) {
  float radius = diameter / 2;
  float tpx = pos.x + radius;
  float tpy = pos.y;
  for (float angle = 0; angle <= 360; angle += 360 / precision) {
    float radians = angle * PI / 180.0;
    float px = pos.x + radius * cos(radians);
    float py = pos.y + radius * sin(radians);
    AEGfxLine(tpx, tpy, 0, color, px, py, 0, color);
    tpx = px;
    tpy = py;
  }
}
