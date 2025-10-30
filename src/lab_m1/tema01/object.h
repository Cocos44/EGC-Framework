/**
 * @file object2D.h
 * @brief Defines Object class.
 *
 * An object will represent a mesh loaded into GPU memory, coordinates of the
 * center of mesh + color + if the mesh is currently being drawn.
 *
 * @author Grigoras Vlad Andrei.
 */

#pragma once

#include "core/gpu/mesh.h"

#define BOTTOM_LEFT_CORNER glm::vec3(0, 0, 0)

namespace hw1 {

class Object {
   public:
    /**
     * @brief Default constructor. Defaults active to true.
     */
    Object(Mesh* mesh, glm::vec3 position, glm::vec3 color, bool active = true);

   private:
    Mesh* mesh;
    glm::vec3 position;
    glm::vec3 color;
    bool active;

   public:
    Mesh* GetMesh() const { return this->mesh; }
    glm::vec2 GetPosition() const { return this->position; }
    glm::vec3 GetColor() const { return this->color; }
    bool IsActive() const { return this->active; }

    void SetMesh(Mesh* mesh) { this->mesh = mesh; }
    void SetPosition(const glm::vec3& position) { this->position = position; }
    void SetColor(const glm::vec3& color) { this->color = color; }
    void SetActive(const bool& active) { this->active = active; }
};

}  // namespace hw1
