/*
 * TimeHud.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#include "TimeHud.h"
#include <time.h>
#include <sstream>
#include <stdio.h>
#include "GL/gl.h"
#include "text3d.h"

using namespace std;

TimeHud::TimeHud() {

	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	time = t.tv_sec;
	t3dInit();
}

void TimeHud::draw() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	int long time_passed = t.tv_sec - time;

	int long minutes = time_passed / 60;
	int long seconds = time_passed % 60;

	stringstream ss;
	ss.str("");
	ss << minutes << ":" << seconds;

	glPushMatrix();
	glTranslatef(500, 15, 0);
	glScalef(20,20,20);
	glRotatef(180, 1,0,0);
	t3dDraw2D(ss.str(), 0, 0, 0.2f);
	glPopMatrix();

}

TimeHud::~TimeHud() {
	// TODO Auto-generated destructor stub
}

