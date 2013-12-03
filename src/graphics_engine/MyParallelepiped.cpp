/*
 * MyParallelepiped.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include <MyParallelepiped.h>
#include <GL/gl.h>
#include "Scene.h"

MyParallelepiped::MyParallelepiped() {
	x_dim = 1;
	y_dim = 1;
	z_dim = 1;
	topAppearance = "default";
	sidesAppearance = "default";
	topAppearance = "default";
}

MyParallelepiped::MyParallelepiped(float x_dim, float y_dim, float z_dim) :
		x_dim(x_dim), y_dim(y_dim), z_dim(z_dim) {
}

MyParallelepiped::~MyParallelepiped() {
}

void MyParallelepiped::setAppearances(string topAppearance,
		string bottomAppearance, string sidesAppearance) {
	this->topAppearance = topAppearance;
	this->bottomAppearance = bottomAppearance;
	this->sidesAppearance = sidesAppearance;
}

void MyParallelepiped::setDims(float x_dim, float y_dim, float z_dim) {
	this->x_dim = x_dim;
	this->y_dim = y_dim;
	this->z_dim = z_dim;
}

void MyParallelepiped::draw() {
	// top
	glPushMatrix();
	glScalef(x_dim, y_dim, z_dim);
	Scene::getInstance()->getAppearance(topAppearance)->apply();
	top.draw();
	glPopMatrix();

	// bottom
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, y_dim, 0);
	glScalef(x_dim, 0, z_dim);
	Scene::getInstance()->getAppearance(bottomAppearance)->apply();
	top.draw();
	glPopMatrix();

	//sides
	glPushMatrix();
	glTranslatef(0, 0, z_dim / 2.0);
	glRotatef(90, 1, 0, 0);
	glScalef(x_dim, 0, y_dim);
	side.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -(z_dim / 2.0));
	glRotatef(-90, 1, 0, 0);
	glScalef(x_dim, 0, y_dim);
	side.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x_dim / 2.0, 0, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	glScalef(x_dim, 0, y_dim);
	side.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-(x_dim / 2.0), 0, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(x_dim, 0, y_dim);
	side.draw();
	glPopMatrix();
}


