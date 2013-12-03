/*
 * MyParallelepiped.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef MYPARALLELEPIPED_H_
#define MYPARALLELEPIPED_H_

#include <MyPrimitive.h>
#include "Appearance.h"
#include "Plane.h"

class MyParallelepiped: public MyPrimitive {
	string topAppearance;
	string sidesAppearance;
	string bottomAppearance;
	Plane top;
	Plane side;
	float x_dim;
	float y_dim;
	float z_dim;
public:
	MyParallelepiped();
	MyParallelepiped(float x_dim, float y_dim, float z_dim);
	virtual ~MyParallelepiped();
	void setDims(float x_dim, float y_dim, float z_dim);
	void setAppearances(string topAppearance, string bottomAppearance,
			string sidesAppearance);
	void draw();
};

#endif /* MYPARALLELEPIPED_H_ */
