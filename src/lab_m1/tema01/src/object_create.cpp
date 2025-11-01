/**
 * @file object_create.cpp
 *
 * @brief Implements mesh and object creation methods of Editor class.
 *
 * @see Editor
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/editor.h"

void hw1::Editor::CreateEditorBorders() {
    Mesh* lineMesh = hw1::CreateLine("editor_line", VEC3_RED);

    AddMeshToList(lineMesh);
    // Create grid border lines.
    {
        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        BorderCorners gridBlocksBorder(
            "gridBlocks", glm::vec3(193, 25, 0), glm::vec3(193, 195, 0),
            glm::vec3(193 + 18 * (GRID_COLUMN_NUMBER + 1), 195, 0),
            glm::vec3(193 + 18 * (GRID_COLUMN_NUMBER + 1), 25, 0));

        // Add every delimiter created.
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

        // Add the border created.
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

        // Add every delimiter created.
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

        // Add the border created.
        this->borders.push_back(choosingBlocksBorder);
    }

    // Create border for every choose block.
    {
        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        BorderCorners choosingBlockSquare(
            "chooseSquare", glm::vec3(1, 25, 0), glm::vec3(1, 105, 0),
            glm::vec3(140, 105, 0), glm::vec3(140, 25, 0));

        BorderCorners choosingBlockBumper(
            "chooseBumper", glm::vec3(1, 105, 0), glm::vec3(1, 195, 0),
            glm::vec3(140, 195, 0), glm::vec3(140, 105, 0));

        this->delimiters.push_back(Line(lineMesh, VEC3_RED,
                                        choosingBlockSquare.topRight,
                                        choosingBlockSquare.topLeft));

        // Add the borders created.
        this->borders.push_back(choosingBlockSquare);
        this->borders.push_back(choosingBlockBumper);
    }

    // Create border for start button.
    {
        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        BorderCorners startButtonBorder(
            "startButton", glm::vec3(455, 235, 0), glm::vec3(455, 260, 0),
            glm::vec3(480, 260, 0), glm::vec3(480, 235, 0));

        // NOTE: Border will not be drawn. Useful to check if player clicked on
        // the start button.

        // Add the border created.
        this->borders.push_back(startButtonBorder);
    }
}

void hw1::Editor::CreateGrid() {
    // Create mesh for a grid square.
    Mesh* squareMesh =
        hw1::CreateSquare("grid_square", GRID_SQUARE_LENGTH, VEC3_BLUE, true);

    AddMeshToList(squareMesh);
    // When creating square we need to remember the center coordinate.
    for (int row = 0; row < GRID_ROW_NUMBER; row++) {
        for (int column = 0; column < GRID_COLUMN_NUMBER; column++) {
            // Compute bottom left corner.
            glm::vec3 bottomLeftSquare =
                GRID_TOP_LEFT + (float)column * GRID_HORIZONTAL_OFFSET -
                (float)row * GRID_VERTICAL_OFFSET;

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

void hw1::Editor::CreateComponentsCounter() {
    // Create mesh for counter section.
    Mesh* squareMesh = hw1::CreateSquare(
        "counter_square", COUNTER_SQUARE_LENGTH, VEC3_GREEN, true);

    AddMeshToList(squareMesh);

    for (int i = 0; i < COUNTER_NUMBER; i++) {
        glm::vec3 bottomLeftSquare =
            COUNTER_TOP_LEFT + (float)i * COUNTER_HORIZONTAL_OFFSET;

        // Compute center position.
        glm::vec3 center_position =
            bottomLeftSquare + glm::vec3(COUNTER_SQUARE_LENGTH / 2.0f,
                                         COUNTER_SQUARE_LENGTH / 2.0f, 0.0f);

        // Add newly created square to grid.
        this->componentsCounter.push_back(hw1::Square(
            squareMesh, center_position, VEC3_GREEN, GRID_SQUARE_LENGTH));
    }
}

void hw1::Editor::CreateChoosingBlocks() {
    {
        // Create new mesh for spaceship square.
        Mesh* squareMesh = hw1::CreateSquare(
            "spaceship_square", SPACESHIP_SQUARE_LENGTH, VEC3_LIGHT_GRAY, true);

        // Add mesh to mesh list.
        AddMeshToList(squareMesh);

        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        glm::vec3 bottomLeft = glm::vec3(60, 50, 0);

        // Compute center coordinate.
        glm::vec3 center_position =
            bottomLeft + glm::vec3(SPACESHIP_SQUARE_LENGTH / 2.0f,
                                   SPACESHIP_SQUARE_LENGTH / 2.0f, 0.0f);

        // Add newly created square.
        this->blocksToChoose.push_back(hw1::Square(squareMesh, center_position,
                                                   VEC3_LIGHT_GRAY,
                                                   SPACESHIP_SQUARE_LENGTH));
    }

    {
        // Create new mesh for a bumper.
        Mesh* bumperMesh =
            hw1::CreateBumper("spaceship_bumper", SPACESHIP_SQUARE_LENGTH,
                              VEC3_LIGHT_GRAY, VEC3_GREEN);

        // Add mesh to mesh list.
        AddMeshToList(bumperMesh);

        /**
         * WARN: DO NOT CHANGE CORNER VALUES!
         */
        glm::vec3 bottomLeft = glm::vec3(60, 120, 0);

        // Compute center position.
        glm::vec3 center_position =
            bottomLeft + glm::vec3(SPACESHIP_SQUARE_LENGTH / 2.0f,
                                   SPACESHIP_SQUARE_LENGTH / 2.0f, 0.0f);

        // Add newly created bumper.
        this->blocksToChoose.push_back(
            hw1::Bumper(bumperMesh, center_position, VEC3_LIGHT_GRAY));
    }
}

void hw1::Editor::CreateStartButton() {
    Mesh* startButtonMesh = hw1::CreateStartButton(
        "start_button", START_BUTTON_LENGTH, VEC3_GREEN, true);

    AddMeshToList(startButtonMesh);

    // Compute center coordinate.
    glm::vec3 center_position =
        START_BUTTON_POSITION +
        glm::vec3(START_BUTTON_LENGTH / 2.0f, START_BUTTON_LENGTH / 2.0f, 0.0f);

    this->startButton = new hw1::StartButton(startButtonMesh, center_position,
                                             VEC3_GREEN, START_BUTTON_LENGTH);
}
