#ifndef MYSPHERE_H_
#define MYSPHERE_H_

#include "MyPrimitive.h"
#include "GL/glut.h"

/**
 * class used to define a cylinder
 */
class MySphere: public MyPrimitive {
	float radius;
	unsigned int slices, stacks;
	GLUquadric *q;
public:
	MySphere();
	MySphere(float radius, unsigned int slices, unsigned int stacks);
	virtual ~MySphere();
	void draw();
};

#endif /* MYSPHERE_H_ */
