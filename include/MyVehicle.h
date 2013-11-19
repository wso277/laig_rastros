/*
 * MyVehicle.h
 *
 *  Created on: Nov 6, 2013
 *      Author: wso277
 */

#ifndef MYVEHICLE_H_
#define MYVEHICLE_H_

#include "MyPrimitive.h"
#include "MyPatch.h"
#include "MySphere.h"

class MyVehicle: public MyPrimitive {
	vector<float> ctrl_pts;
	MySphere *center;
	MyPatch *top;
public:
	MyVehicle();
	virtual ~MyVehicle();
	void draw();
	void setAppearance(string appearance);
};

#endif /* MYVEHICLE_H_ */
