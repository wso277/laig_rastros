#ifndef MYCYLINDER_H_
#define MYCYLINDER_H_

#include "MyPrimitive.h"
#include "GL/glut.h"

/**
 * class used to define a cylinder
 */
class MyCylinder: public MyPrimitive {
	GLUquadric *q;
	float base, top, height;
	unsigned int slices, stacks;
public:
	MyCylinder();
	MyCylinder(float base, float top, float height, unsigned int slices,
	        unsigned int stacks);
	virtual ~MyCylinder();
	void draw();
	void drawCircle(float radius);
};

#endif /* MYCYLINDER_H_ */
