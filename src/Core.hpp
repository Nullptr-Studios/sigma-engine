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

#define FNFE_FACTORY Factory::GetInstance() ///< @brief Major version number

typedef nlohmann::json json_t;

}
