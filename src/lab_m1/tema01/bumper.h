/**
 * @file bumper.h
 * @brief Defines Bumper class and method to create a bumper mesh.
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "lab_m1/tema01/object.h"

namespace hw1 {

class Bumper : public hw1::Object {
   public:
    Bumper(Mesh* mesh, glm::vec3 position, glm::vec3 color, bool active = true);
};

Mesh* CreateBumper(std::string name, float squareLength, glm::vec3 squareColor,
                   glm::vec3 semicircleColor, bool fill = true);

}  // namespace hw1
