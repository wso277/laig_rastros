/*
 * MyVehicle.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: wso277
 */

#include "MyVehicle.h"
#include "GL/gl.h"

MyVehicle::MyVehicle() :
		MyPrimitive() {
	center = new MySphere(0.5, 20, 20);

	top = new MyPatch(3, 30, 30, "fill");

	top->addControlPoint(-1.0, 0.2, 1);
	top->addControlPoint(-0.5, 0.2, 1);
	top->addControlPoint(0.5, 0.2, 1);
	top->addControlPoint(1.0, 0.2, 1);

	top->addControlPoint(-1.0, 1, 0.5);
	top->addControlPoint(-0.5, 1, 0.5);
	top->addControlPoint(0.5, 1, 0.5);
	top->addControlPoint(1.0, 1, 0.5);

	top->addControlPoint(-1.0, 1, -0.5);
	top->addControlPoint(-0.5, 1, -0.5);
	top->addControlPoint(0.5, 1, -0.5);
	top->addControlPoint(1.0, 1, -0.5);

	top->addControlPoint(-1.0, 0.2, -1.0);
	top->addControlPoint(-0.5, 0.2, -1.0);
	top->addControlPoint(0.5, 0.2, -1.0);
	top->addControlPoint(1.0, 0.2, -1.0);

}

MyVehicle::~MyVehicle() {
}

void MyVehicle::draw() {

	glPushMatrix();
	glScalef(2, 2, 2);
	center->draw();
	top->draw();

	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	top->draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 0.5);
	glScalef(1, 0.5, 1);
	glRotatef(90, 1, 0, 0);
	top->draw();
	glPopMatrix();

	glPopMatrix();
}

void MyVehicle::setAppearance(string appearance) {
	MyPrimitive::setAppearance(appearance);
}
