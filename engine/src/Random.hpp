/**
 * @file Random.hpp
 * @author Dante Harper
 * @date 25/02/25
 *
 * @brief Provides Random Utilitys
 */
#pragma once
#include <glm/vec2.hpp>
namespace Sigma::Random {

/**
 * @brief
 *
 * @param max [TODO:parameter]
 * @param min [TODO:parameter]
 * @return [TODO:return]
 */
int Int(int min, int max);
/**
 * @brief 
 *
 * @param max [TODO:parameter]
 * @param min [TODO:parameter]
 * @return [TODO:return]
 */
float Float(float min, float max);
/**
 * @brief [TODO:description]
 *
 * @param radius [TODO:parameter]
 */
glm::vec2 Circle(float magnitude);

}

