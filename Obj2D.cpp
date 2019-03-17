#include "Obj2D.h"

#include <Core/Engine.h>

Mesh* Obj2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, float latime, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, latime, 0), color),
		VertexFormat(corner + glm::vec3(0, latime, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Obj2D::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float diametru, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	const int sides = 72;  // The amount of segment to create the circle
	std::vector<VertexFormat> vertices;

	for (int i = 0; i < 360; i += 360 / sides)
	{
		double heading = i * 3.1415926535897932384626433832795 / 180;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(heading) * diametru, sin(heading) * diametru, 0), color));
	}

	Mesh* circle = new Mesh(name);
	std::vector<unsigned short> indices;

	if (!fill) {
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else
		for (int i = 0; i < sides; i++) {
			indices.push_back(i);
			indices.push_back((i + 1) % sides);
			indices.push_back((i + sides / 2) % sides);
		}

	circle->InitFromData(vertices, indices);
	return circle;


}

Mesh* Obj2D::CreateStar(std::string name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices;

	for (int i = 0; i < 360; i += 360 / 5)
	{
		float rad = i * 3.14 / 180;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(rad) * length1, sin(rad) * length1, 0), color));
	}
	for (int i = 36; i < 396; i += 360 / 5)
	{
		float rad = i * 3.14 / 180;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(rad) * length2, sin(rad) * length2, 0), color));
	}

	Mesh* star = new Mesh(name);
	std::vector<unsigned short> indices = { 5, 2, 4, 6, 3, 0, 7, 1, 4, 8, 0, 2, 9, 1, 3 };

	star->InitFromData(vertices, indices);
	return star;
}