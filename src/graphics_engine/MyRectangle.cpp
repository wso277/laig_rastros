#include "MyRectangle.h"
#include "GL/glut.h"
#include "Scene.h"

MyRectangle::MyRectangle() :
		MyPrimitive() {
	x1 = -0.5;
	y1 = -0.5;
	x2 = 0.5;
	y2 = 0.5;

	calcNormal();
}

MyRectangle::MyRectangle(float x1, float y1, float x2, float y2) :
		MyPrimitive() {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;

	calcNormal();
}

void MyRectangle::setAppearance(string appearance) {
	MyPrimitive::setAppearance(appearance);
	calcTextCoords();
}

MyRectangle::~MyRectangle() {
}

void MyRectangle::draw() {
	glBegin(GL_QUADS);
	glNormal3f(normal[0], normal[1], normal[2]);
	glTexCoord2f(text_coords[0][0], text_coords[0][1]);
	glVertex3f(x1, y1, 0.0);
	glTexCoord2f(text_coords[1][0], text_coords[1][1]);
	glVertex3f(x2, y1, 0.0);
	glTexCoord2f(text_coords[2][0], text_coords[2][1]);
	glVertex3f(x2, y2, 0.0);
	glTexCoord2f(text_coords[3][0], text_coords[3][1]);
	glVertex3f(x1, y2, 0.0);
	glEnd();
}

const float *MyRectangle::calcNormal() {
	if (Scene::getInstance()->getCullorder() == GL_CCW) {
		normal[0] = 0;
		normal[1] = 0;
		normal[2] = (x2 - x1) * (y2 - y1) - (y1 - y1) * (x2 - x1);
	} else {
		normal[0] = 0;
		normal[1] = 0;
		normal[2] = (x2 - x1) * (y1 - y1) - (y2 - y1) * (x2 - x1);
	}

	return normal;
}

void MyRectangle::calcTextCoords() {
	float deltax, deltay, deltas, deltat;

	if (x2 > x1)
		deltax = x2 - x1;
	else
		deltax = x1 - x2;

	if (y2 > y1)
		deltay = y2 - y1;
	else
		deltay = y1 - y2;

	deltas = deltax / getAppearance()->getSWrap();
	deltat = deltay / getAppearance()->getTWrap();

	if (Scene::getInstance()->getCullorder() == GL_CCW) {
		text_coords[0][0] = 0.0;
		text_coords[0][1] = 0.0;
		text_coords[1][0] = deltas;
		text_coords[1][1] = 0.0;
		text_coords[2][0] = deltas;
		text_coords[2][1] = deltat;
		text_coords[3][0] = 0.0;
		text_coords[3][1] = deltat;
	} else {
		text_coords[0][0] = 0.0;
		text_coords[0][1] = 0.0;
		text_coords[1][0] = 0.0;
		text_coords[1][1] = deltat;
		text_coords[2][0] = deltas;
		text_coords[2][1] = deltat;
		text_coords[3][0] = deltas;
		text_coords[3][1] = 0.0;
	}

}
