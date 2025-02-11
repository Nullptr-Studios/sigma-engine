//
// Created by dariormt on 07/02/2025.
//

#include "GameScene.hpp"

#include "Factory.hpp"
#include "Polygon.hpp"
#include "pch.hpp"


void Sigma::GameScene::Load() {
  Sigma::Scene::Load();
  if (m_jsonPath.empty())
    return;

  std::fstream file(m_jsonPath);

  if (!file.is_open()) {
    std::cout << "[GameScene] " << GetName() << " failed to open JSON file " << m_jsonPath << '\n';
    return;
  }

  std::cout << "[GameScene] " << GetName() << " Loading JSON file: " << m_jsonPath << '\n';

  json_t J = json_t::parse(file);

  m_playerStartPos = {J["playerStart"]["x"], J["playerStart"]["y"]};

  m_sceneBounds.reserve(J["bounds"].size());

  for (auto boundCoords: J["bounds"]) {
    glm::vec2 bound = {boundCoords["x"], boundCoords["y"]};
    m_sceneBounds.emplace_back(bound);

    if (m_debug) {
      auto point = GET_FACTORY->CreateObject<Actor>("Point");
      point->transform.position.x = bound.x;
      point->transform.position.y = bound.y;
      point->transform.position.z = 1000;
      point->transform.scale = {10, 10};
      point->SetModulationColor(AE_COLORS_RED);
    }
  }

  std::cout << "[GameScene] " << GetName() << " JSON file loaded\n";

  J.clear();

  file.close();

  m_sceneBoundsPoly = new Polygon(m_sceneBounds);
}
