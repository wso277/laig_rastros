/*
 * PointsHud.h
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#ifndef POINTSHUD_H_
#define POINTSHUD_H_

#include "MyPrimitive.h"

using namespace std;

class PointsHud : public MyPrimitive {
public:
	PointsHud();
	void draw();
	virtual ~PointsHud();
};

#endif /* POINTSHUD_H_ */
