#include <random>

#include "VertexData.h"
#include "Constants.h"

VertexData::VertexData() {
	setVertexData();
}

void VertexData::setVertexData() {
	// pick random lengths for sides for some variability

	std::random_device rd;
	std::mt19937 engine(rd());

	std::uniform_real_distribution<> distribution(0.5, 1.5);

	float sideLength = distribution(engine);
	float x_vertex = sideLength / 2.0;
	float y_vertex = sideLength / 2.0;

	float data[] = {
		x_vertex, y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		x_vertex, -y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		0.0, 0.0, 0.0, COLOR_SKY_BLUE[0], COLOR_SKY_BLUE[1], COLOR_SKY_BLUE[2],

		x_vertex, -y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		-x_vertex, -y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		0.0, 0.0, 0.0, COLOR_SKY_BLUE[0], COLOR_SKY_BLUE[1], COLOR_SKY_BLUE[2],

		-x_vertex, -y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		-x_vertex, y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		0.0, 0.0, 0.0, COLOR_SKY_BLUE[0], COLOR_SKY_BLUE[1], COLOR_SKY_BLUE[2],

		-x_vertex, y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		x_vertex, y_vertex, 0.0, COLOR_ROYAL_BLUE[0], COLOR_ROYAL_BLUE[1], COLOR_ROYAL_BLUE[2],
		0.0, 0.0, 0.0, COLOR_SKY_BLUE[0], COLOR_SKY_BLUE[1], COLOR_SKY_BLUE[2],
	};

	for (int i = 0; i < 12; ++i)
		vertexData[i] = data[i];
};

float* VertexData::getVertexColorAndPositions() {
	return vertexData;
}
