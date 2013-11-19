#include "Camera.h"

using namespace std;

Camera::Camera() {
	id = "";
	far = 0;
	near = 0;
}

Camera::Camera(string id) {

	this->id = id;
	far = 0;
	near = 0;
}

void Camera::setNear(float near) {
	this->near = near;
}

void Camera::setFar(float far) {
	this->far = far;
}

string Camera::getId() {
	return id;
}

float Camera::getNear() {
	return near;
}

float Camera::getFar() {
	return far;
}

void Camera::setCamera() {

}

Camera::~Camera() {
}
