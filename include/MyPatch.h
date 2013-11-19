/*
 * MyPatch.h
 *
 *  Created on: Nov 5, 2013
 *      Author: diogo
 */

#ifndef MYPATCH_H_
#define MYPATCH_H_

#include "MyPrimitive.h"
#include <vector>

class MyPatch: public MyPrimitive {
private:
	int order, partsU, partsV;
	int compute;
	vector<float> ctrlPoints;
	int nrCtrlPoints;
	vector<float> textPoints;

public:
	MyPatch();
	MyPatch(int order, int partsU, int partsV, string compute);
	virtual ~MyPatch();
	virtual void draw();
	virtual void setAppearance(string appearance);
	void addControlPoint(float x, float y, float z);
};

#endif /* MYPATCH_H_ */
