/**
 * @file rectangle.cpp
 * @brief Implements all methods from Square class.
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/rectangle.h"

hw1::Rectangle::Rectangle(Mesh* mesh, glm::vec3 position, glm::vec3 color,
                          float length, float width, bool active)
    : hw1::Object(mesh, position, color, active),
      length(length),
      numberOfLives(3) {
    this->collisionBox.min =
        glm::vec3(position.x - length / 2, position.y - width / 2, 0);

    this->collisionBox.max =
        glm::vec3(position.x + length / 2, position.y + width / 2, 0);
}

Mesh* hw1::CreateRectangle(const std::string& name, float length, float width,
                           glm::vec3 color, bool fill) {
    // Offset so that rectangle position is the center.
    glm::vec3 offset = glm::vec3(-length / 2.0f, -width / 2.0f, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(offset, color),
        VertexFormat(offset + glm::vec3(length, 0, 0), color),
        VertexFormat(offset + glm::vec3(length, width, 0), color),
        VertexFormat(offset + glm::vec3(0, width, 0), color)};

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices.
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}
