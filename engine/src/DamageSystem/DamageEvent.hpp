/**
 * @file DamageEvent.h
 * @author Alexey
 * @date 02/04/2025
 *
 * @brief Damage event class for damage system
 */

// LOOK HOW XEIN DOES COLLISION EVENT
// FIGURE OUT EVENT SYSTEM
// https://git.xein.es/nullptr-studios/sigma-engine/-/blob/collisions-revamped/engine/src/Collision/CollisionEvent.hpp?ref_type=heads
// https://git.xein.es/nullptr-studios/sigma-engine/-/blob/collisions-revamped/engine/src/GameManager.cpp?ref_type=heads
// JUST COPY THE COLLISION EVENT COLLISION EVENT CLASS CODE --> CHANGE IT A BIT!

//Things todo:
/*
 *  variables:
 *   Float damageAmount
 *   Object * damageDealer
 *   Id_T damageReceiver
 *   Enum damageType --> Physical (Basic) or Throw, with Throw damage being based on:
 *                                          - Float Height (Diff between Y and Z coordinates)
 *                                          - Gravity (Vertical deceleration)
 *  functions:
 *    Constructor (with all 4 variables) --> fills them up
 *    Destructor()
 *    ApplyDamage() --> applies damageAmount damage to damageReceiver --> diff formulas for diff damageType
 */

#pragma once
#include "Events/Event.hpp"

namespace FNFE {

class DamageEvent : public Event {



};

}