/**
 * \file ActorTest1.h
 * \author Xein <xgonip@gmail.com>
 * \date 1/17/2025
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include <Objects/Actor.hpp>

namespace FNFE {

class ActorTest1 : public Actor {

public:
  explicit ActorTest1(id_t id) : Actor(id) {}

  void Update(double delta) override;
};

}
