#ifndef CUBE_H
#define CUBE_H

#include <glm/ext.hpp>

typedef enum inView {
	NOLOAD,
	TOCHECK,
	LOAD
} inView;

class Cube {
public:
	glm::vec3 cubePos;
	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;
	inView view;

	Cube(glm::vec3 pos) {
		cubePos = pos;
		minX = pos.x - 0.5;
		maxX = pos.x + 0.5;

		minY = pos.y - 0.5;
		maxY = pos.y + 0.5;

		minZ = pos.z - 0.5;
		maxZ = pos.z + 0.5;

		view = TOCHECK;
	}

	void rayIntersection(glm::vec3 ray) {
		return;
	}
};

#endif CUBE_H