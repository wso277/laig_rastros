/*
 * Plane.h
 *
 *  Created on: Nov 5, 2013
 *      Author: wso277
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "MyPrimitive.h"
#include <vector>

class Plane: public MyPrimitive {
	unsigned int parts;
	vector<float> ctrl_pts;
	vector<float> nrml_pts;
	vector<float> text_pts;
public:
	Plane();
	Plane(unsigned int parts);
	virtual void draw();
	virtual void setAppearance(string appearance);
	virtual ~Plane();
};

#endif /* PLANE_H_ */
