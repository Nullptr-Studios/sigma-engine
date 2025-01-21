/**
 * @file Core.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <pch.hpp>

namespace FNFE {

typedef unsigned int id_t; ///< @brief Type definition for object IDs

#define FNFE_FACTORY Factory::GetInstance() ///< @def FNFE_FACTORY @brief Gets Factory instance
#define FNFE_MANAGER GameManager::GetInstance() ///< @def FNFE_MANAGER @brief Gets Manager instance

typedef nlohmann::json json_t;

}
