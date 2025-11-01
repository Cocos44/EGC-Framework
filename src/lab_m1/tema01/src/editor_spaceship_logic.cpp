/**
 * @file spaceship_logic.cpp
 *
 * @brief Implements spaceship logic.
 *
 * Handles adding / removing objects to / from spaceship, spaceship
 * configuration validation and connectivity.
 *
 * @author Grigoras Vlad Andrei.
 */

#include "lab_m1/tema01/include/editor.h"

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

                this->ChangeGridMatrixPositionValue(resultCoordinate, true);

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

bool hw1::Editor::InSpaceShip(const glm::vec3& position) {
    // Go through every grid square.
    for (int i = 0; i < this->grid.size(); i++) {
        if (this->grid[i].GetPosition() == position) {
            int row = i / GRID_COLUMN_NUMBER;
            int column = i % GRID_COLUMN_NUMBER;

            // Find grid square and check if there is a spaceship object there.
            if ((this->grid[i].GetPosition() == position) &&
                (this->gridMatrix[row][column]))
                return true;
        }
    }

    return false;
}

void hw1::Editor::RemoveFromSpaceship(const glm::vec3& position) {
    // Go through every spaceship object and remove the first element that
    // matches the position coordinates given as a parameter.
    // NOTE: Due to checking, there will only be 1 match.
    for (auto it = spaceship.begin(); it != spaceship.end(); ++it) {
        if (it->GetPosition() == position) {
            spaceship.erase(it);
            return;
        }
    }
}

void hw1::Editor::ChangeGridMatrixPositionValue(const glm::vec3& position,
                                                const bool& value) {
    if (position == glm::vec3(-1, -1, 0)) return;

    // Find position in matrix and either add or delete a value.
    // NOTE: this->gridMatrix[i][j] == false -> No object placed there.
    // NOTE: this->gridMatrix[i][j] == true -> An object placed there.
    for (int i = 0; i < this->grid.size(); i++) {
        if (this->grid[i].GetPosition() == position) {
            int row = i / GRID_COLUMN_NUMBER;
            int column = i % GRID_COLUMN_NUMBER;

            this->gridMatrix[row][column] = value;

            return;
        }
    }
}

bool hw1::Editor::IsBFSNodeValid(
    bool visitedMatrix[GRID_ROW_NUMBER][GRID_COLUMN_NUMBER], int row,
    int column) {
    // Check grid bounds
    if (row < 0 || column < 0 || row >= GRID_ROW_NUMBER ||
        column >= GRID_COLUMN_NUMBER)
        return false;

    // Must be filled and not already visited
    if (!this->gridMatrix[row][column] || visitedMatrix[row][column])
        return false;

    return true;
}

bool hw1::Editor::IsSpaceShipConnected() {
    // Get first non false element in matrix.
    int row = -1;
    int column = -1;
    bool found = false;

    bool visitedMatrix[GRID_ROW_NUMBER][GRID_COLUMN_NUMBER];

    for (int i = 0; i < GRID_ROW_NUMBER; i++) {
        for (int j = 0; j < GRID_COLUMN_NUMBER; j++) {
            visitedMatrix[i][j] = false;

            if (!found && this->gridMatrix[i][j] == true) {
                row = i;
                column = j;

                found = true;
            }
        }
    }

    /**
     *No elements found, so spaceship is connected.

     * NOTE: Returns true but config not correct (spaceship does not have a
     * component). IsSpaceShipConfig picks it up.
     *
     * @see IsSpaceShipConfig.
     */
    if (row == -1 || column == -1) return true;

    // Used to check every adjacent node in matrix.
    int directionRow[] = {-1, 0, 1, 0};
    int directionColumn[] = {0, 1, 0, -1};

    std::queue<std::pair<int, int>> q;

    q.push({row, column});
    visitedMatrix[row][column] = true;

    // Perform BFS search from found node.
    while (!q.empty()) {
        std::pair<int, int> node = q.front();

        int nodeX = node.first;
        int nodeY = node.second;

        q.pop();

        for (int i = 0; i < 4; i++) {
            int adjacentX = nodeX + directionRow[i];
            int adjacentY = nodeY + directionColumn[i];

            // If node inside matrix && not visited => add to queue.
            if (this->IsBFSNodeValid(visitedMatrix, adjacentX, adjacentY)) {
                q.push({adjacentX, adjacentY});
                visitedMatrix[adjacentX][adjacentY] = true;
            }
        }
    }

    for (int i = 0; i < GRID_ROW_NUMBER; i++)
        for (int j = 0; j < GRID_COLUMN_NUMBER; j++)
            if (visitedMatrix[i][j] != this->gridMatrix[i][j]) return false;

    return true;
}

bool hw1::Editor::IsSpaceShipConfig() {
    if (!this->spaceship.size()) return false;
    if (!this->IsSpaceShipConnected()) return false;

    return true;
}
