#ifndef MYRECTANGLE_H_
#define MYRECTANGLE_H_

#include "MyPrimitive.h"

/**
 * class used to define a rectangle
 */
class MyRectangle: public MyPrimitive {
private:
	float x1, y1, x2, y2;
	float normal[3];
	float text_coords[4][2];
public:
	MyRectangle();
	MyRectangle(float x1, float y1, float x2, float y2);
	void draw();
	virtual ~MyRectangle();
	const float *calcNormal();
	void calcTextCoords();
	virtual void setAppearance(string appearance);
};

#endif /* MYRECTANGLE_H_ */
