/**
 * @file circle.h
 * @brief Declares circle class.
 *
 * Inherits Object and represents a simple circle.
 *
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "lab_m1/tema01/include/object.h"

namespace hw1 {

/**
 * @class Circle
 * @brief Represents a circle object.
 *
 * Inherits Object + contains radius.
 *
 * @see Object
 */
class Circle : public Object {
   private:
    float radius;
    float xSpeed;
    float ySpeed;

   public:
    /**
     * @brief Default constructor that calls Object constructor. Sets AABB to
     * square form.
     */
    Circle(Mesh* mesh, glm::vec3 position, glm::vec3 color, float radius,
           bool active = true);

    float GetRadius() const { return this->radius; }
};

/**
 * @brief Creates a mesh that resembles a circle.
 * @param name - Name of mesh.
 * @param length - Length of a side.
 * @param color - Color of mesh.
 * @param fill - True if mesh should be filled with color, false otherwise.
 * @return Newly created mesh.
 */
Mesh* CreateCircle(const std::string& name, float radius, glm::vec3 color,
                   bool fill);
}  // namespace hw1
