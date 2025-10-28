/**
 * @file object2D.h
 * @brief Header for functions that create / modify / handle meshes.
 * @author Grigoras Vlad Andrei.
 */

#pragma once

#include <string>

#include "core/gpu/mesh.h"

/**
 * @namespace object2D
 * @brief Namespace used to declare different functions to create / handle
 * meshes.
 */
namespace object2D {

/**
 * @brief Creates a mesh that resemblances a square.
 * @param name - Name of mesh.
 * @param leftBottomCorner - Specify where the left bottom corner needs to be.
 * @param length - Length of a side.
 * @param color - Color of mesh.
 * @param fill - True if mesh should be filled with color, false otherwise.
 * @return Newly created mesh.
 */
Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner,
                   float length, glm::vec3 color, bool fill = false);
}  // namespace object2D
