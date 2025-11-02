/**
 * @file game.cpp
 *
 * @brief Implements game logic and every method related to the game
 * functionality of Editor class.
 *
 * @see Editor
 *
 * @author Grigoras Vlad Andrei
 */

#include <cmath>

#include "lab_m1/tema01/include/editor.h"

void hw1::Editor::InitGame() {
    this->CleanupEditorResources();

    this->isGameRunning = true;
    this->PlaceSpaceShipStartPosition();
}

void hw1::Editor::PlaceSpaceShipStartPosition() {
    // Compute current center based on components
    this->spaceship->CalculateCenterPosition();

    glm::vec3 offsetToStart =
        spaceship->GetCenterPosition() - GAME_STARTING_POSITION;

    // Move every spaceship object to starting position - offset.
    for (auto& object : this->spaceship->components)
        object->SetPosition(object->GetPosition() - offsetToStart);

    //  Recalculate center position.
    this->spaceship->CalculateCenterPosition();
}

void hw1::Editor::PlaceBallStartingPosition() {
    this->gameBall->SetPosition(GAME_BALL_STARTING_POSITION);

    // Start ball movement with random X coordinate direction.
    int randomDirection = rand() % 2;

    this->gameBall->velocity.x =
        randomDirection % 2 == 0 ? GAME_BALL_SPEED : -GAME_BALL_SPEED;
    this->gameBall->velocity.y = GAME_BALL_SPEED;
}

void hw1::Editor::UpdateBallPosition(float deltaTimeSeconds) {
    // Calculate offset to future position.
    glm::vec3 nextPositionOffset =
        glm::vec3(this->gameBall->velocity.x, this->gameBall->velocity.y, 0) *
        deltaTimeSeconds;

    // Get new position and update ball center.
    glm::vec3 updatedPosition =
        this->gameBall->GetPosition() + nextPositionOffset;

    this->gameBall->SetPosition(updatedPosition);

    // Recalculate hitbox for new position.
    this->gameBall->collisionBox.min =
        updatedPosition -
        glm::vec3(this->gameBall->GetRadius(), this->gameBall->GetRadius(), 0);

    this->gameBall->collisionBox.max =
        updatedPosition +
        glm::vec3(this->gameBall->GetRadius(), this->gameBall->GetRadius(), 0);
}

void hw1::Editor::CheckCollisionSpaceShip() {
    glm::vec3 ballPosition = this->gameBall->GetPosition();
    float ballRadius = this->gameBall->GetRadius();

    // For every object, detect if there is a collision with ball.
    for (auto& object : spaceship->components) {
        // Detected collision.
        if (object->collisionBox.IsCollision(gameBall->collisionBox)) {
            /**
             * NOTE: Need to check which type of collision has occured.
             * If there is a vertical collision, we just invert the X speed.
             * If there is a horizontal collision, we calculate new speed based
             * on: - Angle of ball and center of the spaceship.
             *     - Original ball speed.
             */
            float objectLeftBottomEdge = object->collisionBox.min.x;
            float objectRighBottomtEdge = object->collisionBox.max.x;
            float objectLeftTopEdge = object->collisionBox.min.y;
            float objectRightTopEdge = object->collisionBox.max.y;

            float ballBottomLeftEdge = this->gameBall->collisionBox.min.x;
            float ballTopRightEdge = this->gameBall->collisionBox.max.x;
            float ballTopLeftEdge = this->gameBall->collisionBox.min.y;

            // Calculates overlap on the X axis.
            // Ball can either come from the left or the right side.
            float overlapX =
                std::min(ballTopRightEdge - objectLeftBottomEdge,
                         objectRighBottomtEdge - ballBottomLeftEdge);

            // Calculates overlap on the Y axis.
            // Ball can only come down on the spaceship.
            float overlapY = objectRightTopEdge - ballTopLeftEdge;

            float newBallXSpeed = 0;
            float newBallYSpeed = 0;

            // Check which overlap is smaller.
            if (overlapX < overlapY) {
                newBallXSpeed = -gameBall->GetXSpeed();
                newBallYSpeed = gameBall->GetYSpeed();
            } else {
                float xSpeed = gameBall->GetXSpeed();
                float ySpeed = gameBall->GetYSpeed();

                // Calculate current ball speed.
                float ballSpeed = sqrt(xSpeed * xSpeed + ySpeed * ySpeed);

                // Calculate angle at which ball collides with spaceship. Angle
                // is calculated based on spaceship center and ball center.
                float angle = (this->gameBall->position.x -
                               spaceship->GetCenterPosition().x) /
                              ((float)(this->spaceship->highestPosition.x -
                                       this->spaceship->lowestPosition.x) /
                               2);

                // Get new X and Y speed.
                newBallXSpeed = ballSpeed * sin(angle);
                newBallYSpeed = ballSpeed * cos(angle);
            }

            // Set new speed.
            gameBall->SetVelocity(newBallXSpeed, newBallYSpeed);
        }
    }
}

void hw1::Editor::CheckCollisionBricks() {
    glm::vec3 ballPosition = this->gameBall->GetPosition();
    float ballRadius = this->gameBall->GetRadius();

    for (auto iterator = bricks.begin(); iterator != bricks.end();) {
        if (iterator->collisionBox.IsCollision(gameBall->collisionBox)) {
            /**
             * NOTE: Need to check which type of collision has occured.
             * If there is a vertical collision, we just invert the X speed.
             * If there is a horizontal collision, we just invert the Y speed.
             */
            float brickBottomLeftEdge = iterator->collisionBox.min.x;
            float brickBottomRightEdge = iterator->collisionBox.max.x;
            float brickTopLeftEdge = iterator->collisionBox.min.y;
            float brickTopRightEdge = iterator->collisionBox.max.y;

            float ballBottomLeftEdge = this->gameBall->collisionBox.min.x;
            float ballBottomRightEdge = this->gameBall->collisionBox.max.x;
            float ballTopLeftEdge = this->gameBall->collisionBox.min.y;
            float ballTopRightEdge = this->gameBall->collisionBox.max.y;

            // Compute overlap along both axes
            float overlapX =
                std::min(ballBottomRightEdge - brickBottomLeftEdge,
                         brickBottomRightEdge - ballBottomLeftEdge);
            float overlapY = std::min(ballTopRightEdge - brickTopLeftEdge,
                                      brickTopRightEdge - ballTopLeftEdge);

            float newBallXSpeed = 0;
            float newBallYSpeed = 0;

            // Check which overlap is smaller.
            if (overlapX < overlapY) {
                newBallXSpeed = -this->gameBall->GetXSpeed();
                newBallYSpeed = this->gameBall->GetYSpeed();
            } else {
                newBallXSpeed = this->gameBall->GetXSpeed();
                newBallYSpeed = -this->gameBall->GetYSpeed();
            }

            // Set new ball speed.
            gameBall->SetVelocity(newBallXSpeed, newBallYSpeed);

            iterator->SetNumberOfLives(iterator->GetNumberOfLives() - 1);

            // If number of lives == 0 => delete brick.
            if (iterator->GetNumberOfLives() <= 0)
                iterator = bricks.erase(iterator);

            gameScore += 10;

            // No bricks remaining => Player won.
            if (bricks.empty()) {
                hasGameStarted = false;
                hasWon = true;
            }

            return;
        } else {
            ++iterator;
        }
    }
}

void hw1::Editor::CheckCollision() {
    /**
     * WARN: For each frame, we only check for ONE collision. If we try mulitple
     * collisions per frame, it gets really buggy.
     */
    glm::vec3 ballPosition = gameBall->GetPosition();

    float radius = gameBall->GetRadius();
    float speed = GAME_BALL_SPEED;

    // Check if ball collides with a wall.
    if (ballPosition.x - radius <= 0.0f || ballPosition.x + radius >= 500.0f) {
        gameBall->SetVelocity(-gameBall->GetXSpeed(), gameBall->GetYSpeed());
        return;
    }

    if (ballPosition.y + radius >= 266.0f) {
        gameBall->SetVelocity(gameBall->GetXSpeed(), -gameBall->GetYSpeed());
        return;
    }

    // Ball reached the bottom part of the screen. You either lose a life or, if
    // you have 0 lives left, lose the game.
    if (ballPosition.y - radius <= 0.0f) {
        numberOfLives--;

        if (this->numberOfLives == 0) {
            this->hasLost = true;
            this->hasGameStarted = false;

            return;
        }

        // Restart positions.
        this->PlaceBallStartingPosition();
        this->PlaceSpaceShipStartPosition();

        return;
    }

    // If no collision with walls, check other objects.
    this->CheckCollisionSpaceShip();
    this->CheckCollisionBricks();
}
