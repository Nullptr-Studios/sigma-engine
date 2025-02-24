/**
 * @file Pch.hpp
 * @author Dario
 * @date 12/01/2025
 *
 * @brief Precompiled header file
 */

#pragma once

// C++ standard libraries (AEEngine.h already includes most of these)
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

// Alpha Engine includes
#include <aecore/AEEngine.h>
#include <core.hpp>

// FMOD includes
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_studio.hpp>

// json
#include <json.hpp>

// Bitwise
#define BIT(x) (1 << x)
