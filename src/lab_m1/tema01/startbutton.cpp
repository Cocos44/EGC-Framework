/**
 * @file startbutton.cpp
 *
 * @brief Implements all methods from StartButton class + mesh creation.
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/startbutton.h"

hw1::StartButton::StartButton(Mesh* mesh, glm::vec3 position, glm::vec3 color,
                              float leftSideLength, bool active)
    : hw1::Object(mesh, position, color, active),
      leftSideLength(leftSideLength) {}

void hw1::StartButton::SetColor(const glm::vec3& color) {
    for (auto& vertex : this->GetMesh()->vertices) vertex.color = color;

    this->GetMesh()->InitFromData(this->GetMesh()->vertices,
                                  this->GetMesh()->indices);
}

Mesh* hw1::CreateStartButton(const std::string& name, float length,
                             glm::vec3 color, bool fill) {
    glm::vec3 offset = glm::vec3(-length / 2.0f, -length / 2.0f, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(offset, color),
        VertexFormat(offset + glm::vec3(0, length, 0), color),
        VertexFormat(offset + glm::vec3(length, length, 0), color),
        VertexFormat(offset + glm::vec3(length / 2, length / 2, 0), color),
        VertexFormat(offset + glm::vec3(length, 0, 0), color)};

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = {3, 0, 1, 3, 1, 2, 3, 0, 4};

    square->SetDrawMode(GL_TRIANGLES);

    square->InitFromData(vertices, indices);
    return square;
}
