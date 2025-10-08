#include "lab_m1/lab1/lab1.h"

#include <cmath>

#include "glm/fwd.hpp"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab1::Lab1() {
    // Initiate with a black background.
    this->backgroundColors = glm::vec3(0, 0, 0);

    // Start background cycle with 1.
    this->backgroundCycle = 1;

    // Start mesh cycle with 0.
    this->meshCycle = 0;

    // Initial position of our mesh.
    this->meshPosition = glm::vec3(-3, 1, 1);
    this->rotationAngle = 0;
}

Lab1::~Lab1() {}

void Lab1::Init() {
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
                                 "primitives"),
                       "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
                                 "primitives"),
                       "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
                                 "primitives"),
                       "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}

void Lab1::FrameStart() {}

void Lab1::ChooseMeshToRender() {
    switch (this->meshCycle) {
        case 1:
            RenderMesh(meshes["box"], this->meshPosition);
            break;
        case 2:
            RenderMesh(meshes["teapot"], this->meshPosition);
            break;
        case 3:
            RenderMesh(meshes["sphere"], this->meshPosition);
            break;
        default:
            RenderMesh(meshes["teapot"], this->meshPosition);
    }
}

void Lab1::Update(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer
    glClearColor(this->backgroundColors.x, this->backgroundColors.y,
                 this->backgroundColors.z, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // You can draw the same mesh any number of times.

    /**
     NOTE: Code for rotating controlled mesh.
     Calculate rotation matrix and update mesh position.

     this->rotationAngle += deltaTimeSeconds * 0.01f;
     this->meshPosition =
        glm::mat3(cos(this->rotationAngle), -1 * sin(this->rotationAngle), 0,
                  sin(this->rotationAngle), cos(this->rotationAngle), 0, 0,
                  0, 1) *
     this->meshPosition;
     */

    this->ChooseMeshToRender();
}

void Lab1::FrameEnd() { DrawCoordinateSystem(); }

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Lab1::OnInputUpdate(float deltaTime, int mods) {
    // Treat continuous update based on input
    if (window->KeyHold(GLFW_KEY_W)) {
        this->meshPosition.z -= 1.0f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        this->meshPosition.x -= 1.0f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        this->meshPosition.z += 1.0f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        this->meshPosition.x += 1.0f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        this->meshPosition.y += 1.0f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        this->meshPosition.y -= 1.0f * deltaTime;
    }
}

glm::vec3 Lab1::GetBackgroundColor() {
    switch (this->backgroundCycle) {
        case 0:
            return m1::BackgroundType::BLACK;
        case 1:
            return m1::BackgroundType::RED;
        case 2:
            return m1::BackgroundType::GREEN;
        case 3:
            return m1::BackgroundType::BLUE;
        default:
            return m1::BackgroundType::BLACK;
    }
}

void Lab1::OnKeyPress(int key, int mods) {
    // Add key press event
    if (key == GLFW_KEY_F) {
        this->backgroundColors = this->GetBackgroundColor();

        if (this->backgroundCycle == 3)
            this->backgroundCycle = 0;
        else
            this->backgroundCycle++;
    }

    if (key == GLFW_KEY_T) {
        if (this->meshCycle == 3) {
            this->meshCycle = 1;
        } else {
            this->meshCycle++;
        }
    }
}

void Lab1::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
}

void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
    // Treat mouse scroll event
}

void Lab1::OnWindowResize(int width, int height) {
    // Treat window resize event
}
