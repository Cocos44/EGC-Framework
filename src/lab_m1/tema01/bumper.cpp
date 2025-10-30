/**
 * @file bumper.cpp
 * @brief Implements all methods of Bumper class + bumper mesh creation.
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/bumper.h"

hw1::Bumper::Bumper(Mesh* mesh, glm::vec3 position, glm::vec3 color,
                    bool active)
    : Object(mesh, position, color, active) {}

Mesh* hw1::CreateBumperMesh(std::string name, float squareLength,
                            glm::vec3 squareColor, glm::vec3 semicircleColor,
                            bool fill) {
    // Offset so that square is centered at (0, 0)
    glm::vec3 offset = glm::vec3(-squareLength / 2.0f, -squareLength / 2.0f, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(offset, squareColor),
        VertexFormat(glm::vec3(squareLength, 0, 0) + offset, squareColor),
        VertexFormat(glm::vec3(squareLength, squareLength, 0) + offset,
                     squareColor),
        VertexFormat(glm::vec3(0, squareLength, 0) + offset, squareColor)};

    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    int segments = 100;
    float radius = 1.5f * squareLength;
    glm::vec3 center = glm::vec3(0, squareLength / 2, 0);

    int startIndex = vertices.size();
    vertices.push_back(VertexFormat(center, semicircleColor));

    for (int i = 0; i <= segments; i++) {
        float angle = M_PI * i / segments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), semicircleColor));
    }

    // Indices for semicircle (triangle fan).
    for (int i = 1; i <= segments; i++) {
        indices.push_back(startIndex);
        indices.push_back(startIndex + i);
        indices.push_back(startIndex + i + 1);
    }

    Mesh* bumperMesh = new Mesh(name);

    if (!fill) {
        bumperMesh->SetDrawMode(GL_LINE_LOOP);
    } else {
        bumperMesh->SetDrawMode(GL_TRIANGLES);
    }

    bumperMesh->InitFromData(vertices, indices);

    return bumperMesh;
}
