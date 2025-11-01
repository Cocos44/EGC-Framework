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

void hw1::Editor::DrawGameBall() {
    if (this->hasGameStarted) {
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix = this->visMatrix *
                      transform2D::Translate(this->gameBall->GetPosition().x,
                                             this->gameBall->GetPosition().y);

        // Render mesh.
        RenderMesh2D(this->gameBall->GetMesh(), shaders["VertexColor"],
                     modelMatrix);
    }
}
