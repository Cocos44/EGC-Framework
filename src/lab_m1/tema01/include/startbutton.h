/**
 * @file startbutton.h
 *
 * @brief Declares start button class.
 *
 * Inherits Object and represents the start button.
 *
 * @author Grigoras Vlad Andrei.
 */

#pragma once

#include "lab_m1/tema01/include/object.h"

namespace hw1 {

/**
 * @class StartButton
 *
 * @brief Represent the start button.
 *
 * Inherits Object + contains side length.
 *
 * @see Object
 */
class StartButton : public Object {
   private:
    float leftSideLength;

   public:
    StartButton(Mesh* mesh, glm::vec3 position, glm::vec3 color, float length,
                bool active = true);

    float GetLeftSideLength() const { return this->leftSideLength; }
};

/**
 * @brief Creates a mesh that resembles the starting button.
 * @param name - Name of mesh.
 * @param length - Length of left side.
 * @param color - Color of mesh.
 * @param fill - True if mesh should be filled with color, false otherwise.
 * @return Newly created mesh.
 */
Mesh* CreateStartButton(const std::string& name, float length, glm::vec3 color,
                        bool fill = false);
}  // namespace hw1
