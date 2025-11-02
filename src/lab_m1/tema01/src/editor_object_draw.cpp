/**
 * @file draw_object_create.cpp
 *
 * @brief Implements mesh and object drawing methods of Editor class.
 *
 * Only draws objects related to the editor.
 *
 * @see Editor
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/editor.h"

void hw1::Editor::DrawBorders() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every line to render it.
    for (const auto& line : this->delimiters) {
        // First move line to it's starting position, then rotate it based on
        // the arctangent calculated in constructor, then scale to the size
        // wanted.
        modelMatrix =
            this->visMatrix *
            transform2D::Translate(line.GetPosition().x, line.GetPosition().y) *
            transform2D::Rotate(line.GetAngle()) *
            transform2D::Scale(line.GetLength(), 1);

        // Make line a bit thicker.
        glLineWidth(2.0f);

        // Render mesh.
        RenderMesh2D(line.GetMesh(), shaders["VertexColor"], modelMatrix);

        // Reset line thickness.
        glLineWidth(1.0f);

        // Reset modelMatrix.
        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawGrid() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every grid square to render it.
    for (auto& square : this->grid) {
        // Move the square to wanted position.
        modelMatrix =
            this->visMatrix * transform2D::Translate(square.GetPosition().x,
                                                     square.GetPosition().y);

        // Render mesh.
        RenderMesh2D(square.GetMesh(), shaders["VertexColor"], modelMatrix);

        // Reset modelMatrix.
        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawChoosingBlocks() {
    glm::mat3 modelMatrix = glm::mat3(1);

    // Go through every choosing object to render it.
    for (auto& object : this->blocksToChoose) {
        std::string meshID = object.GetMesh()->GetMeshID();
        if (meshID == "spaceship_square") {
            // Move the square to wanted position.
            modelMatrix = this->visMatrix *
                          transform2D::Translate(object.GetPosition().x,
                                                 object.GetPosition().y);

            // Render mesh.
            RenderMesh2D(object.GetMesh(), shaders["VertexColor"], modelMatrix);

            // Reset modelMatrix.
            modelMatrix = glm::mat3(1);
        }

        else if (meshID == "spaceship_bumper") {
            // Move the bumper to wanted position.
            modelMatrix = this->visMatrix *
                          transform2D::Translate(object.GetPosition().x,
                                                 object.GetPosition().y);
            // Render mesh.
            RenderMesh2D(object.GetMesh(), shaders["VertexColor"], modelMatrix);

            // Reset modelMatrix.
            modelMatrix = glm::mat3(1);
        }
    }
}

void hw1::Editor::DrawHoldObject() {
    if (this->isLeftButtonHold && (this->buttonHoldObject != nullptr)) {
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix =
            this->visMatrix *
            transform2D::Translate(this->buttonHoldObject->GetPosition().x,
                                   this->buttonHoldObject->GetPosition().y);
        // Render the mesh
        RenderMesh2D(this->buttonHoldObject->GetMesh(), shaders["VertexColor"],
                     modelMatrix);
    }
}

void hw1::Editor::DrawSpaceShip() {
    if (!this->hasWon && !this->hasLost) {
        glm::mat3 modelMatrix = glm::mat3(1);

        // Go through every spaceship object to render it.
        for (auto& object : this->spaceship->components) {
            // Move the square to wanted position.
            modelMatrix = this->visMatrix *
                          transform2D::Translate(object->GetPosition().x,
                                                 object->GetPosition().y);

            // Render mesh.
            RenderMesh2D(object->GetMesh(), shaders["VertexColor"],
                         modelMatrix);

            // Reset modelMatrix.
            modelMatrix = glm::mat3(1);
        }
    }
}

void hw1::Editor::DrawCounterSection() {
    glm::mat3 modelMatrix = glm::mat3(1);

    for (int i = 0; i < COUNTER_NUMBER - this->spaceship->numberOfComponents;
         i++) {
        // Move the square to wanted position.
        modelMatrix =
            this->visMatrix *
            transform2D::Translate(this->componentsCounter[i].GetPosition().x,
                                   this->componentsCounter[i].GetPosition().y);

        // Render mesh.
        RenderMesh2D(this->componentsCounter[i].GetMesh(),
                     shaders["VertexColor"], modelMatrix);

        // Reset modelMatrix.
        modelMatrix = glm::mat3(1);
    }
}

void hw1::Editor::DrawEditorText() {
    // Use visMatrix to convert logic space to screen space.
    glm::vec3 startingPosition = visMatrix * glm::vec3(50, 40, 0);

    // Render the text
    this->textRenderer.RenderText("BREAKOUT", startingPosition.x,
                                  startingPosition.y, 1.5f,
                                  glm::vec3(1.0f, 1.0f, 0.0f));
}

void hw1::Editor::DrawStartButton() {
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix = this->visMatrix *
                  transform2D::Translate(this->startButton->GetPosition().x,
                                         this->startButton->GetPosition().y);

    // Render button color depending on spaceship config.
    if (this->spaceship->IsConfigCorrect())
        this->startButton->SetColor(VEC3_GREEN);
    else
        this->startButton->SetColor(VEC3_RED);

    // Render mesh.
    RenderMesh2D(this->startButton->GetMesh(), shaders["VertexColor"],
                 modelMatrix);
}
