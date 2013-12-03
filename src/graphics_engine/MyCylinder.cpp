#include "MyCylinder.h"
#include "GL/glut.h"
#include <math.h>

float pi = acos(-1.0);
float deg2rad = pi / 180.0;

MyCylinder::MyCylinder() :
		MyPrimitive() {
	this->base = 0.5;
	this->top = 0.5;
	this->height = 1.0;
	this->slices = 10.0;
	this->stacks = 10.0;

	q = gluNewQuadric();
	gluQuadricTexture(q, true);
}

MyCylinder::MyCylinder(float base, float top, float height, unsigned int slices,
        unsigned int stacks) :
		MyPrimitive() {
	this->base = base;
	this->top = top;
	this->height = height;
	this->slices = slices;
	this->stacks = stacks;

	q = gluNewQuadric();
	gluQuadricTexture(q, true);
}

MyCylinder::~MyCylinder() {
	gluDeleteQuadric(q);
}

void MyCylinder::draw() {
	gluCylinder(q, base, top, height, slices, stacks);

	glPushMatrix();
	glTranslatef(0.0, 0.0, height);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	drawCircle(top);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawCircle(base);
	glPopMatrix();
}

void MyCylinder::drawCircle(float radius) {

	glNormal3f(0.0, 1.0, 0.0);

	float ang_slice = 360.0 / slices;

	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i <= 10; i++) {
		float x = radius * cos(((i * ang_slice) + 90.0) * deg2rad);
		float y = radius * sin(((i * ang_slice) + 90.0) * deg2rad);
		glTexCoord2f(x + 0.5, y + 0.5);
		glVertex3f(x, 0.0, y);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0.0, 0.0, 0.0);
	}
	glEnd();
}

