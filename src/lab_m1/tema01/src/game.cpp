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
    this->grid.clear();
    this->componentsCounter.clear();
    this->blocksToChoose.clear();

    delete this->startButton;
    this->startButton = nullptr;

    this->delimiters.clear();
    this->borders.clear();

    this->isGameRunning = true;
    this->PlaceSpaceShipStartPosition();
}

void hw1::Editor::PlaceSpaceShipStartPosition() {
    glm::vec3 offsetToStart =
        this->spaceship->centerPosition - GAME_STARTING_POSITION;

    for (auto& object : this->spaceship->components) {
        object->SetPosition(object->GetPosition() - offsetToStart);
    }
}

void hw1::Editor::PlaceBallStartingPosition() {
    this->gameBall->SetPosition(GAME_BALL_STARTING_POSITION);

    this->gameBall->velocity.x = GAME_BALL_SPEED;
    this->gameBall->velocity.y = GAME_BALL_SPEED;
}

void hw1::Editor::UpdateBallPosition(float deltaTimeSeconds) {
    glm::vec3 nextPositionOffset =
        glm::vec3(this->gameBall->velocity.x, this->gameBall->velocity.y, 0) *
        deltaTimeSeconds;

    glm::vec3 updatedPosition =
        this->gameBall->GetPosition() + nextPositionOffset;
    this->gameBall->SetPosition(updatedPosition);

    // Update AABB for collision
    float radius = this->gameBall->GetRadius();
    this->gameBall->collisionBox.min =
        updatedPosition - glm::vec3(radius, radius, 0);
    this->gameBall->collisionBox.max =
        updatedPosition + glm::vec3(radius, radius, 0);
}

void hw1::Editor::RemoveBrick(const glm::vec3& position) {
    // Find the brick in the vector
    this->gameScore += 10;
    for (auto it = this->bricks.begin(); it != this->bricks.end(); it++) {
        if ((*it).GetPosition() == position) {
            this->bricks.erase(it);
            break;
        }
    }
}
void hw1::Editor::CheckCollision(float deltaTimeSeconds) {
    glm::vec3 ballPosition = gameBall->GetPosition();

    float radius = gameBall->GetRadius();
    float speed = GAME_BALL_SPEED;

    if (ballPosition.x - radius <= 0.0f || ballPosition.x + radius >= 500.0f) {
        gameBall->SetVelocity(-gameBall->GetXSpeed(), gameBall->GetYSpeed());
    }
    if (ballPosition.y + radius >= 266.0f) {
        gameBall->SetVelocity(gameBall->GetXSpeed(), -gameBall->GetYSpeed());
    }
    if (ballPosition.y - radius <= 0.0f) {
        numberOfLives--;
        PlaceBallStartingPosition();
        PlaceSpaceShipStartPosition();
        return;
    }

    for (auto& paddle : spaceship->components) {
        if (paddle->collisionBox.IsCollision(gameBall->collisionBox)) {
            // Paddle bounds
            float paddleLeft = paddle->collisionBox.min.x;
            float paddleRight = paddle->collisionBox.max.x;
            float paddleBottom = paddle->collisionBox.min.y;
            float paddleTop = paddle->collisionBox.max.y;

            // Ball bounds
            float ballLeft = ballPosition.x - radius;
            float ballRight = ballPosition.x + radius;
            float ballBottom = ballPosition.y - radius;
            float ballTop = ballPosition.y + radius;

            // Compute overlap along both axes
            float overlapX =
                std::min(ballRight - paddleLeft, paddleRight - ballLeft);
            float overlapY =
                std::min(ballTop - paddleBottom, paddleTop - ballBottom);

            float newX, newY;
            float speed = GAME_BALL_SPEED;

            if (overlapX < overlapY) {
                // Vertical collision → invert x velocity
                newX = -gameBall->GetXSpeed();
                newY = gameBall->GetYSpeed();
            } else {
                // Horizontal collision (top of paddle) → calculate impact angle
                float paddleY = (paddleTop + paddleBottom) / 2.0f;
                float paddleHeight = paddleTop - paddleBottom;

                float impact =
                    (ballPosition.y - paddleY) / (paddleHeight / 2.0f);
                impact = glm::clamp(impact, -1.0f, 1.0f);

                float angle =
                    impact * glm::radians(60.0f);  // max 60 deg deflection
                newX = speed * sin(angle);
                newY = speed * cos(angle);

                // Ensure ball goes upwards
                if (newY < 0) newY = -newY;
            }

            gameBall->SetVelocity(newX, newY);
            break;
        }
    }

    for (auto it = bricks.begin(); it != bricks.end();) {
        if (it->collisionBox.IsCollision(gameBall->collisionBox)) {
            // Decide which axis to invert based on overlap
            float ballCenterX = ballPosition.x;
            float ballCenterY = ballPosition.y;

            float brickLeft = it->collisionBox.min.x;
            float brickRight = it->collisionBox.max.x;
            float brickBottom = it->collisionBox.min.y;
            float brickTop = it->collisionBox.max.y;

            float overlapX = std::min(ballCenterX + radius - brickLeft,
                                      brickRight - (ballCenterX - radius));
            float overlapY = std::min(ballCenterY + radius - brickBottom,
                                      brickTop - (ballCenterY - radius));

            if (overlapX < overlapY)
                gameBall->SetVelocity(-gameBall->GetXSpeed(),
                                      gameBall->GetYSpeed());
            else
                gameBall->SetVelocity(gameBall->GetXSpeed(),
                                      -gameBall->GetYSpeed());

            it = bricks.erase(it);
            gameScore += 10;
            break;
        } else {
            ++it;
        }
    }
}
