/**
 * @file editor.cpp
 *
 * @brief Implements higher lever logic of Editor class.
 *
 * Handles scene rendering, transitioning from logic space to view space and
 * correct field initialization.
 *
 * @see Editor.h
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/editor.h"

#include "lab_m1/tema01/include/spaceship.h"

hw1::Editor::Editor()
    : textRenderer("/home/vlad/Dev/EGC/EGC-Framework",
                   window->GetResolution().x, window->GetResolution().y) {}

hw1::Editor::~Editor() {}

void hw1::Editor::Init() {
    // Init camera options.
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
    this->startButton = nullptr;
    this->isGameRunning = false;

    // Load text font.
    this->textRenderer.Load(
        "/home/vlad/Dev/EGC/EGC-Framework/assets/fonts/Hack-Bold.ttf", 40);

    this->spaceship = new SpaceShip();

    this->CreateEditorBorders();
    this->CreateGrid();
    this->CreateChoosingBlocks();
    this->CreateComponentsCounter();
    this->CreateStartButton();
}

glm::mat3 hw1::Editor::GetSpaceConversionMatrix() {
    // Calculate logic space conversion matrix.
    // NOTE: Formula taken from second lab.
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

    // Draw current scene.
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
                if (this->spaceship->numberOfComponents ==
                    SPACESHIP_MAX_COMPONENTS)
                    return;
                // Player chose square object.
                if (border.name == "chooseSquare") {
                    this->isLeftButtonHold = true;

                    this->buttonHoldObject = new Square(
                        meshes["spaceship_square"], mousePositionLogicSpace,
                        VEC3_LIGHT_GRAY, SPACESHIP_SQUARE_LENGTH);
                }
                // Player chose bumper object.
                else if (border.name == "chooseBumper") {
                    this->isLeftButtonHold = true;

                    this->buttonHoldObject = new Bumper(
                        meshes["spaceship_bumper"], mousePositionLogicSpace,
                        VEC3_LIGHT_GRAY, true);
                } else if (border.name == "startButton" &&
                           this->spaceship->IsConfigCorrect()) {
                    this->InitGame();
                }
            } else {
                continue;
            }
        }
    }
    // Right mouse button was clicked.
    else if (button == GLFW_MOUSE_BUTTON_3) {
        /**
         * Delete spaceship part in that square grid.
         * NOTE: GetSquareFromGrid returns glm::vec3(-1, -1, 0) if not found.
         * GetPositionFromGrid checks value of squarePosition before operating.
         */
        glm::vec3 squarePosition =
            this->GetSquareFromGrid(mousePositionLogicSpace);

        this->spaceship->RemoveObject(
            squarePosition, this->GetPositionFromGrid(squarePosition));
    }
}

void hw1::Editor::OnMouseBtnRelease(int mouseX, int mouseY, int button,
                                    int mods) {
    // Left mouse button was clicked.
    if (button == GLFW_MOUSE_BUTTON_2) {
        // If player selected an object to be placed and matches it
        // with a free square grid, add it to the spaceship.
        if (this->buttonHoldObject != nullptr) {
            // Convert mouse position to logic space
            glm::vec3 mousePositionLogicSpace =
                this->ConvertScreenToLogicSpace(mouseX, mouseY);

            this->PlaceObjectInSpaceShip(mousePositionLogicSpace);

            // Clean up dragged object
            delete this->buttonHoldObject;
            this->buttonHoldObject = nullptr;
            this->isLeftButtonHold = false;
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
    }
}

void hw1::Editor::FrameEnd() {}

void hw1::Editor::PlaceObjectInSpaceShip(
    const glm::vec3& mousePositionLogicSpace) {
    for (const auto& border : this->borders) {
        if (border.name == "gridBlocks" &&
            this->IsInsideBorder(mousePositionLogicSpace, border)) {
            glm::vec3 resultCoordinate =
                this->GetSquareFromGrid(mousePositionLogicSpace);

            // If the mouse is over a valid grid square and object is not
            // already there.
            if (resultCoordinate.x >= 0 && resultCoordinate.y >= 0 &&
                !this->spaceship->InSpaceShip(resultCoordinate)) {
                std::string meshID =
                    this->buttonHoldObject->GetMesh()->GetMeshID();

                if (meshID == "spaceship_square") {
                    // Add new square to spaceship vector.
                    this->spaceship->AddObject(
                        Square(meshes["spaceship_square"], resultCoordinate,
                               VEC3_LIGHT_GRAY, SPACESHIP_SQUARE_LENGTH),
                        this->GetPositionFromGrid(resultCoordinate));
                } else if (meshID == "spaceship_bumper") {
                    // Add new bumper to spaceship vector.
                    this->spaceship->AddObject(
                        Bumper(meshes["spaceship_bumper"], resultCoordinate,
                               VEC3_LIGHT_GRAY),
                        this->GetPositionFromGrid(resultCoordinate));
                }
            }

            // No need to check further objects.
            break;
        }
    }
}

void hw1::Editor::DrawScene() {
    if (!this->isGameRunning) {
        this->DrawChoosingBlocks();
        this->DrawSpaceShip();
        this->DrawHoldObject();
        this->DrawBorders();
        this->DrawGrid();
        this->DrawCounterSection();
        this->DrawText();
        this->DrawStartButton();
    } else {
        this->DrawSpaceShip();
    }
}

glm::vec3 hw1::Editor::GetSquareFromGrid(
    const glm::vec3& mousePositionLogicSpace) {
    // Go through every grid square.
    for (const auto& square : this->grid) {
        // Compute area of grid square.
        glm::vec3 bottomLeft =
            square.GetPosition() -
            glm::vec3(GRID_SQUARE_LENGTH / 2.0f, GRID_SQUARE_LENGTH / 2.0f, 0);
        glm::vec3 topRight =
            square.GetPosition() +
            glm::vec3(GRID_SQUARE_LENGTH / 2.0f, GRID_SQUARE_LENGTH / 2.0f, 0);

        // Check if player released mouse button on this grid square.
        if (mousePositionLogicSpace.x >= bottomLeft.x &&
            mousePositionLogicSpace.x <= topRight.x &&
            mousePositionLogicSpace.y >= bottomLeft.y &&
            mousePositionLogicSpace.y <= topRight.y) {
            return square.GetPosition();
        }
    }

    // If no match, return an invalid square position that will be checked.
    return glm::vec3(-1, -1, 0);
}

glm::vec2 hw1::Editor::GetPositionFromGrid(const glm::vec3& squarePosition) {
    // Go through every grid square.
    for (int i = 0; i < this->grid.size(); i++) {
        if (this->grid[i].GetPosition() == squarePosition) {
            int row = i / GRID_COLUMN_NUMBER;
            int column = i % GRID_COLUMN_NUMBER;

            return glm::vec2(row, column);
        }
    }

    return glm::vec2(-1, -1);
}

glm::vec3 hw1::Editor::ConvertScreenToLogicSpace(int mouseX, int mouseY) {
    // Converts view space coordinates to logic space coordinates.
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
    // Check if mouse position is inside a border.
    return (mousePositionLogicSpace.x >= border.bottomLeft.x) &&
           (mousePositionLogicSpace.y >= border.bottomLeft.y) &&
           (mousePositionLogicSpace.x <= border.topRight.x) &&
           (mousePositionLogicSpace.y <= border.topRight.y);
}
