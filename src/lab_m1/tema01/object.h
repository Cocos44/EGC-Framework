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
 * meshes + Object class.
 *
 * Object class represents a simple mesh loaded into memory and scene. Contains
 * information about position, size color and if it is rendered on scree.
 */
namespace hw1 {

class Object {
   public:
    /**
     * @brief Default constructor. Defaults active to true.
     */
    Object(Mesh* mesh, glm::vec2 position, glm::vec3 color, bool active = true);

   private:
    Mesh* mesh;
    glm::vec2 position;
    glm::vec3 color;
    bool active;

   public:
    Mesh* getMesh() const { return this->mesh; }
    glm::vec2 getPosition() const { return this->position; }
    glm::vec3 getColor() const { return this->color; }
    bool isActive() const { return this->active; }
};

/**
 * @brief Creates a mesh that resembles a square.
 * @param name - Name of mesh.
 * @param leftBottomCorner - Specify where the left bottom corner needs to
 * be.
 * @param length - Length of a side.
 * @param color - Color of mesh.
 * @param fill - True if mesh should be filled with color, false otherwise.
 * @return Newly created mesh.
 */
Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner,
                   float length, glm::vec3 color, bool fill = false);
}  // namespace hw1
