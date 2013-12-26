/*
 * TimeHud.h
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#ifndef TIMEHUD_H_
#define TIMEHUD_H_

#include "MyPrimitive.h"

using namespace std;

class TimeHud: public MyPrimitive {
private:
	int long time;
public:
	TimeHud();
	void draw();
	virtual ~TimeHud();
};

#endif /* TIMEHUD_H_ */
