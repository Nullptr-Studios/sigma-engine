/**
 * @file Character.hpp
 * @author a.goi
 * @date 1/16/2025
 *
 * @brief Characters are Pawns that can be controlled by the player
 */

#pragma once
#include "Pawn.hpp"

namespace FNFE {

/**
 * @class Character
 * @brief A Pawn that can be controlled by the Player
 *
 * Characters inherit from @c Pawns and can be controlled by the player. They also introduce combo tracking and another
 * combat features.
 */
class Character : public Pawn {
public:
  explicit Character(id_t id) : Pawn(id) {}

  void Init() override;
  void Start() override;
  void Update(double delta) override;
  void Destroy() override;

private:
  unsigned char m_defCombo = 0;    ///< @brief Combo status for default attack
  unsigned char m_superCombo = 0;  ///< @brief Combo status for super attack
  unsigned char m_ultCombo = 0;    ///< @brief Combo status for ultimate attack

};

}
