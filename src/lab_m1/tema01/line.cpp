/**
 * @file line.cpp
 * @brief Implements all methods of Line class.
 * @see line.h
 * @author Grigoras Vlad Andrei
 */

#include "lab_m1/tema01/line.h"

#include <cmath>

hw1::Line::Line(Mesh* mesh, glm::vec3 color, glm::vec3 start, glm::vec3 end,
                bool active)
    : hw1::Object(mesh, start, color, active) {
    this->start = start;
    this->end = end;
    this->length = glm::distance(start, end);
    this->angle = atan2(end.y - start.y, end.x - start.x);
}

Mesh* hw1::CreateLine(const std::string& name, glm::vec3 color) {
    // Line only contains 2 vertices: start and end vertex.
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(1, 0, 0), color)};

    std::vector<unsigned int> indices = {0, 1};

    Mesh* line = new Mesh(name);

    line->SetDrawMode(GL_LINES);
    line->InitFromData(vertices, indices);

    return line;
}
