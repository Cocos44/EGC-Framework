#include "lab_m1/lab3/lab3.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "lab_m1/lab3/object2D.h"
#include "lab_m1/lab3/transform2D.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab3::Lab3() {}

Lab3::~Lab3() {}

void Lab3::Init() {
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y,
                            0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // Square center x coordinate.
    this->cx = corner.x + squareSide / 2;

    // Square center y coordinate.
    this->cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide,
                                           glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide,
                                           glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide,
                                           glm::vec3(0, 0, 1));

    AddMeshToList(square3);

    float base = 100;
    float height = 100;

    Mesh* triangle = object2D::CreateTriangle("triangle", corner, base, height,
                                              glm::vec3(1, 1, 0), true);

    AddMeshToList(triangle);
}

void Lab3::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab3::Update(float deltaTimeSeconds) {
    this->modelMatrix = glm::mat3(1);

    this->translateX += deltaTimeSeconds * 100;
    this->translateY += deltaTimeSeconds * 100;

    if (this->translateX > window->GetResolution().x ||
        this->translateY > window->GetResolution().y) {
        this->translateX = this->translateY = 0;
    }

    modelMatrix *= transform2D::Translate(this->translateX, this->translateY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], this->modelMatrix);

    this->modelMatrix = glm::mat3(1);

    this->scaleX += deltaTimeSeconds * 1.5f;
    this->scaleY += deltaTimeSeconds * 1.5f;

    this->modelMatrix = glm::mat3(1);
    this->modelMatrix *= transform2D::Translate(400, 250);
    this->modelMatrix *= transform2D::Translate(this->cx, this->cy);
    this->modelMatrix *= transform2D::Scale(this->scaleX, this->scaleY);
    this->modelMatrix *= transform2D::Translate(-this->cx, -this->cy);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], this->modelMatrix);

    modelMatrix = glm::mat3(1);

    this->angularStep += deltaTimeSeconds * 1.5f;

    this->modelMatrix *= transform2D::Translate(650, 250);
    this->modelMatrix *= transform2D::Translate(this->cx, this->cy);
    this->modelMatrix *= transform2D::Rotate(this->angularStep);
    this->modelMatrix *= transform2D::Translate(-this->cx, -this->cy);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], this->modelMatrix);

    float base = 100;
    float height = 100;

    for (int i = 0; i < 5; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(650, 250);
        modelMatrix *= transform2D::Rotate(i * 3.14159f / 2.5f);
        modelMatrix *= transform2D::Translate(-base / 2, 0);
        modelMatrix *= transform2D::Translate(0, 60);
        RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);
    }
}

void Lab3::FrameEnd() {}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Lab3::OnInputUpdate(float deltaTime, int mods) {}

void Lab3::OnKeyPress(int key, int mods) {
    // Add key press event
}

void Lab3::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
}

void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Lab3::OnWindowResize(int width, int height) {}
