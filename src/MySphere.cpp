#include "MySphere.h"
#include "GL/glut.h"

MySphere::MySphere() :
		MyPrimitive() {
	this->radius = 0.5;
	this->slices = 10;
	this->stacks = 10;

	q = gluNewQuadric();
	gluQuadricTexture(q, true);
}

MySphere::MySphere(float radius, unsigned int slices, unsigned int stacks) :
		MyPrimitive() {
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;

	q = gluNewQuadric();
	gluQuadricTexture(q, true);
}

MySphere::~MySphere() {
	gluDeleteQuadric(q);
}

void MySphere::draw() {
	gluSphere(q, radius, slices, stacks);
}
