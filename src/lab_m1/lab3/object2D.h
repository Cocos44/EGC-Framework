#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object2D {

Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner,
                   float length, glm::vec3 color, bool fill = false);

Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner,
                     float base, float height, glm::vec3 color,
                     bool fill = false);
}  // namespace object2D
