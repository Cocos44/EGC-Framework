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

    Object* collidedObject = nullptr;
    float biggestOverlapArea = -1.0f;

    for (auto& object : spaceship->components) {
        if (object->collisionBox.IsCollision(gameBall->collisionBox)) {
            float overlapX = std::min(
                gameBall->collisionBox.max.x - object->collisionBox.min.x,
                object->collisionBox.max.x - gameBall->collisionBox.min.x);
            float overlapY = std::min(
                gameBall->collisionBox.max.y - object->collisionBox.min.y,
                object->collisionBox.max.y - gameBall->collisionBox.min.y);

            if (overlapX > 0 && overlapY > 0) {
                float overlapArea = overlapX * overlapY;
                if (overlapArea > biggestOverlapArea) {
                    biggestOverlapArea = overlapArea;
                    collidedObject = object;
                }
            }
        }
    }

    if (!collidedObject) return;

    float objectLeft = collidedObject->collisionBox.min.x;
    float objectRight = collidedObject->collisionBox.max.x;
    float objectBottom = collidedObject->collisionBox.min.y;
    float objectTop = collidedObject->collisionBox.max.y;

    float ballLeft = gameBall->collisionBox.min.x;
    float ballRight = gameBall->collisionBox.max.x;
    float ballBottom = gameBall->collisionBox.min.y;
    float ballTop = gameBall->collisionBox.max.y;

    float overlapX = std::min(ballRight - objectLeft, objectRight - ballLeft);
    float overlapY = std::min(ballTop - objectBottom, objectTop - ballBottom);

    if (overlapX <= 0 || overlapY <= 0) return;

    float newBallXSpeed = 0.0f;
    float newBallYSpeed = 0.0f;

    float xSpeed = this->gameBall->GetXSpeed();
    float ySpeed = this->gameBall->GetYSpeed();

    if (overlapX < overlapY) {
        newBallXSpeed = -xSpeed;
        newBallYSpeed = ySpeed;

        float offsetX = overlapX + 0.5f;
        if (ballPosition.x < collidedObject->GetPosition().x)
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(offsetX, 0.0f, 0.0f));
        else
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(offsetX, 0.0f, 0.0f));
    } else {
        float ballSpeed = sqrt(xSpeed * xSpeed + ySpeed * ySpeed);

        float angle =
            (this->gameBall->position.x - spaceship->GetCenterPosition().x) /
            ((float)(this->spaceship->highestPosition.x -
                     this->spaceship->lowestPosition.x) /
             2.0f);

        angle = glm::clamp(angle, -1.0f, 1.0f);
        angle *= glm::radians(60.0f);

        newBallXSpeed = ballSpeed * sin(angle);
        newBallYSpeed = ballSpeed * cos(angle);

        // Determine whether ball hit from above or below
        float ballCenterY = (ballBottom + ballTop) / 2.0f;
        float objectCenterY = (objectBottom + objectTop) / 2.0f;

        if (ballCenterY > objectCenterY)
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(0.0f, 3.0f, 0.0f));
        else {
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(0.0f, 3.0f, 0.0f));
            newBallYSpeed = -newBallYSpeed;
        }
    }

    gameBall->SetVelocity(newBallXSpeed, newBallYSpeed);
}

void hw1::Editor::CheckCollisionBricks() {
    glm::vec3 ballPosition = this->gameBall->GetPosition();
    float ballRadius = this->gameBall->GetRadius();

    Rectangle* collidedBrick = nullptr;
    float biggestOverlapArea = -1.0f;

    for (auto& brick : bricks) {
        if (brick.collisionBox.IsCollision(gameBall->collisionBox)) {
            float overlapX = std::min(
                gameBall->collisionBox.max.x - brick.collisionBox.min.x,
                brick.collisionBox.max.x - gameBall->collisionBox.min.x);
            float overlapY = std::min(
                gameBall->collisionBox.max.y - brick.collisionBox.min.y,
                brick.collisionBox.max.y - gameBall->collisionBox.min.y);

            if (overlapX > 0 && overlapY > 0) {
                float overlapArea = overlapX * overlapY;
                if (overlapArea > biggestOverlapArea) {
                    biggestOverlapArea = overlapArea;
                    collidedBrick = &brick;
                }
            }
        }
    }

    if (!collidedBrick) return;

    float brickLeft = collidedBrick->collisionBox.min.x;
    float brickRight = collidedBrick->collisionBox.max.x;
    float brickBottom = collidedBrick->collisionBox.min.y;
    float brickTop = collidedBrick->collisionBox.max.y;

    float ballLeft = this->gameBall->collisionBox.min.x;
    float ballRight = this->gameBall->collisionBox.max.x;
    float ballBottom = this->gameBall->collisionBox.min.y;
    float ballTop = this->gameBall->collisionBox.max.y;

    // Compute overlap
    float overlapX = std::min(ballRight - brickLeft, brickRight - ballLeft);
    float overlapY = std::min(ballTop - brickBottom, brickTop - ballBottom);

    if (overlapX <= 0 || overlapY <= 0) return;

    float xSpeed = this->gameBall->GetXSpeed();
    float ySpeed = this->gameBall->GetYSpeed();
    float newBallXSpeed = xSpeed;
    float newBallYSpeed = ySpeed;
    float epsilon = 0.5f;

    if (overlapX < overlapY) {
        newBallXSpeed = -xSpeed;
        newBallYSpeed = ySpeed;

        float offsetX = overlapX + epsilon;
        if (ballPosition.x < collidedBrick->GetPosition().x)
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(offsetX, 0.0f, 0.0f));
        else
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(offsetX, 0.0f, 0.0f));
    } else {
        newBallXSpeed = xSpeed;
        newBallYSpeed = -ySpeed;

        float offsetY = overlapY + epsilon;
        if (ballPosition.y < collidedBrick->GetPosition().y)
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(0.0f, offsetY, 0.0f));
        else
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(0.0f, offsetY, 0.0f));
    }

    this->gameBall->SetVelocity(newBallXSpeed, newBallYSpeed);

    collidedBrick->SetNumberOfLives(collidedBrick->GetNumberOfLives() - 1);

    if (collidedBrick->GetNumberOfLives() > 0)
        collidedBrick->SetColor(
            this->colors[collidedBrick->GetNumberOfLives() - 1]);
    else
        bricks.erase(std::remove_if(
                         bricks.begin(), bricks.end(),
                         [&](const Object& b) { return &b == collidedBrick; }),
                     bricks.end());

    gameScore += 10;

    if (bricks.empty()) {
        hasGameStarted = false;
        hasWon = true;
    }
}

void hw1::Editor::CheckCollision() {
    /**
     * WARN: For each frame, we only change the ball position ONCE. If we try
     * mulitple changes per frame, it gets really buggy.
     */
    glm::vec3 ballPosition = this->gameBall->GetPosition();

    float radius = this->gameBall->GetRadius();

    // Check if ball collides with a wall.
    if (ballPosition.x - radius <= 0.0f) {
        this->gameBall->SetPosition(ballPosition + glm::vec3(1.0f, 0.0f, 0.0f));
        this->gameBall->SetVelocity(-this->gameBall->GetXSpeed(),
                                    this->gameBall->GetYSpeed());
        return;
    }

    if (ballPosition.x + radius >= 500.0f) {
        this->gameBall->SetPosition(ballPosition - glm::vec3(1.0f, 0.0f, 0.0f));
        this->gameBall->SetVelocity(-this->gameBall->GetXSpeed(),
                                    this->gameBall->GetYSpeed());
        return;
    }

    if (ballPosition.y + radius >= 266.0f) {
        this->gameBall->SetPosition(ballPosition - glm::vec3(0.0f, 1.0f, 0.0f));
        this->gameBall->SetVelocity(this->gameBall->GetXSpeed(),
                                    -this->gameBall->GetYSpeed());
        return;
    }

    // Ball reached the bottom part of the screen. You either lose a life or, if
    // you have 0 lives left, lose the game.
    if (ballPosition.y - radius <= 0.0f) {
        this->numberOfLives--;

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
