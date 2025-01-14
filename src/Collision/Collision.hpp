#pragma once

// Test Functions
void TestCircle();
void TestRect();

// DEBUG
void DrawRectangleAt(AEVec2 pos, float width, float height, unsigned color);
void DrawCircleAt(AEVec2 pos, int precision, float diameter, unsigned color);
class Collision {

public:
  enum Type { ENTER, STAY, EXIT };
  Collision(Type type) : m_type(type) {}

  Type m_type;
  //some reference to object prob

private:
};


/**
 * calculates if 2 rectangles in the 3rd dimension are colliding
 * position relative to center of the rectangle
 * @param pos_a Position of Rectangle A
 * @param scale_a Width and Height and Depth of Rectangle A
 * @param pos_b Position of Rectangle B
 * @param scale_b Width and Height and Depth of Rectangle B
 * @return Info on the collision hopefully soon
 */
Collision *RectOnRect(AEVec3 &pos_a, AEVec3 &scale_a, AEVec3 &pos_b, AEVec3 &scale_b);
