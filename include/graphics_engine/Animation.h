/*
 * Animation.h
 *
 *  Created on: Oct 22, 2013
 *      Author: wso277
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <string>
#include <vector>
#include "Point.h"
#include <time.h>

using namespace std;

class Animation {
private:
	string id;
	float span;
	string type;
	vector<Point*> points;
	vector<Point*> delta;
	vector<float> time_exp;
	vector<Point*> direction;
	vector<float> rotations;
	int vec_index;
	float time_passed;
	Point point;
	float time_last;
	float time_line;

public:
	Animation();
	Animation(string id, float span, string type);
	void addPoint(float x, float y, float z);
	vector<Point*> getPoints();
	float getSpan();
	Point* getDelta();
	void calculateDelta();
	int getTime();
	float updateValues();
	float getRotation();
	Point getPoint();
	virtual ~Animation();
};

#endif /* ANIMATION_H_ */
