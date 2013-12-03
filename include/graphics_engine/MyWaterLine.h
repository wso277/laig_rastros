/*
 * MyWaterLine.h
 *
 *  Created on: Nov 6, 2013
 *      Author: ei11120
 */

#ifndef MYWATERLINE_H_
#define MYWATERLINE_H_

#include "CGFshader.h"
#include <string>
#include "Appearance.h"
#include "Plane.h"

using namespace std;

void updateWaterLine(int i);

class MyWaterLine: public Plane {
	CGFshader shader;
	static float prev_delta;
	static float delta;
	static float time;
	Appearance height, text;
public:
	MyWaterLine();
	MyWaterLine(string heightmap, string texturemap, string vert_shader, string frag_shader);
	virtual ~MyWaterLine();
	virtual void setAppearance(string appearance);
	virtual void draw();
	friend void updateWaterLine(int i);
};

#endif /* MYWATERLINE_H_ */
