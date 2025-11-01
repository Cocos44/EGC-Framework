/**
 * @file rectangle.h
 *
 * @brief Declares rectangle class.
 *
 * Inherits Object and represents a simple rectangle.
 *
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "lab_m1/tema01/include/object.h"

namespace hw1 {

/**
 * @class Rectangle
 *
 * @brief Represents a rectangle object.
 *
 * Inherits Object + contains side and width length.
 *
 * @see Object
 */
class Rectangle : public Object {
   public:
    /**
     * @brief Default constructor that calls Object constructor. Sets AABB to
     * rectangle form.
     */
    Rectangle(Mesh* mesh, glm::vec3 position, glm::vec3 color, float length,
              float width, bool active = true);

    float GetLength() const { return this->length; }
    float GetWidth() const { return this->width; }

   private:
    float length;
    float width;
};

/**
 * @brief Creates a mesh that resembles a rectangle.
 * @param name - Name of mesh.
 * @param length - Default length of rectangle.
 * @param width - Default width or rectangle.
 * @param color - Color of mesh.
 * @param fill - True if mesh should be filled with color, false otherwise.
 * @return Newly created mesh.
 */
Mesh* CreateRectangle(const std::string& name, float length, float width,
                      glm::vec3 color, bool fill = false);
}  // namespace hw1
