/**
 * @file object2D.cpp
 * @brief Implementation for mesh functions declared in header file.
 * @author Grigoras Vlad Andrei.
 * @see object2D.h
 */

#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object2D::CreateSquare(const std::string& name,
                             glm::vec3 leftBottomCorner, float length,
                             glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(leftBottomCorner, color),
        VertexFormat(leftBottomCorner + glm::vec3(length, 0, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(length, length, 0), color),
        VertexFormat(leftBottomCorner + glm::vec3(0, length, 0), color)};

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
