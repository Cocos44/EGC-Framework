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

#include <iostream>

#include "core/gpu/mesh.h"
#include "glm/fwd.hpp"

#define BOTTOM_LEFT_CORNER glm::vec3(0, 0, 0)

namespace hw1 {

class Object {
    friend class Editor;
    friend class SpaceShip;

   private:
    struct AABB {
        glm::vec3 min;
        glm::vec3 max;

        bool IsCollision(const AABB& other) {
            bool isCollisionX =
                this->max.x >= other.min.x && other.max.x >= this->min.x;
            bool isCollisionY =
                this->max.y >= other.min.y && other.max.y >= this->min.y;

            if (isCollisionX && isCollisionY) {
                float overlapX = std::min(this->max.x - other.min.x,
                                          other.max.x - this->min.x);
                float overlapY = std::min(this->max.y - other.min.y,
                                          other.max.y - this->min.y);

                return true;
            }

            return false;
        }
    };

   public:
    /**
     * @brief Default constructor. Defaults active to true.
     */
    Object(Mesh* mesh, glm::vec3 position, glm::vec3 color, bool active = true);

    virtual ~Object() {}

   private:
    Mesh* mesh;

   protected:
    glm::vec3 position;

   private:
    glm::vec3 color;
    bool active;

   protected:
    // NOTE: Only used for objects that are in actual game. (square, rectangle,
    // circle).
    AABB collisionBox;

   public:
    Mesh* GetMesh() const { return this->mesh; }
    glm::vec3 GetPosition() const { return this->position; }
    glm::vec3 GetColor() const { return this->color; }
    bool IsActive() const { return this->active; }
    AABB getCollisionBox() const { return this->collisionBox; }

    void SetMesh(Mesh* mesh) { this->mesh = mesh; }

    virtual void SetPosition(const glm::vec3& position) {
        this->position = position;
    }

    /**
     * @brief Changes color for every vertex and loads new data in GPU.
     * @param color - Color to change mesh to.
     */
    void SetColor(const glm::vec3& color) {
        for (auto& vertex : this->GetMesh()->vertices) vertex.color = color;

        this->GetMesh()->InitFromData(this->GetMesh()->vertices,
                                      this->GetMesh()->indices);
    }

    void SetActive(const bool& active) { this->active = active; }
    void SetCollisionBox(const AABB& collisionBox) {
        this->collisionBox = collisionBox;
    }
};

}  // namespace hw1
