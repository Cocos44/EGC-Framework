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

#include <algorithm>

#include "lab_m1/tema01/include/editor.h"

using std::find;

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

    this->gameBall->xAxisOrientation = true;
    this->gameBall->yAxisOrientation = true;
}

void hw1::Editor::UpdateBallPosition(float deltaTimeSeconds) {
    // Check ball orientation and based on it either add / subtract.
    glm::vec3 nextPositionOffset = glm::vec3(0, 0, 0);
    nextPositionOffset.x = this->gameBall->xAxisOrientation
                               ? GAME_BALL_SPEED * deltaTimeSeconds
                               : -GAME_BALL_SPEED * deltaTimeSeconds;
    nextPositionOffset.y = this->gameBall->yAxisOrientation
                               ? GAME_BALL_SPEED * deltaTimeSeconds
                               : -GAME_BALL_SPEED * deltaTimeSeconds;

    glm::vec3 updatedPosition =
        this->gameBall->GetPosition() + nextPositionOffset;

    float radius = this->gameBall->GetRadius();
    this->gameBall->collisionBox.min =
        updatedPosition - glm::vec3(radius, radius, 0);
    this->gameBall->collisionBox.max =
        updatedPosition + glm::vec3(radius, radius, 0);

    this->gameBall->SetPosition(updatedPosition);
}

void hw1::Editor::RemoveBrick(const glm::vec3& position) {
    // Find the brick in the vector
    for (auto it = this->bricks.begin(); it != this->bricks.end(); it++) {
        if ((*it).GetPosition() == position) {
            this->bricks.erase(it);
            break;
        }
    }
}
void hw1::Editor::CheckCollision(float deltaTimeSeconds) {
    glm::vec3 ballPosition = this->gameBall->GetPosition();

    if (ballPosition.x - this->gameBall->GetRadius() <= 0.0f)
        this->gameBall->xAxisOrientation = true;

    if (ballPosition.x + this->gameBall->GetRadius() >= 500.0f)
        this->gameBall->xAxisOrientation = false;

    if (ballPosition.y + this->gameBall->GetRadius() >= 266)
        this->gameBall->yAxisOrientation = false;

    if (ballPosition.y - this->gameBall->GetRadius() <= -0.0f) {
        this->numberOfLives--;
        this->PlaceBallStartingPosition();
        this->PlaceSpaceShipStartPosition();
    }

    for (auto& component : this->spaceship->components) {
        Object::AABB::CollisionAxis collisionType =
            component->collisionBox.CheckCollision(
                this->gameBall->collisionBox);

        if (collisionType == Object::AABB::CollisionAxis::X) {
            this->gameBall->xAxisOrientation =
                !this->gameBall->xAxisOrientation;

        } else if (collisionType == Object::AABB::CollisionAxis::Y) {
            this->gameBall->yAxisOrientation =
                !this->gameBall->yAxisOrientation;
        }
    }

    for (auto& component : this->bricks) {
        Object::AABB::CollisionAxis collisionType =
            component.collisionBox.CheckCollision(this->gameBall->collisionBox);

        if (collisionType == Object::AABB::CollisionAxis::X) {
            this->gameBall->xAxisOrientation =
                !this->gameBall->xAxisOrientation;
            this->RemoveBrick(component.position);

        } else if (collisionType == Object::AABB::CollisionAxis::Y) {
            this->gameBall->yAxisOrientation =
                !this->gameBall->yAxisOrientation;

            this->RemoveBrick(component.position);
        }
    }
}
