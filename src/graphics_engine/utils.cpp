#include "utils.h"
#include <math.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

float DegToRad(float i) {
	float pi = 3.14159265359;

	return i * pi / 180;
}

float RadToDeg(float i) {
	float pi = 3.14159265359;

	return i * 180.0 / pi;
}

float* getNewellsMethod(std::vector<float> x, std::vector<float> y, std::vector<float> z) {
	if (x.size() != y.size() || x.size() != z.size())
		return NULL;

	float *normal = new float[3];
	int nVertices = x.size();

	for (unsigned int i = 0; i < 3; i++) {
		normal[i] = 0.0;
	}

	for (unsigned int i = 0; i < nVertices; i++) {
		int indiceNextVertex = (i + 1) % nVertices;

		normal[0] += (y[i] - y[indiceNextVertex]) * (z[i] + z[indiceNextVertex]);
		normal[1] += (z[i] - z[indiceNextVertex]) * (x[i] + x[indiceNextVertex]);
		normal[2] += (x[i] - x[indiceNextVertex]) * (y[i] + y[indiceNextVertex]);

	}

	float norma = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));

	for (unsigned int i = 0; i < 3; i++) {
		normal[i] = normal[i] / norma;
	}

	return normal;
}

float* getNewellsMethodSecondForm(std::vector<float> x, std::vector<float> y, std::vector<float> z, float x_center,
		float y_center, float z_center) {
	if (x.size() != y.size() || x.size() != z.size())
		return NULL;

	std::vector<float> normais_x, normais_y, normais_z;
	float *normal = new float[3];
	int nVertices = x.size();

	for (unsigned int i = 0; i < 3; i++) {
		normal[i] = 0.0;
	}

	for (unsigned int i = 0; i < nVertices; i++) {
		int indiceNextVertex = (i + 1) % nVertices;
		float Ux = (x[i] - x_center), Uy = (y[i] - y_center), Uz = (z[i] - z_center);
		float Vx = (x[indiceNextVertex] - x_center), Vy = (y[indiceNextVertex] - y_center), Vz = (z[indiceNextVertex]
				- z_center);
		normais_x.push_back((Uy * Vz) - (Uz * Vy));
		normais_y.push_back((Uz * Vx) - (Ux * Vz));
		normais_z.push_back((Ux * Vy) - (Uy * Vx));
	}

	for (unsigned int i = 0; i < normais_x.size(); i++) {
		normal[0] += normais_x[i];
		normal[1] += normais_y[i];
		normal[2] += normais_z[i];
	}

	float norma = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));

	for (unsigned int i = 0; i < 3; i++) {
		normal[i] = normal[i] / norma;
	}

	return normal;

}

float* getNormalsAlternative(std::vector<float> x, std::vector<float> y, std::vector<float> z, float x_center,
		float y_center, float z_center) {
	if (x.size() != y.size() || x.size() != z.size())
		return NULL;

	float *normal = new float[3];
	int nVertices = x.size();

	for (unsigned int i = 0; i < 3; i++) {
		normal[i] = 0.0;
	}

	for (unsigned int i = 0; i < nVertices - 1; i++) {
		float a[3] = { 0.0, 0.0, 0.0 };
		float b[3] = { 0.0, 0.0, 0.0 };

		a[0] = x[i] - x_center;
		a[1] = y[i] - y_center;
		a[2] = z[i] - z_center;

		b[0] = x[i + 1] - x_center;
		b[1] = y[i + 1] - y_center;
		b[2] = z[i + 1] - z_center;

		normal[0] += (a[1] * b[2]) - (b[1] * a[2]);
		normal[1] += (a[2] * b[0]) - (b[2] * a[0]);
		normal[2] += (a[0] * b[1]) - (b[0] * a[1]);

	}

	float norma = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));

	for (unsigned int i = 0; i < 3; i++) {
		normal[i] = normal[i] / norma;
	}

	return normal;
}

char *strdup(char *str1, const char *str2) {
	if (str1 == NULL || str2 == NULL) {
		return NULL;
	} else {
		return strcpy(str1, str2);
	}
}

float distanceTwoPoints(Point* p1, Point* p2) {
	float dist;

	dist = sqrt(
			pow(fabs(p1->getX() - p2->getX()), 2) + pow(fabs(p1->getY() - p2->getY()), 2)
					+ pow(fabs(p1->getZ() - p2->getZ()), 2));

	return dist;
}

float crossProduct(Point* p1, Point* p2) {

	float product = p1->getX() * p2->getX() + p1->getY() * p2->getY() + p1->getZ() * p2->getZ();

	return product;
}

float vectorSize(Point* p1) {
	return sqrt(pow(p1->getX(), 1) + pow(p1->getY(), 2) + pow(p1->getZ(), 2));
}
