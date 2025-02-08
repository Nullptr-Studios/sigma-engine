//
// Created by dariormt on 08/02/2025.
//

#pragma once

#include "Actor.hpp"

namespace Sigma {

class Damageable : public Sigma::Actor {
  
public:
  explicit Damageable(id_t id) : Actor(id) {}
  ~Damageable() override = default;
  
  void Init() override {Actor::Init();};
  void Start() override {Actor::Start();};
  void Update(double delta) override {Actor::Update(delta);};
  void Draw() override {Actor::Draw();};
  void Destroy() override {Actor::Destroy();};
  
};

} // Sigma

