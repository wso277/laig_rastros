/*
 * Plane.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: wso277
 */

#include "Plane.h"
#include "GL/glut.h"
#include "Scene.h"
#include <iostream>

Plane::Plane() :
		MyPrimitive() {
	setParts(1);
	initPointsVector();
}

Plane::Plane(unsigned int parts) :
		MyPrimitive() {
	setParts(parts);
	initPointsVector();
}
void Plane::draw() {
	if (!isVisible()) {
		glCullFace(GL_FRONT_AND_BACK);
	}

	float deltas = 1.0 / getAppearance()->getSWrap();
	float deltat = 1.0 / getAppearance()->getTWrap();

	text_pts.clear();
	text_pts.push_back(0);
	text_pts.push_back(deltat);

	text_pts.push_back(deltas);
	text_pts.push_back(deltat);

	text_pts.push_back(0);
	text_pts.push_back(0);

	text_pts.push_back(deltas);
	text_pts.push_back(0);

	glPushMatrix();
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2, 0.0, 1.0, 6, 2, &ctrl_pts[0]);
	glMap2f(GL_MAP2_NORMAL, 0.0, 1.0, 3, 2, 0.0, 1.0, 6, 2, &nrml_pts[0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2,
			&text_pts[0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(parts, 0.0, 1.0, parts, 0.0, 1.0);

	glEvalMesh2(Scene::getInstance()->getDrawmode(), 0.0, parts, 0.0, parts);

	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glPopMatrix();

	if (!isVisible()) {
		glCullFace(Scene::getInstance()->getCullface());
	}
}
void Plane::setAppearance(string appearance) {
	MyPrimitive::setAppearance(appearance);
}

Plane::~Plane() {
}

void Plane::setParts(unsigned int parts) {
	this->parts = parts;
}

void Plane::initPointsVector() {
	ctrl_pts.push_back(-0.5);
	ctrl_pts.push_back(0.0);
	ctrl_pts.push_back(-0.5);

	ctrl_pts.push_back(0.5);
	ctrl_pts.push_back(0.0);
	ctrl_pts.push_back(-0.5);

	ctrl_pts.push_back(-0.5);
	ctrl_pts.push_back(0.0);
	ctrl_pts.push_back(0.5);

	ctrl_pts.push_back(0.5);
	ctrl_pts.push_back(0.0);
	ctrl_pts.push_back(0.5);

	nrml_pts.push_back(0);
	nrml_pts.push_back(1);
	nrml_pts.push_back(0);

	nrml_pts.push_back(0);
	nrml_pts.push_back(1);
	nrml_pts.push_back(0);

	nrml_pts.push_back(0);
	nrml_pts.push_back(1);
	nrml_pts.push_back(0);

	nrml_pts.push_back(0);
	nrml_pts.push_back(1);
	nrml_pts.push_back(0);
}
