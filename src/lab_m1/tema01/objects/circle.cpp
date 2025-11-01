/**
 * @file circle.cpp
 *
 * @brief Implements all methods of Circle class + circle mesh creation.
 *
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/include/circle.h"

#include "glm/common.hpp"

hw1::Circle::Circle(Mesh* mesh, glm::vec3 position, glm::vec3 color,
                    float radius, bool active)
    : Object(mesh, position, color, active),
      radius(radius),
      velocity(glm::vec2(0, 0)) {
    this->collisionBox.min =
        glm::vec3(position.x - radius, position.y - radius, 0);

    this->collisionBox.max =
        glm::vec3(position.x + radius, position.y + radius, 0);
}

Mesh* hw1::CreateCircle(const std::string& name, float radius, glm::vec3 color,
                        bool fill) {
    // Offset so that circle is positioned in center.
    std::vector<VertexFormat> vertices = {};
    std::vector<unsigned int> indices = {};

    int segments = 100;
    glm::vec3 center = glm::vec3(0, 0, 0);

    int startIndex = vertices.size();
    vertices.push_back(VertexFormat(center, color));

    // Rotate 360 degrees to get full circle.
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
    }

    for (int i = 1; i <= segments; i++) {
        indices.push_back(startIndex);
        indices.push_back(startIndex + i);
        indices.push_back(startIndex + i + 1);
    }

    Mesh* circleMesh = new Mesh(name);

    if (!fill)
        circleMesh->SetDrawMode(GL_LINE_LOOP);
    else
        circleMesh->SetDrawMode(GL_TRIANGLES);

    circleMesh->InitFromData(vertices, indices);

    return circleMesh;
}
