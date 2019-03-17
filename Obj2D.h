#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Obj2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, float latime, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(std::string name, glm::vec3 leftBottomCorner, float diametru, glm::vec3 color, bool fill = false);
	Mesh* CreateStar(std::string name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec3 color);

}

