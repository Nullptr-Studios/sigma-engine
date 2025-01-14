#pragma once
#include "../Object.hpp"
using namespace FNFE;
namespace Collision {

bool RectOnRect(AEVec2 &pos_a, AEVec2 &scale_a, AEVec2 &pos_b, AEVec2 &scale_b);
bool CircleOnCircle(AEVec2 &pos_a, float radius_a, AEVec2 &pos_b, float radius_b);

void TestCircle();
void TestRect();
} // namespace Collision


// TODO: DEBUG DELETE LATER
void DrawRectangleAt(AEVec2 pos, float width, float height, unsigned color);
void DrawCircleAt(AEVec2 pos, int precision, float diameter, unsigned color);
