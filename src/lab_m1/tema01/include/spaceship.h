/**
 * @file spaceship.h
 *
 * @brief Declares Spaceship class. Used to remember spaceship configuration,
 * AABB and components.
 *
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include <vector>

#include "lab_m1/tema01/include/object.h"

#define SPACESHIP_ROW_NUMBER 9
#define SPACESHIP_COLUMN_NUMBER 15
#define SPACESHIP_SQUARE_LENGTH 18

#define SPACESHIP_LEFT_MAX 1.0f
#define SPACESHIP_RIGHT_MAX 499.0f

namespace hw1 {

enum class DIRECTION { LEFT, RIGHT, UP, DOWN };

/**
 * @class SpaceShip
 *
 * @brief Contains all spaceship objects. Handles adding + removing objects,
 * conectivity checking + AABB collision model.
 */
class SpaceShip {
    friend class Editor;

   public:
    SpaceShip();

    ~SpaceShip();

    /**
     * @brief Adds spaceship component.
     *
     * @param object - Object to add.
     * @param matrixPosition - Position in matrix.
     */
    void AddObject(Object object, const glm::vec2& matrixPosition);

    /**
     * @brief Removes spaceship component.
     *
     * @param position - Position used to find object in vector.
     * @param matrixPosition - Position in matrix.
     */
    void RemoveObject(const glm::vec3& position,
                      const glm::vec2& matrixPosition);

    /**
     * @return Returns number of components.
     */
    int GetNumberOfComponents() const { return this->numberOfComponents; }

    glm::vec3 GetCenterPosition() const { return this->centerPosition; }

    /**
     * @brief Calculates center position of spaceship in order to start drawing
     * it in the lower middle part of the screen for the game.
     */
    void CalculateCenterPosition();

    /**
     * @brief Checks if spaceship can be moved.
     *
     * Calculates future position and checks if it is out of bounds.
     *
     * @return True if there is space left on the left / right, false otherwise.
     */
    bool CanMoveSpaceship(float moveOffset, DIRECTION direction);

    /**
     * @brief Move spaceship.
     *
     * First it checks if spaceship can be moved, then if true moves.
     *
     * @param moveOffset - Amount to move spaceship by.
     * @param direction - Direction to move spaceship to.
     */
    void MoveSpaceship(float moveOffset, DIRECTION direction);

    /**
     * @brief Checks if the grid square is used.
     *
     * @param position - Check if object has this position.
     *
     * @return True if inside, false otherwise.
     */
    bool InSpaceShip(const glm::vec3& position);

   private:
    /**
     * @brief Checks if adjacent node can be added to BFS search.
     *
     * Checks if row and column are inside matrix boundaries, if
     * visitedMatrix[row][column] is already visited and if
     * gridMatrix[row][column] contains a valid node.
     *
     * @param visitedMatrix - All visited nodes in BFS search.
     * @param row - Row that contains node to check.
     * @param column - Column that contains node to check.
     *
     * @return True if node can be added to BFS search, false otherwise.
     */
    bool IsBFSNodeValid(
        bool visitedMatrix[SPACESHIP_ROW_NUMBER][SPACESHIP_COLUMN_NUMBER],
        int row, int column);

    /**
     * @brief Checks is spaceship is connected.
     *
     * Performs a BFS on the grid matrix starting from the first non false
     * value found in grid matrix. If visited matrix is equal to the grid
     * matrix, then the spaceship is connected.
     *
     * @return True if spaceship is connected, false otherwise.
     */
    bool IsSpaceShipConnected();

   public:
    /**
     * @brief Returns true if spaceship configuration is valid. If valid,
     * game can start.
     *
     * @return Spaceship config corectness.
     */
    bool IsConfigCorrect();

   private:
    std::vector<Object> components;

    bool positionMatrix[SPACESHIP_ROW_NUMBER][SPACESHIP_COLUMN_NUMBER];

    glm::vec3 lowestPosition;
    glm::vec3 centerPosition;
    glm::vec3 highestPosition;

    int numberOfComponents;
};

}  // namespace hw1
