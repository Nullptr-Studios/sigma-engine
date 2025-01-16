/**
 * @file Pawn.hpp
 * @author a.goi
 * @date 1/16/2025
 *
 * @brief Pawns are Actors that interact with the game world
 */

#pragma once
#include "Actor.hpp"

namespace FNFE {
/**
 * @class Pawn
 * @brief Actors that can interact with the game world
 *
 * Pawns inherit from Actors and have the ability to interact with the world. It mainly adds collision and
 * detection of the NavMesh
 */
class Pawn : public Actor {
public:
  explicit Pawn(id_t id) : Actor(id) {}

  void Init() override;
  void Start() override;
  void Update(double delta) override;
  void Destroy() override;

protected:
  // TODO: @Dante do the collision shits here
};

}
