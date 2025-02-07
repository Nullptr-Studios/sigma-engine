//
// Created by dariormt on 07/02/2025.
//

#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace Sigma {

class Polygon {

public:
  /**
   * @brief Construct a new Polygon object from a list of points
   *
   * @param points The list of points that define the polygon
   */
  Polygon(const std::vector<glm::vec2> &points) : vertices(points) {
    computeBounds();
    precomputeSlopes();
  }

  /**
   * @brief Check if a point is inside the polygon
   *
   * @param testPoint The point to check
   * @return true if the point is inside the polygon, false otherwise
   */
  bool isPointInside(const glm::vec2 &testPoint) const {
    // Bounding box check
    if (testPoint.x < minBounds.x || testPoint.x > maxBounds.x || testPoint.y < minBounds.y ||
        testPoint.y > maxBounds.y)
      return false;

    int intersections = 0;
    size_t n = vertices.size();

    for (size_t i = 0; i < n; i++) {
      glm::vec2 p1 = vertices[i];
      glm::vec2 p2 = vertices[(i + 1) % n];

      if ((p1.y > testPoint.y) != (p2.y > testPoint.y)) { // Edge crosses testPoint.y
        float x_intersect = p1.x + (testPoint.y - p1.y) * invSlopes[i];

        if (testPoint.x < x_intersect) // Point is to the left of the intersection
          intersections++;
      }
    }

    return (intersections % 2) == 1; // Odd = inside, Even = outside
  }

private:
  
  std::vector<glm::vec2> vertices;
  glm::vec2 minBounds, maxBounds;
  std::vector<float> invSlopes; // Precomputed 1/slope for fast intersection tests

  //* Compute the bounding box of the polygon
  void computeBounds() {
    minBounds.x = maxBounds.x = vertices[0].x;
    minBounds.y = maxBounds.y = vertices[0].y;
    for (const auto &p: vertices) {
      if (p.x < minBounds.x)
        minBounds.x = p.x;
      if (p.x > maxBounds.x)
        maxBounds.x = p.x;
      if (p.y < minBounds.y)
        minBounds.y = p.y;
      if (p.y > maxBounds.y)
        maxBounds.y = p.y;
    }
  }

  //* Precompute the slopes of the edges for fast intersection tests
  void precomputeSlopes() {
    size_t n = vertices.size();
    invSlopes.resize(n);
    for (size_t i = 0; i < n; i++) {
      glm::vec2 p1 = vertices[i];
      glm::vec2 p2 = vertices[(i + 1) % n];
      invSlopes[i] = (p2.y != p1.y) ? (p2.x - p1.x) / (p2.y - p1.y) : 0.0f;
    }
  }
};

} // namespace Sigma
