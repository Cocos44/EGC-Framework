/**
 * @file object2D.cpp
 * @brief Implementation for mesh functions declared in header file.
 * @author Grigoras Vlad Andrei.
 * @see object2D.h
 */

#include "lab_m1/tema01/include/object.h"

hw1::Object::Object(Mesh* mesh, glm::vec3 position, glm::vec3 color,
                    bool active)
    : mesh(mesh), position(position), color(color), active(active) {}
