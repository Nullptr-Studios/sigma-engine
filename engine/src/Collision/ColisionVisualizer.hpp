//
// Created by Dario on 12/02/2025.
//

#pragma once

#include "Factory.hpp"
#include "Objects/Actor.hpp"



class ColisionVisualizer final : public Sigma::Actor {
public:
  explicit ColisionVisualizer(Sigma::id_t id, glm::vec3 pos, glm::vec2 scale, float time) : Actor(id), m_pos(pos), m_scale(scale), m_time(time) {}

  void Init() override {
    Actor::Init();
    transform.position = m_pos;
    transform.scale = m_scale;
  }
  void Update(double deltaTime) override {
    Actor::Update(deltaTime);
    m_timer += deltaTime;
    if (m_timer >= m_time) {
      m_timer = 0.0f;
      GET_FACTORY->DestroyObject(this);
    }
  }

private:
  glm::vec3 m_pos;
  glm::vec2 m_scale;
  float m_time;
  float m_timer = 0.0f;
};



