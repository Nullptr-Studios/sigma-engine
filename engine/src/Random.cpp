#include <glm/vec2.hpp>
#include "Random.hpp"

namespace Sigma {

int Random::Int(int min, int max) {
  int spread = max-min;
  return (std::rand()%spread) + min;
  
}
float Random::Float(float min, float max) {
  float spread = max-min;
  return (std::rand()/ RAND_MAX) * spread + min;
}

glm::vec2 Random::Circle(float magnitude) {
  float angle = Random::Float(0,360);
  return {cos(angle)*magnitude,sin(angle)*magnitude};
}


}

