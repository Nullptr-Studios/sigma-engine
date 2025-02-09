//
// Created by dariormt on 07/02/2025.
//

#pragma once

#include "Scene.hpp"

#include <string>

#include <vector>
#include "glm/vec2.hpp"


namespace Sigma {
class Polygon;

/**
 * @brief Game Scene class
 */
class GameScene : public Sigma::Scene {

public:
  
  /**
   * @brief Construct a new Game Scene object
   *
   * @param name
   * @param ID
   * @param jsonPath
   */
  GameScene(const char *name, unsigned ID, const char *jsonPath) : Scene(name, ID), m_jsonPath(jsonPath) {};

  void Load() override;

  /**
   * @brief returns the current scene bounds
   * @return Polygon*
   */
  [[nodiscard]] Polygon *GetSceneBoundsPoly() const { return m_sceneBoundsPoly; }

private:
  bool m_debug = false;

  std::string m_jsonPath;

  glm::vec2 m_playerStartPos;

  std::vector<glm::vec2> m_sceneBounds;

  Sigma::Polygon *m_sceneBoundsPoly;
};

} // namespace Sigma
