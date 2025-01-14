#include "Collision.hpp"
#include <aecore/AEGraphics.h>
#include <cstdlib>
#include "../Object.hpp"

bool Collision::CircleOnCircle(AEVec2 &pos_a, float radius_a, AEVec2 &pos_b, float radius_b) {
  if (AEVector2Distance(&pos_a, &pos_b) <= radius_a + radius_b) {
#ifdef Debug
        AEGfxLine(pos_a.x, pos_a.y, 0, AE_COLORS_RED, pos_b.x, pos_b.y, 0, AE_COLORS_RED);
#endif
    return true;
  }
  return false;
}

bool Collision::RectOnRect(AEVec2 &pos_a, AEVec2 &scale_a, AEVec2 &pos_b, AEVec2 &scale_b) {
  AEVec2 distance = (pos_a - pos_b) * 2;
  if (fabs(distance.x) <= scale_a.x + scale_b.x && fabs(distance.y) <= scale_a.y + scale_b.y) {
#ifdef Debug
        AEGfxLine(pos_a.x, pos_a.y, 0, AE_COLORS_RED, pos_b.x, pos_b.y, 0, AE_COLORS_RED);
#endif
    return true;
  }
  return false;
}


void Collision::TestCircle() {
  AEVec2 pos_a = {0, 0};
  MousePositionData data = AEGetMouseData();
  AEVec2 pos_b = {data.position.x, data.position.y};
  float radius_a = 50;
  float radius_b = 100;
  unsigned color = AE_COLORS_BLUE;
  if (Collision::CircleOnCircle(pos_a, radius_a, pos_b, radius_b)) {
    color = AE_COLORS_RED;
  }
  DrawCircleAt(pos_a, 20, radius_a * 2, color);
  DrawCircleAt(pos_b, 20, radius_b * 2, color);
}

void Collision::TestRect() {
  AEVec2 pos_a = {0, 0};
  MousePositionData data = AEGetMouseData();
  AEVec2 pos_b = {data.position.x, data.position.y};
  AEVec2 scale_a = {100, 200};
  AEVec2 scale_b = {200, 100};
  unsigned color = AE_COLORS_BLUE;
  if (Collision::RectOnRect(pos_a, scale_a, pos_b, scale_b)) {
    color = AE_COLORS_RED;
  }
  DrawRectangleAt(pos_a, scale_a.x, scale_a.y, color);
  DrawRectangleAt(pos_b, scale_b.x, scale_b.y, color);
}

// :TODO DEBUG DELETE LATER
void DrawRectangleAt(AEVec2 pos, float width, float height, unsigned color) {
  // Draw Four Lines to make the Rectangle
  // Calculate the 4 (x0,y0,x1,y1) points given the width and height, position is 0,0
  // x0, y0 is the top left point
  // x1, y1 is the bottom right point
  pos.x -= width / 2;
  pos.y += height / 2;
  AEGfxLine(pos.x, pos.y, 0, color, pos.x + width, pos.y, 0, color);
  AEGfxLine(pos.x, pos.y, 0, color, pos.x, pos.y - height, 0, color);
  AEGfxLine(pos.x + width, pos.y, 0, color, pos.x + width, pos.y - height, 0, color);
  AEGfxLine(pos.x, pos.y - height, 0, color, pos.x + width, pos.y - height, 0, color);
}


/*!
\brief	Draws a circle at the specified position using AEGfxLine(...).
\param	pos			Position at which to draw the circle.
\param	precision	Number of segments for the circle perimeter.
\param	diameter	Diameter of the circle.
\param	color		Color of the circle.
*/
void DrawCircleAt(AEVec2 pos, int precision, float diameter, unsigned color) {
  // Calculates the points in the circle:
  // 𝑃𝑥 = 𝑟𝑎𝑑𝑖𝑢𝑠 ∗ 𝑐𝑜𝑠(𝜃)
  // 𝑃𝑦 = 𝑟𝑎𝑑𝑖𝑢𝑠 ∗ sin(𝜃)
  // precision indicates number of points in the circle: more points, more polished circle.
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
