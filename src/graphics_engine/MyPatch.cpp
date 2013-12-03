/*
 * MyPatch.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: diogo
 */

#include <MyPatch.h>
#include <GL/gl.h>
#include <math.h>
#include "InvalidPreAttrException.h"
#include "Scene.h"
#include <iostream>
#include <stdio.h>

MyPatch::MyPatch() :
		MyPrimitive() {
	this->order = 1;
	this->partsU = 1;
	this->partsV = 1;
	this->compute = GL_FILL;
	//int nr = pow(order + 1, 2);
	//ctrlPoints = new float[nr];
	nrCtrlPoints = 0;

	textPoints.push_back(1.0);
	textPoints.push_back(1.0);

	textPoints.push_back(0.0);
	textPoints.push_back(1.0);

	textPoints.push_back(1.0);
	textPoints.push_back(0.0);

	textPoints.push_back(0.0);
	textPoints.push_back(0.0);
}

MyPatch::~MyPatch() {
}

MyPatch::MyPatch(int order, int partsU, int partsV, string compute) :
		MyPrimitive() {

	if (order < 1 || order > 3) {
		throw InvalidPreAttrException("order");
	}

	this->order = order;
	this->partsU = partsU;
	this->partsV = partsV;

	if (compute == "fill") {
		this->compute = GL_FILL;
	} else if (compute == "line") {
		this->compute = GL_LINE;
	} else if (compute == "point") {
		this->compute = GL_POINT;
	} else {
		throw InvalidPreAttrException("compute");
	}

	//int nr = pow(order + 1, 2);
	//ctrlPoints = new float[nr];
	nrCtrlPoints = 0;

	textPoints.push_back(1.0);
	textPoints.push_back(1.0);

	textPoints.push_back(0.0);
	textPoints.push_back(1.0);

	textPoints.push_back(1.0);
	textPoints.push_back(0.0);

	textPoints.push_back(0.0);
	textPoints.push_back(0.0);
}

void MyPatch::addControlPoint(float x, float y, float z) {
	if (nrCtrlPoints < pow(order + 1, 2)) {
		ctrlPoints.push_back(x);
		ctrlPoints.push_back(y);
		ctrlPoints.push_back(z);
		nrCtrlPoints++;
	}
}

void MyPatch::draw() {
	glPushMatrix();
	if (nrCtrlPoints == pow(order + 1, 2)) {
		if (Scene::getInstance()->getCullorder() == GL_CCW) {
			glFrontFace(GL_CW);
		}

		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order + 1, 0.0, 1.0, (order + 1) * 3, order + 1, &ctrlPoints[0]);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &textPoints[0]);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);

		glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);

		glEvalMesh2(compute, 0.0, partsU, 0.0, partsV);

		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_AUTO_NORMAL);
		glDisable(GL_MAP2_TEXTURE_COORD_2);

		glFrontFace(Scene::getInstance()->getCullorder());

	}
	glPopMatrix();
}

void MyPatch::setAppearance(string appearance) {
	MyPrimitive::setAppearance(appearance);
}
