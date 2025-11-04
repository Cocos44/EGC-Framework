#pragma once

#include "glm/ext/quaternion_geometric.hpp"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace implemented {
class Camera {
   public:
    Camera() {
        position = glm::vec3(0, 2, 5);
        forward = glm::vec3(0, 0, -1);
        up = glm::vec3(0, 1, 0);
        right = glm::vec3(1, 0, 0);
        distanceToTarget = 2;
    }

    Camera(const glm::vec3& position, const glm::vec3& center,
           const glm::vec3& up) {
        Set(position, center, up);
    }

    ~Camera() {}

    // Update camera
    void Set(const glm::vec3& position, const glm::vec3& center,
             const glm::vec3& up) {
        this->position = position;
        forward = glm::normalize(center - position);
        right = glm::cross(forward, up);
        this->up = glm::cross(right, forward);
    }

    void MoveForward(float distance) {
        // Translates the camera using the `dir` vector computed from
        // `forward`. Movement will always keep the camera at the same
        // height. For example, if you rotate your head up/down, and then
        // walk forward, then you will still keep the same relative
        // distance (height) to the ground!
        glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
        position += dir * distance;
    }

    void TranslateForward(float distance) {
        this->position += this->forward * distance;
    }

    void TranslateUpward(float distance) {
        this->position += this->up * distance;
    }

    void TranslateRight(float distance) {
        // You need to translate the camera using the `right` vector.
        // Usually, however, translation using camera's `right` vector
        // is not very useful, because if the camera is rotated around the
        // `forward` vector, then the translation on the `right` direction
        // will have an undesired effect, more precisely, the camera will
        // get closer or farther from the ground. The solution is to
        // actually use the projected `right` vector (projected onto the
        // ground plane), which makes more sense because we will keep the
        // same distance from the ground plane.
        glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
        this->position += dir * distance;
    }

    void RotateFirstPerson_OX(float angle) {
        this->forward =
            glm::normalize(glm::rotate(glm::mat4(1.0f), angle, this->right) *
                           glm::vec4(this->forward, 0));
        this->up = glm::normalize(glm::cross(this->right, this->forward));
    }

    void RotateFirstPerson_OY(float angle) {
        this->forward = glm::normalize(
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) *
            glm::vec4(this->forward, 0));
        this->right = glm::normalize(
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) *
            glm::vec4(this->right, 0));
        this->up = glm::normalize(glm::cross(this->right, this->forward));
    }

    void RotateFirstPerson_OZ(float angle) {
        this->right =
            glm::normalize(glm::rotate(glm::mat4(1.0f), angle, this->forward) *
                           glm::vec4(this->right, 0));
        this->up = glm::normalize(glm::cross(this->right, this->forward));
    }

    void RotateThirdPerson_OX(float angle) {
        this->position -= this->forward * this->distanceToTarget;
        this->RotateFirstPerson_OX(angle);
        this->position += this->forward * this->distanceToTarget;
    }

    void RotateThirdPerson_OY(float angle) {
        this->position -= this->forward * this->distanceToTarget;
        this->RotateFirstPerson_OY(angle);
        this->position += this->forward * this->distanceToTarget;
    }

    void RotateThirdPerson_OZ(float angle) {
        this->position -= this->forward * this->distanceToTarget;
        this->RotateFirstPerson_OZ(angle);
        this->position += this->forward * this->distanceToTarget;
    }

    glm::mat4 GetViewMatrix() {
        // Returns the view matrix
        return glm::lookAt(position, position + forward, up);
    }

    glm::vec3 GetTargetPosition() {
        return position + forward * distanceToTarget;
    }

   public:
    float distanceToTarget;
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
};
}  // namespace implemented
