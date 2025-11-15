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
    this->gameBall->SetPosition(
        glm::vec3(this->spaceship->GetCenterPosition().x,
                  this->spaceship->highestPosition.y + 25.0f, 0));

    // Start ball movement with random X coordinate direction.
    if (this->hasGameStarted) {
        int randomDirection = rand() % 2;

        this->gameBall->velocity.x =
            randomDirection % 2 == 0 ? GAME_BALL_SPEED : -GAME_BALL_SPEED;
        this->gameBall->velocity.y = GAME_BALL_SPEED;
    }
}

void hw1::Editor::UpdateBallPosition(float deltaTimeSeconds) {
    if (!this->hasGameStarted) {
        this->PlaceBallStartingPosition();
        return;
    }

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
    if (!this->hasGameStarted) return;

    glm::vec3 ballPosition = this->gameBall->GetPosition();
    float ballRadius = this->gameBall->GetRadius();

    // Determines the object that the ball has made "the most amount of
    // collision" with. Determined by the area of overlap created when making
    // contact.
    Object* collidedObject = nullptr;
    float biggestOverlapArea = -1.0f;

    // Go through every object that has collided with the ball.
    for (auto& object : this->spaceship->components) {
        if (object->collisionBox.IsCollision(this->gameBall->collisionBox)) {
            // Calculate collision area.
            float overlapArea = object->collisionBox.getOverlapArea(
                this->gameBall->collisionBox);

            if (overlapArea > biggestOverlapArea) {
                biggestOverlapArea = overlapArea;
                collidedObject = object;
            }
        }
    }

    // If ball did not collide with any spaceship object, return.
    if (!collidedObject) return;

    // Calculate X and Y overlap.
    float overlapX =
        collidedObject->collisionBox.getOverlapX(this->gameBall->collisionBox);
    float overlapY =
        collidedObject->collisionBox.getOverlapY(this->gameBall->collisionBox);

    float xSpeed = this->gameBall->GetVelocity().x;
    float ySpeed = this->gameBall->GetVelocity().y;

    float newBallXSpeed = 0.0f;
    float newBallYSpeed = 0.0f;

    // The axis with less overlap determines the type of collision. (Horizontal
    // / vertical).
    if (overlapX < overlapY) {
        // X axis collision detected. (Ball comes from left / right).
        newBallXSpeed = -xSpeed;
        newBallYSpeed = ySpeed;

        // To prevent ball from sticking to spaceship, move it out of spaceship
        // hitbox.
        float offsetX = overlapX + 0.5f;

        // If ball comes from left side, move ball out of hitbox to the left.
        // Same thing for the right side.
        if (ballPosition.x < collidedObject->GetPosition().x)
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(offsetX, 0.0f, 0.0f));
        else
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(offsetX, 0.0f, 0.0f));
    } else {
        // Y axis collision detected. (Ball comes upward / downward).
        // Calculate current ball speed.

        // Calculate ball angle attack compared to spaceship center.
        float angle =
            (this->gameBall->position.x - spaceship->GetCenterPosition().x) /
            ((float)(this->spaceship->highestPosition.x -
                     this->spaceship->lowestPosition.x) /
             2.0f);

        // Clamp value between [-1, 1].
        angle = glm::clamp(angle, -1.0f, 1.0f);
        // Max angle should be 60 degrees. (Prevent angles from being close to
        // 90 degrees).
        angle *= glm::radians(60.0f);

        newBallXSpeed = sin(angle);
        newBallYSpeed = cos(angle);

        float offsetY = overlapY + 0.5f;

        float ballCenterY = this->gameBall->GetPosition().y;
        float objectCenterY = collidedObject->GetPosition().y;

        // Change position depending on ball direction.
        // NOTE: Ball coming from below the spaceship is rare, but needs to be
        // handled. If not, ball keeps same Y speed and gets stuck below
        // spaceship.
        if (ballCenterY > objectCenterY)
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(0.0f, offsetY, 0.0f));
        else {
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(0.0f, offsetY, 0.0f));
            newBallYSpeed = -newBallYSpeed;
        }
    }

    // Set new ball velocity. Normalize so we keep same speed.
    glm::vec2 dir = glm::normalize(glm::vec2(newBallXSpeed, newBallYSpeed));
    this->gameBall->SetVelocity(dir * GAME_BALL_SPEED);
}

void hw1::Editor::CheckCollisionBricks() {
    glm::vec3 ballPosition = this->gameBall->GetPosition();
    float ballRadius = this->gameBall->GetRadius();

    // Determines the brick that the ball has made "the most amount of
    // collision" with. Determined by the area of overlap created when making
    // contact.
    Rectangle* collidedBrick = nullptr;
    float biggestOverlapArea = -1.0f;

    // Go through every brick that has collided with the ball.
    for (auto& brick : this->bricks) {
        if (brick.collisionBox.IsCollision(this->gameBall->collisionBox)) {
            // Calculate collision area.
            float overlapArea =
                brick.collisionBox.getOverlapArea(this->gameBall->collisionBox);

            if (overlapArea > biggestOverlapArea) {
                biggestOverlapArea = overlapArea;
                collidedBrick = &brick;
            }
        }
    }

    // If no bricks made contact with the ball, return.
    if (!collidedBrick) return;

    // Calculate X and Y overlap.
    float overlapX =
        collidedBrick->collisionBox.getOverlapX(this->gameBall->collisionBox);
    float overlapY =
        collidedBrick->collisionBox.getOverlapY(this->gameBall->collisionBox);

    float newBallXSpeed = 0.0f;
    float newBallYSpeed = 0.0f;

    // The axis with less overlap determines the type of collision. (Horizontal
    // / vertical).
    if (overlapX < overlapY) {
        // X axis collision detected. (Ball comes from left / right).
        newBallXSpeed = -this->gameBall->GetXSpeed();
        newBallYSpeed = this->gameBall->GetYSpeed();

        float offsetX = overlapX + 0.5f;

        // If ball comes from left side, move ball out of hitbox to the left.
        // Same thing for the right side.
        if (ballPosition.x < collidedBrick->GetPosition().x)
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(offsetX, 0.0f, 0.0f));
        else
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(offsetX, 0.0f, 0.0f));
    } else {
        // Y axis collision detected. (Ball comes upward / downward).
        newBallXSpeed = this->gameBall->GetXSpeed();
        newBallYSpeed = -this->gameBall->GetYSpeed();

        float offsetY = overlapY + 0.5f;

        // If the ball comes upwards, move ball downwards.
        // Same thing for downwards.
        if (ballPosition.y < collidedBrick->GetPosition().y)
            this->gameBall->SetPosition(ballPosition -
                                        glm::vec3(0.0f, offsetY, 0.0f));
        else
            this->gameBall->SetPosition(ballPosition +
                                        glm::vec3(0.0f, offsetY, 0.0f));
    }

    // Set new ball velocity. Normalize so ball keeps constant speed.
    glm::vec2 dir = glm::normalize(glm::vec2(newBallXSpeed, newBallYSpeed));
    this->gameBall->SetVelocity(dir * GAME_BALL_SPEED);

    // Remove a life from bricks.
    collidedBrick->SetNumberOfLives(collidedBrick->GetNumberOfLives() - 1);

    // Change color of brick to new life color if brick has one or more lives.
    // Else remove brick.
    if (collidedBrick->GetNumberOfLives() > 0) {
        collidedBrick->Shrink();
        collidedBrick->SetColor(
            this->colors[collidedBrick->GetNumberOfLives() - 1]);
    } else
        this->RemoveBrick(collidedBrick->GetPosition());

    // Add 10 points for hitting a brick.
    this->gameScore += 10;

    // If there are no more bricks, player won.
    if (this->bricks.empty()) {
        this->hasGameStarted = false;
        this->hasWon = true;
    }

    // Make camera shake for GAME_SHAKE_TIME.
    this->isShaking = true;
    this->shakeTime = GAME_SHAKE_TIME;
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
        this->PlaceSpaceShipStartPosition();
        this->PlaceBallStartingPosition();

        return;
    }

    // If no collision with walls, check other objects.
    this->CheckCollisionSpaceShip();
    this->CheckCollisionBricks();
}

void hw1::Editor::RemoveBrick(const glm::vec3& position) {
    for (auto it = this->bricks.begin(); it != this->bricks.end(); ++it) {
        if (it->GetPosition() == position) {
            this->bricks.erase(it);

            return;
        }
    }
}
