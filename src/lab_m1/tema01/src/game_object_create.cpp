/**
 * @file object_create.cpp
 *
 * @brief Implements mesh and object creation methods of Editor class.
 *
 * File contains methods only related to editor objects.
 *
 * @see Editor
 *
 * @author Grigoras Vlad Andrei
 */

#include <iostream>

#include "lab_m1/tema01/include/editor.h"

void hw1::Editor::CreateGameBricks() {
    // When creating rectangle we need to remember the center coordinate.
    for (int row = 0; row < GAME_ROW_NUMBER; row++) {
        std::string meshName = "game_brick" + std::to_string(row);
        glm::vec3 meshColor = this->colors[rand() % this->colors.size()];

        Mesh* rectangleMesh = hw1::CreateRectangle(
            meshName, GAME_BRICK_LENGTH, GAME_BRICK_WIDTH, meshColor, true);

        AddMeshToList(rectangleMesh);

        for (int column = 0; column < GAME_COLUMN_NUMBER; column++) {
            // Compute top left corner.
            glm::vec3 topLeftRectangle =
                GAME_BRICKS_STARTING_POSITION +
                (float)column * GAME_HORIZONTAL_OFFSET -
                (float)row * GAME_VERTICAL_OFFSET;

            // Compute center position.
            glm::vec3 center_position =
                topLeftRectangle + glm::vec3(GAME_BRICK_LENGTH / 2.0f,
                                             GAME_BRICK_WIDTH / 2.0f, 0.0f);

            // Add newly created rectangle to grid.
            this->bricks.push_back(Rectangle(rectangleMesh, center_position,
                                             meshColor, GAME_BRICK_LENGTH,
                                             GAME_BRICK_WIDTH, true));
        }
    }
}
