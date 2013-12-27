/*
 * PointsHud.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#include "PointsHud.h"
#include "text3d.h"
#include <GL/gl.h>
#include <sstream>
#include "GameLogic.h"
#include <stdio.h>

using namespace std;

PointsHud::PointsHud() {
	t3dInit();
}

void PointsHud::draw() {
	glPushMatrix();
	glTranslatef(70, 15, 0);
	glScalef(20,20,20);
	glRotatef(180, 1,0,0);
	t3dDraw2D(GameLogic::getInstance()->getPlayer1Name(), 0, 0, 0.2f);
	glPopMatrix();

	stringstream ss;
	ss.str("");
	ss << GameLogic::getInstance()->getPlayer1();

	glPushMatrix();
	glTranslatef(70, 45, 0);
	glScalef(20,20,20);
	glRotatef(180, 1,0,0);
	t3dDraw2D(ss.str(), 0, 0, 0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(950, 15, 0);
	glScalef(20,20,20);
	glRotatef(180, 1,0,0);
	t3dDraw2D(GameLogic::getInstance()->getPlayer2Name(), 0, 0, 0.2f);
	glPopMatrix();

	ss.str("");
	ss << GameLogic::getInstance()->getPlayer2();

	glPushMatrix();
	glTranslatef(950, 45, 0);
	glScalef(20,20,20);
	glRotatef(180, 1,0,0);
	t3dDraw2D(ss.str(), 0, 0, 0.2f);
	glPopMatrix();
}

PointsHud::~PointsHud() {
	// TODO Auto-generated destructor stub
}

