#include "StateManager.hpp"
#include "Core.hpp"
#include "GameManager.hpp"

namespace FNFE {

GameState StateManager::GetCurrent() {
  return FNFE_MANAGER->m_gameState;  
}

void StateManager::SetCurrent(GameState state) {
  FNFE_MANAGER->m_gameState = state;
}

}
