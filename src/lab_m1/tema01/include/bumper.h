/**
 * @file bumper.h
 * @brief Defines Bumper class and method to create a bumper mesh.
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "lab_m1/tema01/include/object.h"

namespace hw1 {

/**
 * @class Bumper
 *
 * @brief Declares a bumper object. Inherits Object.
 *
 * @see Object
 */
class Bumper : public Object {
   public:
    Bumper(Mesh* mesh, glm::vec3 position, glm::vec3 color, bool active = true);
};

/**
 * @brief Creates a mesh that resembles a bumper
 *
 * @param name - Name of mesh.
 * @param squareLength - Length of a square side.
 * @param squareColor - Color of base square.
 * @param semicirceColor - Color of semicircle.
 * @param fill - True if mesh should be filled with color, false otherwise.
 *
 * @return Newly created mesh.
 */
Mesh* CreateBumper(std::string name, float squareLength, glm::vec3 squareColor,
                   glm::vec3 semicircleColor, bool fill = true);

}  // namespace hw1
