/**
 * @file spaceship_logic.cpp
 *
 * @brief Implements spaceship methods and logic.
 *
 * Handles adding / removing objects to / from spaceship, spaceship
 * configuration validation and connectivity.
 *
 * @see Spaceship
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/spaceship.h"

#include <queue>
#include <utility>

hw1::SpaceShip::SpaceShip() {
    this->numberOfComponents = 0;

    for (int row = 0; row < SPACESHIP_ROW_NUMBER; row++)
        for (int column = 0; column < SPACESHIP_COLUMN_NUMBER; column++)
            this->positionMatrix[row][column] = false;

    this->lowestPosition = glm::vec3(600, 600, 0);
    this->centerPosition = glm::vec3(0, 0, 0);
    this->highestPosition = glm::vec3(0, 0, 0);
}

void hw1::SpaceShip::AddObject(Object* object,
                               const glm::vec2& matrixPosition) {
    this->components.push_back(object);

    this->positionMatrix[(int)matrixPosition.x][(int)matrixPosition.y] = true;

    this->numberOfComponents++;

    this->CalculateCenterPosition();
}

void hw1::SpaceShip::RemoveObject(const glm::vec3& position,
                                  const glm::vec2& matrixPosition) {
    // Go through every spaceship object and remove the first element that
    // matches the position coordinates given as a parameter.
    // NOTE: Due to checking, there will only be 1 match.

    if (position == glm::vec3(-1, -1, 0)) return;

    for (auto it = this->components.begin(); it != this->components.end();
         ++it) {
        if ((*it)->GetPosition() == position) {
            this->components.erase(it);
            this->positionMatrix[(int)matrixPosition.x][(int)matrixPosition.y] =
                false;
            this->numberOfComponents--;

            this->CalculateCenterPosition();

            return;
        }
    }
}

void hw1::SpaceShip::CalculateCenterPosition() {
    this->lowestPosition = glm::vec3(600, 600, 0);
    this->highestPosition = glm::vec3(0, 0, 0);

    // To calculate center position, calculate the average between highest and
    // lowest position.
    for (const auto& object : components) {
        this->lowestPosition =
            glm::min(this->lowestPosition, object->GetPosition());
        this->highestPosition =
            glm::max(this->highestPosition, object->GetPosition());
    }

    this->centerPosition =
        (this->lowestPosition + this->highestPosition) / 2.0f;
}

bool hw1::SpaceShip::CanMoveSpaceship(float moveOffset, DIRECTION direction) {
    this->CalculateCenterPosition();

    // Calculate future position.
    // NOTE: Tried with actual position (if actual position out of bounds then
    // add / subtract from position so that it stopped being out of bounds), but
    // didn't work.
    float futureLowestX = this->lowestPosition.x;
    float futureHighestX = this->highestPosition.x;

    if (direction == DIRECTION::LEFT)
        futureLowestX -= moveOffset;
    else if (direction == DIRECTION::RIGHT)
        futureHighestX += moveOffset;

    if (futureLowestX - (float)SPACESHIP_SQUARE_LENGTH / 2 < SPACESHIP_LEFT_MAX)
        return false;
    if (futureHighestX + (float)SPACESHIP_SQUARE_LENGTH / 2 >
        SPACESHIP_RIGHT_MAX)
        return false;

    return true;
}

void hw1::SpaceShip::MoveSpaceship(float moveOffset, DIRECTION direction) {
    if (!this->CanMoveSpaceship(moveOffset, direction)) return;

    glm::vec3 offset = (direction == DIRECTION::LEFT)
                           ? glm::vec3(-moveOffset, 0, 0)
                           : glm::vec3(moveOffset, 0, 0);

    // Move every spaceship object to current position + offset and recalculate
    // spaceship center position.
    for (auto& object : this->components)
        object->SetPosition(object->GetPosition() + offset);

    this->CalculateCenterPosition();
}

bool hw1::SpaceShip::InSpaceShip(const glm::vec3& position) {
    // Checks if there is an object at given position.
    for (auto& object : this->components)
        if (object->GetPosition() == position) return true;

    return false;
}

bool hw1::SpaceShip::IsBFSNodeValid(
    bool visitedMatrix[SPACESHIP_ROW_NUMBER][SPACESHIP_COLUMN_NUMBER], int row,
    int column) {
    // Check grid bounds
    if (row < 0 || column < 0 || row >= SPACESHIP_ROW_NUMBER ||
        column >= SPACESHIP_COLUMN_NUMBER)
        return false;

    // Must be filled and not visited.
    if (!this->positionMatrix[row][column] || visitedMatrix[row][column])
        return false;

    return true;
}

bool hw1::SpaceShip::IsSpaceShipConnected() {
    // Get first non false element in matrix.
    int row = -1;
    int column = -1;
    bool found = false;

    bool visitedMatrix[SPACESHIP_ROW_NUMBER][SPACESHIP_COLUMN_NUMBER];

    for (int i = 0; i < SPACESHIP_ROW_NUMBER; i++) {
        for (int j = 0; j < SPACESHIP_COLUMN_NUMBER; j++) {
            visitedMatrix[i][j] = false;

            if (!found && this->positionMatrix[i][j] == true) {
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

    // Check if the visited matrix is identical to the positional matrix.
    for (int i = 0; i < SPACESHIP_ROW_NUMBER; i++)
        for (int j = 0; j < SPACESHIP_COLUMN_NUMBER; j++)
            if (visitedMatrix[i][j] != this->positionMatrix[i][j]) return false;

    return true;
}

bool hw1::SpaceShip::IsConfigCorrect() {
    // If the spaceship is connected and has at least one component,
    // configuration is correct.
    if (!this->numberOfComponents) return false;
    if (!this->IsSpaceShipConnected()) return false;

    return true;
}
