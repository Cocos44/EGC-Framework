/**
 * @file editor.cpp
 * @brief Implements all methods and logic of Editor class.
 * @see Editor.h
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/editor.h"

#include "glm/fwd.hpp"
#include "lab_m1/tema01/object.h"
#include "lab_m1/tema01/transform2D.h"

hw1::Editor::Editor() {}

hw1::Editor::~Editor() {}

void hw1::Editor::Init() {
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Create logic space
    this->logicSpace = LogicSpace(0, 0, LOGIC_SPACE_WIDTH, LOGIC_SPACE_HEIGHT);

    glm::vec3 corner(0.0f, 0.0f, 0.0f);
    float length = 25.0f;
    Mesh* squareMesh =
        hw1::CreateSquare("square", corner, length, glm::vec3(1, 0, 0), true);
    AddMeshToList(squareMesh);

    glm::vec2 centerPos(corner.x + length / 2.0f, corner.y + length / 2.0f);
    hw1::Object squareObject(squareMesh, centerPos, glm::vec3(1, 0, 0));
    objects.push_back(squareObject);
}

glm::mat3 hw1::Editor::GetSpaceConversionMatrix() {
    float sx, sy, tx, ty;

    sx = this->viewSpace.width / this->logicSpace.width;
    sy = this->viewSpace.height / this->logicSpace.height;
    tx = this->viewSpace.x - sx * this->logicSpace.x;
    ty = this->viewSpace.y - sy * this->logicSpace.y;

    return glm::transpose(
        glm::mat3(sx, 0.0f, tx, 0.0f, sy, ty, 0.0f, 0.0f, 1.0f));
}

void hw1::Editor::SetViewportArea(glm::vec3 colorColor, bool clear) {
    glViewport(this->viewSpace.x, this->viewSpace.y, this->viewSpace.width,
               this->viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(this->viewSpace.x, this->viewSpace.y, this->viewSpace.width,
              this->viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic(
        (float)this->viewSpace.x,
        (float)(this->viewSpace.x + this->viewSpace.width),
        (float)this->viewSpace.y,
        (float)(this->viewSpace.y + this->viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void hw1::Editor::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void hw1::Editor::Update(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area to where to draw.
    this->viewSpace = ViewSpace(0, 0, resolution.x, resolution.y);
    this->SetViewportArea(glm::vec3(0), true);

    // Compute window aspect ratio.
    float windowAspect = resolution.x / float(resolution.y);

    // Adjust logic space to match viewport aspect ratio
    float logicHeight = LOGIC_SPACE_HEIGHT / windowAspect;
    this->logicSpace = LogicSpace(0, 0, LOGIC_SPACE_WIDTH, logicHeight);

    // Compute the 2D visualization matrix
    this->visMatrix = this->GetSpaceConversionMatrix();

    DrawScene();
}

void hw1::Editor::FrameEnd() {}

void hw1::Editor::DrawScene() {
    for (auto& obj : this->objects) {
        if (!obj.isActive()) continue;
        glm::mat3 modelMatrix = visMatrix;
        modelMatrix *=
            transform2D::Translate(obj.getPosition().x, obj.getPosition().y);
        RenderMesh2D(obj.getMesh(), shaders["VertexColor"], modelMatrix);
    }
}
