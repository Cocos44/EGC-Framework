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

    this->isLeftButtonHold = false;
    this->buttonHoldObject = nullptr;

    // Create square mesh and set up grid for rendering.
    this->CreateEditorBorders();
    this->CreateGrid();
    this->CreateChoosingBlocks();
}

void hw1::Editor::CreateEditorBorders() {
    Mesh* lineMesh = hw1::CreateLine("editor_line", VEC3_RED);

    // Create grid border lines.
    {
        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        BorderCorners gridBlocksBorder(
            "gridBlocks", glm::vec3(193, 25, 0), glm::vec3(193, 195, 0),
            glm::vec3(193 + 18 * (GRID_ROW_NUMBER + 1), 195, 0),
            glm::vec3(193 + 18 * (GRID_ROW_NUMBER + 1), 25, 0));

        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        gridBlocksBorder.bottomLeft,
                                        gridBlocksBorder.topLeft));
        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        gridBlocksBorder.topLeft,
                                        gridBlocksBorder.topRight));
        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        gridBlocksBorder.topRight,
                                        gridBlocksBorder.bottomRight));
        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        gridBlocksBorder.bottomRight,
                                        gridBlocksBorder.bottomLeft));

        this->borders.push_back(gridBlocksBorder);
    }

    // Create choosing blocks border lines.
    {
        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        BorderCorners choosingBlocksBorder(
            "choosingBlocks", glm::vec3(140, 25, 0), glm::vec3(140, 195, 0),
            glm::vec3(1, 195, 0), glm::vec3(1, 25, 0));

        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        choosingBlocksBorder.bottomLeft,
                                        choosingBlocksBorder.topLeft));
        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        choosingBlocksBorder.topLeft,
                                        choosingBlocksBorder.topRight));
        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        choosingBlocksBorder.topRight,
                                        choosingBlocksBorder.bottomRight));
        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        choosingBlocksBorder.bottomRight,
                                        choosingBlocksBorder.bottomLeft));

        this->borders.push_back(choosingBlocksBorder);
    }

    // Create border for every choose block.
    {
        BorderCorners choosingBlockSquare(
            "chooseSquare", glm::vec3(1, 25, 0), glm::vec3(1, 105, 0),
            glm::vec3(140, 105, 0), glm::vec3(140, 25, 0));

        BorderCorners choosingBlockBumper(
            "chooseBumper", glm::vec3(1, 105, 0), glm::vec3(1, 195, 0),
            glm::vec3(140, 195, 0), glm::vec3(140, 105, 0));

        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        choosingBlockSquare.topRight,
                                        choosingBlockSquare.topLeft));

        this->borders.push_back(choosingBlockSquare);
        this->borders.push_back(choosingBlockBumper);
    }
}

void hw1::Editor::CreateGrid() {
    Mesh* squareMesh =
        hw1::CreateSquare("grid_square", GRID_SQUARE_LENGTH, VEC3_BLUE, true);
    // When creating square we need to remember the center coordinate.
    for (int row = 0; row < GRID_ROW_NUMBER; row++) {
        for (int column = 0; column < GRID_COLUMN_NUMBER; column++) {
            // Compute bottom left corner.
            glm::vec3 bottomLeftSquare = GRID_TOP_LEFT +
                                         (float)row * GRID_HORIZONTAL_OFFSET -
                                         (float)column * GRID_VERTICAL_OFFSET;

            // Compute center position.
            glm::vec3 center_position =
                bottomLeftSquare + glm::vec3(GRID_SQUARE_LENGTH / 2.0f,
                                             GRID_SQUARE_LENGTH / 2.0f, 0.0f);

            // Add newly created square to grid.
            this->grid.push_back(hw1::Square(squareMesh, center_position,
                                             VEC3_GREEN, GRID_SQUARE_LENGTH));
        }
    }
}

void hw1::Editor::CreateChoosingBlocks() {
    {
        // Create new mesh for spaceship square.
        Mesh* squareMesh = hw1::CreateSquare(
            "spaceship_square", SPACESHIP_SQUARE_LENGTH, VEC3_LIGHT_GRAY, true);
        AddMeshToList(squareMesh);
        glm::vec3 bottomLeft = glm::vec3(60, 50, 0);
        glm::vec3 center_position =
            bottomLeft + glm::vec3(SPACESHIP_SQUARE_LENGTH / 2.0f,
                                   SPACESHIP_SQUARE_LENGTH / 2.0f, 0.0f);

        this->blocksToChoose.push_back(hw1::Square(squareMesh, center_position,
                                                   VEC3_LIGHT_GRAY,
                                                   SPACESHIP_SQUARE_LENGTH));
    }

    {
        Mesh* bumperMesh =
            hw1::CreateBumper("spaceship_bumper", SPACESHIP_SQUARE_LENGTH,
                              VEC3_LIGHT_GRAY, VEC3_GREEN);
        AddMeshToList(bumperMesh);

        glm::vec3 bottomLeft = glm::vec3(60, 120, 0);
        glm::vec3 center_position =
            bottomLeft + glm::vec3(SPACESHIP_SQUARE_LENGTH / 2.0f,
                                   SPACESHIP_SQUARE_LENGTH / 2.0f, 0.0f);

        this->blocksToChoose.push_back(
            hw1::Bumper(bumperMesh, center_position, VEC3_LIGHT_GRAY));
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

void hw1::Editor::OnMouseBtnPress(int mouseX, int mouseY, int button,
                                  int mods) {
    // Get the mouse coordinates for our logic space.
    glm::vec3 mousePositionLogicSpace =
        this->ConvertScreenToLogicSpace(mouseX, mouseY);

    // Left mouse button was clicked.
    if (button == GLFW_MOUSE_BUTTON_2) {
        // Go through every border.
        for (auto& border : this->borders) {
            // Find in which border the mouse button was presesd.
            if (this->IsInsideBorder(mousePositionLogicSpace, border)) {
                if (border.name == "chooseSquare") {
                    this->buttonHoldObject = new Square(
                        meshes["spaceship_square"], mousePositionLogicSpace,
                        VEC3_LIGHT_GRAY, SPACESHIP_SQUARE_LENGTH);
                }

                else if (border.name == "chooseBumper") {
                    this->buttonHoldObject =
                        new Bumper(meshes["spaceship_bumper"],
                                   mousePositionLogicSpace, VEC3_LIGHT_GRAY);
                }
                if (border.name == "chooseSquare") {
                    this->isLeftButtonHold = true;

                    this->buttonHoldObject = new Square(
                        meshes["spaceship_square"], mousePositionLogicSpace,
                        VEC3_LIGHT_GRAY, SPACESHIP_SQUARE_LENGTH);
                } else if (border.name == "chooseBumper") {
                    this->isLeftButtonHold = true;

                    this->buttonHoldObject = new Bumper(
                        meshes["spaceship_bumper"], mousePositionLogicSpace,
                        VEC3_LIGHT_GRAY, true);
                }
            } else {
                continue;
            }
        }
    } else if (button == GLFW_MOUSE_BUTTON_3) {
        this->RemoveFromSpaceship(
            this->GetSquareFromGrid(mousePositionLogicSpace));
    }
}

void hw1::Editor::OnMouseBtnRelease(int mouseX, int mouseY, int button,
                                    int mods) {
    if (button == GLFW_MOUSE_BUTTON_2) {
        if (this->buttonHoldObject != nullptr) {
            // Convert mouse position to logic space
            glm::vec3 mousePositionLogicSpace =
                this->ConvertScreenToLogicSpace(mouseX, mouseY);

            this->PlaceObjectInGrid(mousePositionLogicSpace);

            // Clean up dragged object
            delete this->buttonHoldObject;
            this->buttonHoldObject = nullptr;
            this->isLeftButtonHold = false;
        }
    }
}

void hw1::Editor::PlaceObjectInGrid(const glm::vec3& mousePositionLogicSpace) {
    for (const auto& border : this->borders) {
        if (border.name == "gridBlocks" &&
            this->IsInsideBorder(mousePositionLogicSpace, border)) {
            glm::vec3 resultCoordinate =
                this->GetSquareFromGrid(mousePositionLogicSpace);

            // If the mouse is over a valid grid square and object is not
            // already there.
            if (resultCoordinate.x >= 0 && resultCoordinate.y >= 0 &&
                !this->InSpaceShip(resultCoordinate)) {
                std::string meshID =
                    this->buttonHoldObject->GetMesh()->GetMeshID();

                if (meshID == "spaceship_square") {
                    // Add new square to spaceship vector.
                    this->spaceship.push_back(
                        Square(meshes["spaceship_square"], resultCoordinate,
                               VEC3_LIGHT_GRAY, SPACESHIP_SQUARE_LENGTH));
                } else if (meshID == "spaceship_bumper") {
                    // Add new bumper to spaceship vector.
                    this->spaceship.push_back(Bumper(meshes["spaceship_bumper"],
                                                     resultCoordinate,
                                                     VEC3_LIGHT_GRAY));
                }
            }

            // No need to check further objects.
            break;
        }
    }
}

void hw1::Editor::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (this->isLeftButtonHold && this->buttonHoldObject != nullptr) {
        // Convert mouse coordinates to logic space
        glm::vec3 mousePositionLogicSpace =
            this->ConvertScreenToLogicSpace(mouseX, mouseY);

        // Update the object position
        this->buttonHoldObject->SetPosition(mousePositionLogicSpace);

        // Build the model matrix using the updated position
    }
}

void hw1::Editor::FrameEnd() {}

void hw1::Editor::DrawBorders() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every line to render it.
    for (const auto& line : this->delimiters) {
        // First move line to it's starting position, then rotate it based on
        // the arctangent calculated in constructor, then scale to the size
        // wanted.
        modelMatrix =
            visMatrix *
            transform2D::Translate(line.GetPosition().x, line.GetPosition().y) *
            transform2D::Rotate(line.GetAngle()) *
            transform2D::Scale(line.GetLength(), 1);

        // Make line a bit thicker.
        glLineWidth(2.0f);

        // Render mesh.
        RenderMesh2D(line.GetMesh(), shaders["VertexColor"], modelMatrix);

        // Reset line thickness.
        glLineWidth(1.0f);

        // Reset modelMatrix.
        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawGrid() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every grid square to render it.
    for (auto& square : this->grid) {
        // Move the square to wanted position.
        modelMatrix =
            visMatrix * transform2D::Translate(square.GetPosition().x,
                                               square.GetPosition().y);

        // Render mesh.
        RenderMesh2D(square.GetMesh(), shaders["VertexColor"], modelMatrix);

        // Reset modelMatrix.
        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawChoosingBlocks() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every choosing object to render it.
    for (auto& object : this->blocksToChoose) {
        std::string meshID = object.GetMesh()->GetMeshID();
        if (meshID == "spaceship_square") {
            // Move the square to wanted position.
            modelMatrix =
                visMatrix * transform2D::Translate(object.GetPosition().x,
                                                   object.GetPosition().y);

            // Render mesh.
            RenderMesh2D(object.GetMesh(), shaders["VertexColor"], modelMatrix);

            // Reset modelMatrix.
            modelMatrix = glm::mat3(1);
        }

        else if (meshID == "spaceship_bumper") {
            // Move the bumper to wanted position.
            modelMatrix =
                visMatrix * transform2D::Translate(object.GetPosition().x,
                                                   object.GetPosition().y);
            // Render mesh.
            RenderMesh2D(object.GetMesh(), shaders["VertexColor"], modelMatrix);

            // Reset modelMatrix.
            modelMatrix = glm::mat3(1);
        }
    }
}

void hw1::Editor::DrawHoldObject() {
    // Render the mesh
    if (this->isLeftButtonHold && (this->buttonHoldObject != nullptr)) {
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix = visMatrix * transform2D::Translate(
                                      this->buttonHoldObject->GetPosition().x,
                                      this->buttonHoldObject->GetPosition().y);
        RenderMesh2D(this->buttonHoldObject->GetMesh(), shaders["VertexColor"],
                     modelMatrix);
    }
}

void hw1::Editor::DrawSpaceShip() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every spaceship object to render it.
    for (auto& object : this->spaceship) {
        // Move the square to wanted position.
        modelMatrix =
            visMatrix * transform2D::Translate(object.GetPosition().x,
                                               object.GetPosition().y);

        // Render mesh.
        RenderMesh2D(object.GetMesh(), shaders["VertexColor"], modelMatrix);

        // Reset modelMatrix.
        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawScene() {
    this->DrawChoosingBlocks();
    this->DrawSpaceShip();
    this->DrawHoldObject();
    this->DrawBorders();
    this->DrawGrid();
}

void hw1::Editor::RemoveFromSpaceship(const glm::vec3& position) {
    for (auto it = spaceship.begin(); it != spaceship.end(); ++it) {
        if (it->GetPosition() == position) {
            spaceship.erase(it);
            return;
        }
    }
}

glm::vec3 hw1::Editor::GetSquareFromGrid(
    const glm::vec3& mousePositionLogicSpace) {
    for (const auto& square : this->grid) {
        glm::vec3 bottomLeft =
            square.GetPosition() -
            glm::vec3(GRID_SQUARE_LENGTH / 2.0f, GRID_SQUARE_LENGTH / 2.0f, 0);
        glm::vec3 topRight =
            square.GetPosition() +
            glm::vec3(GRID_SQUARE_LENGTH / 2.0f, GRID_SQUARE_LENGTH / 2.0f, 0);

        if (mousePositionLogicSpace.x >= bottomLeft.x &&
            mousePositionLogicSpace.x <= topRight.x &&
            mousePositionLogicSpace.y >= bottomLeft.y &&
            mousePositionLogicSpace.y <= topRight.y) {
            return square.GetPosition();
        }
    }

    return glm::vec3(-1, -1, 0);
}

glm::vec3 hw1::Editor::ConvertScreenToLogicSpace(int mouseX, int mouseY) {
    int windowHeight = window->GetResolution().y;
    float flippedY = windowHeight - mouseY;

    float normalX = float(mouseX - viewSpace.x) / viewSpace.width;
    float normalY = float(flippedY - viewSpace.y) / viewSpace.height;

    float logicX = logicSpace.x + normalX * logicSpace.width;
    float logicY = logicSpace.y + normalY * logicSpace.height;

    return glm::vec3(logicX, logicY, 0);
}

bool hw1::Editor::IsInsideBorder(const glm::vec3& mousePositionLogicSpace,
                                 const BorderCorners& border) const {
    return (mousePositionLogicSpace.x >= border.bottomLeft.x) &&
           (mousePositionLogicSpace.y >= border.bottomLeft.y) &&
           (mousePositionLogicSpace.x <= border.topRight.x) &&
           (mousePositionLogicSpace.y <= border.topRight.y);
}

bool hw1::Editor::InSpaceShip(const glm::vec3& position) {
    for (auto& object : this->spaceship)
        if (object.GetPosition() == position) return true;

    return false;
}
