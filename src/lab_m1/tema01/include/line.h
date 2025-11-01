/**
 * @file line.h
 * @brief Header file containing Line class and line mesh creation method.
 * @author Grigoras Vlad Andrei.
 */

#pragma once

#include "lab_m1/tema01/include/object.h"

namespace hw1 {

/**
 * @class Line
 *
 * @brief Represents a line object. Inherits Object.
 *
 * @see Object
 */
class Line : public Object {
   public:
    /**
     * @brief Default constructor. Calls Object constructor.
     */
    Line(Mesh* mesh, glm::vec3 color, glm::vec3 start, glm::vec3 end,
         bool active = true);

    float GetAngle() const { return angle; }
    float GetLength() const { return length; }

   private:
    glm::vec3 start, end;
    float length;
    float angle;
};

/**
 * @brief Creates a simple horizontal line mesh from (0,0) to (1,0)
 *        that can later be scaled and rotated.
 * @param name Name of the mesh.
 * @param color Line color.
 * @return Pointer to the created mesh.
 */
Mesh* CreateLine(const std::string& name, glm::vec3 color);

}  // namespace hw1
