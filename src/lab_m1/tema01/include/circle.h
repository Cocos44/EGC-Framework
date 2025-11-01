/**
 * @file circle.h
 * @brief Declares circle class.
 *
 * Inherits Object and represents the ball used in game.
 *
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "lab_m1/tema01/include/object.h"

namespace hw1 {

/**
 * @class Circle
 * @brief Represents a circle object used as the ball in game.
 *
 * Inherits Object + contains radius, speed and orientation.
 *
 * @see Object
 */
class Circle : public Object {
    friend class Editor;

   private:
    float radius;

    glm::vec2 velocity;

   public:
    /**
     * @brief Default constructor that calls Object constructor. Sets AABB to
     * square form.
     */
    Circle(Mesh* mesh, glm::vec3 position, glm::vec3 color, float radius,
           bool active = true);

    float GetRadius() const { return this->radius; }
    glm::vec2 GetVelocity() const { return this->velocity; }
    float GetXSpeed() const { return this->velocity.x; }
    float GetYSpeed() const { return this->velocity.y; }

    void SetXSpeed(const float& xSpeed) { this->velocity.x = xSpeed; }
    void SetYSpeed(const float& ySpeed) { this->velocity.x = ySpeed; }
    void SetVelocity(const glm::vec2& velocity) { this->velocity = velocity; }
    void SetVelocity(float xSpeed, float ySpeed) {
        this->velocity.x = xSpeed;
        this->velocity.y = ySpeed;
    }
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
                   bool fill = false);
}  // namespace hw1
