#pragma once

#include "components/simple_scene.h"
#include "glm/fwd.hpp"

namespace m1 {
class Lab1 : public gfxc::SimpleScene {
   public:
    Lab1();
    ~Lab1();

    void Init() override;

   private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button,
                           int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX,
                       int offsetY) override;
    void OnWindowResize(int width, int height) override;

    // Student fields + methods.
   private:
    glm::vec3 backgroundColors;

    // Field used to cycle through meshes ("BOX", "SPHERE", "TEAPOT").
    int meshCycle;

    // Field used to cycle through background colors ("BLACK", "RED", "GREEN",
    // "BLUE").
    int backgroundCycle;

    // Position of our controlled mesh.
    glm::vec3 meshPosition;

    // Used for rotating mesh.
    float rotationAngle;

   private:
    /**
     * @brief Choose a mesh based on the meshCycle field and render it.
     * @see m1::Lab1->meshCycle
     */
    void ChooseMeshToRender();

    /**
     * @brief Choose a mesh based on the backgroundCycle field.
     * @return 3Vector containing background color.
     */
    glm::vec3 GetBackgroundColor();
};

/**
 * @namespace BackgroundType
 * @brief Contains constant vectors used to choose a background color.
 */
namespace BackgroundType {
static const glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 RED = glm::vec3(1.0f, 0.0f, 0.0f);
static const glm::vec3 GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 BLUE = glm::vec3(0.0f, 0.0f, 1.0f);
};  // namespace BackgroundType
}  // namespace m1
