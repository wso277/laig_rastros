#include "Ortho.h"
#include "GL/glut.h"

using namespace std;

Ortho::Ortho() :
		Camera() {
	left = 0;
	right = 0;
	top = 0;
	bottom = 0;
}

Ortho::Ortho(string id) :
		Camera(id) {
	left = 0;
	right = 0;
	top = 0;
	bottom = 0;
}

void Ortho::setLeft(float left) {
	this->left = left;
}
void Ortho::setRight(float right) {
	this->right = right;
}

void Ortho::setTop(float top) {
	this->top = top;
}

void Ortho::setBottom(float bottom) {
	this->bottom = bottom;
}

float Ortho::getLeft() {
	return left;
}

float Ortho::getRight() {
	return right;
}

float Ortho::getTop() {
	return top;
}

float Ortho::getBottom() {
	return bottom;
}

void Ortho::setCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Ortho::~Ortho() {
}
