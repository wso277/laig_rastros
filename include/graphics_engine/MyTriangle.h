#ifndef MYTRIANGLE_H_
#define MYTRIANGLE_H_

#include "MyPrimitive.h"

/**
 * class used to define a triangle
 */
class MyTriangle: public MyPrimitive {
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	float normal[3];
	float text_coords[3][2];
public:
	MyTriangle();
	MyTriangle(float x1, float y1, float z1, float x2, float y2, float z2,
	        float x3, float y3, float z3);
	virtual ~MyTriangle();
	void draw();
	const float *calcNormal();
	void calcTextCoords();
	virtual void setAppearance(string appearance);
};

#endif /* MYTRIANGLE_H_ */
