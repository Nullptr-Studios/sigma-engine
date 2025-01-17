/**
 * \file ActorTest2.h
 * \author Xein <xgonip@gmail.com>
 * \date 1/17/2025
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include <Objects/Actor.hpp>

namespace FNFE {

class ActorTest2 : public Actor {
public:
  explicit ActorTest2(id_t id) : Actor(id) {}
  bool OnMessage(Object *sender) override;
};

}
