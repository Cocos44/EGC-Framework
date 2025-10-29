/**
 * @file editor.cpp
 * @brief Implements all methods and logic of Editor class.
 *
 * Handles scene rendering, transitioning from logic space to view space and
 * spaceship configuration logic.
 *
 * @see Editor.h
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/editor.h"

hw1::Editor::Editor() {}

hw1::Editor::~Editor() {}

void hw1::Editor::Init() {
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Create logic space.
    this->logicSpace =
        hw1::Editor::LogicSpace(0, 0, LOGIC_SPACE_WIDTH, LOGIC_SPACE_HEIGHT);

    // Create square mesh and set up grid for rendering.
    this->CreateGrid();
}

void hw1::Editor::CreateGrid() {
    Mesh* squareMesh = hw1::CreateSquare("grid_square", BOTTOM_LEFT_CORNER,
                                         GRID_SQUARE_LENGTH, VEC3_GREEN, true);
    // When creating square we need to remember the center coordinate.
    for (int row = 0; row < GRID_ROW_NUMBER; row++) {
        for (int column = 0; column < GRID_COLUMN_NUMBER; column++) {
            // Compute bottom left corner.
            glm::vec3 bottomLeft = GRID_TOP_LEFT +
                                   (float)row * GRID_HORIZONTAL_OFFSET -
                                   (float)column * GRID_VERTICAL_OFFSET;

            // Compute center position.
            glm::vec3 center_position =
                bottomLeft + glm::vec3(GRID_SQUARE_LENGTH / 2.0f,
                                       GRID_SQUARE_LENGTH / 2.0f, 0.0f);

            this->grid.push_back(hw1::Square(squareMesh, center_position,
                                             VEC3_GREEN, GRID_SQUARE_LENGTH));
        }
    }
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

void hw1::Editor::DrawGrid() {
    glm::mat3 modelMatrix = glm::mat3(1);

    for (auto& square : this->grid) {
        modelMatrix =
            visMatrix * transform2D::Translate(square.GetPosition().x,
                                               square.GetPosition().y);

        RenderMesh2D(square.GetMesh(), shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawScene() { this->DrawGrid(); }
