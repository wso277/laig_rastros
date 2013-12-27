/*
 * VictoryHud.h
 *
 *  Created on: Dec 27, 2013
 *      Author: knoweat
 */

#ifndef VICTORYHUD_H_
#define VICTORYHUD_H_

#include "MyPrimitive.h"

class VictoryHud: public MyPrimitive {
	int winner;
public:
	VictoryHud(int Winner);
	virtual ~VictoryHud();
	void draw();
};

#endif /* VICTORYHUD_H_ */
