/**
 * @file draw_object_create.cpp
 *
 * @brief Implements mesh and object drawing methods of Editor class.
 *
 * Only draws objects related to the game.
 *
 * @see Editor
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/editor.h"

void hw1::Editor::DrawGameBricks() {
    if (this->hasGameStarted) {
        glm::mat3 modelMatrix = glm::mat3(1);

        // Go through every game rectangle to render it.
        for (auto& rectangle : this->bricks) {
            if (rectangle.IsActive()) {
                // Move the rectangle to wanted position.
                modelMatrix = this->visMatrix *
                              transform2D::Translate(rectangle.GetPosition().x,
                                                     rectangle.GetPosition().y);

                // Render mesh.
                RenderMesh2D(rectangle.GetMesh(), shaders["VertexColor"],
                             modelMatrix);

                // Reset modelMatrix.
                modelMatrix = glm::mat3(1);
            }
        }
    }
}

void hw1::Editor::DrawGameBall(float deltaTimeSeconds) {
    if (this->hasGameStarted) {
        this->CheckCollision();
        this->UpdateBallPosition(deltaTimeSeconds);

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix = this->visMatrix *
                      transform2D::Translate(this->gameBall->GetPosition().x,
                                             this->gameBall->GetPosition().y);

        // Render mesh.
        RenderMesh2D(this->gameBall->GetMesh(), shaders["VertexColor"],
                     modelMatrix);
    }
}

void hw1::Editor::DrawGameText() {
    if (!this->hasGameStarted) {
        if (this->hasLost) {
            glm::vec3 startingPosition = visMatrix * glm::vec3(170, 100, 0);
            this->textRenderer.RenderText("YOU LOSE!", startingPosition.x,
                                          startingPosition.y, 3.0f,
                                          glm::vec3(1.0f, 1.0f, 1.0f));

            std::string gameScoreText =
                "Score gained: " + std::to_string(this->gameScore);

            startingPosition = visMatrix * glm::vec3(176, 150, 0);

            this->textRenderer.RenderText(gameScoreText, startingPosition.x,
                                          startingPosition.y, 1.5f,
                                          glm::vec3(1.0f, 1.0f, 1.0f));

        } else if (this->hasWon) {
            glm::vec3 startingPosition = visMatrix * glm::vec3(170, 100, 0);
            this->textRenderer.RenderText("YOU WIN!", startingPosition.x,
                                          startingPosition.y, 3.0f,
                                          glm::vec3(1.0f, 1.0f, 1.0f));

            std::string gameScoreText =
                "Score gained: " + std::to_string(this->gameScore);

            startingPosition = visMatrix * glm::vec3(172, 150, 0);

            this->textRenderer.RenderText(gameScoreText, startingPosition.x,
                                          startingPosition.y, 1.5f,
                                          glm::vec3(1.0f, 1.0f, 1.0f));
        } else {
            glm::vec3 startingPosition = visMatrix * glm::vec3(170, 5, 0);
            this->textRenderer.RenderText(
                "Press [Space Bar] To Start", startingPosition.x,
                startingPosition.y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    } else {
        // Render game score.
        std::string gameScoreText = "Score: " + std::to_string(this->gameScore);

        glm::vec3 startingPosition = visMatrix * glm::vec3(2, 5, 0);

        this->textRenderer.RenderText(gameScoreText, startingPosition.x,
                                      startingPosition.y, 0.7f,
                                      glm::vec3(1.0f, 1.0f, 1.0f));

        // Render number of lives.
        std::string numberOfLivesText =
            "Lives: " + std::to_string(this->numberOfLives);

        startingPosition = visMatrix * glm::vec3(460, 5, 0);

        this->textRenderer.RenderText(numberOfLivesText, startingPosition.x,
                                      startingPosition.y, 0.7f,
                                      glm::vec3(1.0f, 1.0f, 1.0f));
    }
}
