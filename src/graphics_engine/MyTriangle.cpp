#include "MyTriangle.h"
#include "GL/glut.h"
#include <math.h>
#include "Scene.h"

MyTriangle::MyTriangle() :
		MyPrimitive() {
	this->x1 = -0.5;
	this->y1 = -0.5;
	this->z1 = 0.0;
	this->x2 = 0.5;
	this->y2 = -0.5;
	this->z2 = 0.0;
	this->x3 = 0.0;
	this->y3 = 0.5;
	this->z3 = 0.0;

	calcNormal();
}

MyTriangle::MyTriangle(float x1, float y1, float z1, float x2, float y2,
        float z2, float x3, float y3, float z3) :
		MyPrimitive() {
	this->x1 = x1;
	this->y1 = y1;
	this->z1 = z1;
	this->x2 = x2;
	this->y2 = y2;
	this->z2 = z2;
	this->x3 = x3;
	this->y3 = y3;
	this->z3 = z3;

	calcNormal();
}

void MyTriangle::setAppearance(string appearance) {
	MyPrimitive::setAppearance(appearance);
	calcTextCoords();
}

MyTriangle::~MyTriangle() {
}

void MyTriangle::draw() {
	glNormal3f(normal[0], normal[1], normal[2]);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(text_coords[0][0], text_coords[0][1]);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(text_coords[1][0], text_coords[1][1]);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(text_coords[2][0], text_coords[2][1]);
	glVertex3f(x3, y3, z3);
	glEnd();
}

const float *MyTriangle::calcNormal() {
	if (Scene::getInstance()->getCullorder() == GL_CCW) {
		normal[0] = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
		normal[1] = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
		normal[2] = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	} else {
		normal[0] = (y3 - y1) * (z2 - z1) - (z3 - z1) * (y2 - y1);
		normal[1] = (z3 - z1) * (x2 - x1) - (x3 - x1) * (z2 - z1);
		normal[2] = (x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1);
	}

	return normal;
}

void MyTriangle::calcTextCoords() {

	float base1 = sqrtf(exp2(x2 - x1) + exp2(y2 - y1) + exp2(z2 - z1));
	float hipot = sqrtf(exp2(x3 - x1) + exp2(y3 - y1) + exp2(z3 - z1));
	float teta = acos((x1 * x2 + y1 * y2 + z1 * z2) / (base1 * hipot));
	float base2 = hipot * cos(teta);
	float hight = sqrt(exp2(hipot) - exp2f(base2));
	float deltas1 = base1 / getAppearance()->getSWrap();
	float deltas2 = base2 / getAppearance()->getSWrap();
	float deltat = hight / getAppearance()->getTWrap();

	text_coords[0][0] = 0.0;
	text_coords[0][1] = 0.0;
	text_coords[1][0] = deltas1;
	text_coords[1][1] = 0.0;
	text_coords[2][0] = deltas2;
	text_coords[2][1] = deltat;
}
