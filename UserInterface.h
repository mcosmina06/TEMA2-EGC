#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace UserInterface
{
	Mesh* CreateRectangle1(const char* name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateMesh1(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
}

