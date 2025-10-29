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

class Square : public hw1::Object {
   private:
    float length;

   public:
    Square(Mesh* mesh, glm::vec3 position, glm::vec3 color, float length,
           bool active = true);
    float getLength() const { return this->length; }
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
