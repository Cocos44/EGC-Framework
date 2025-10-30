/**
 * @file square.h
 * @brief Declares square class.
 *
 * Inherits Object and represents a simple square.
 *
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "lab_m1/tema01/object.h"

namespace hw1 {

/**
 * @class Square
 * @brief Represents a square object.
 *
 * Inherits Object + contains side length.
 *
 * @see Object
 */
class Square : public Object {
   private:
    float length;

   public:
    /**
     * @brief Default constructor that calls Object constructor.
     */
    Square(Mesh* mesh, glm::vec3 position, glm::vec3 color, float length,
           bool active = true);

    float GetLength() const { return this->length; }
};

/**
 * @brief Creates a mesh that resembles a square.
 * @param name - Name of mesh.
 * @param length - Length of a side.
 * @param color - Color of mesh.
 * @param fill - True if mesh should be filled with color, false otherwise.
 * @return Newly created mesh.
 */
Mesh* CreateSquare(const std::string& name, float length, glm::vec3 color,
                   bool fill = false);
}  // namespace hw1
