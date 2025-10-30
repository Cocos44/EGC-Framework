/**
 * @file square.cpp
 * @brief Implements all methods from Square class.
 * @author Grigoras Vlad Andrei
 */
#include "lab_m1/tema01/square.h"

hw1::Square::Square(Mesh* mesh, glm::vec3 position, glm::vec3 color,
                    float length, bool active)
    : hw1::Object(mesh, position, color, active), length(length) {}

Mesh* hw1::CreateSquare(const std::string& name, float length, glm::vec3 color,
                        bool fill) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(BOTTOM_LEFT_CORNER, color),
        VertexFormat(BOTTOM_LEFT_CORNER + glm::vec3(length, 0, 0), color),
        VertexFormat(BOTTOM_LEFT_CORNER + glm::vec3(length, length, 0), color),
        VertexFormat(BOTTOM_LEFT_CORNER + glm::vec3(0, length, 0), color)};

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
