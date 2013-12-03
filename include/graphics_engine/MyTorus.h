#ifndef MYTORUS_H_
#define MYTORUS_H_

#include "MyPrimitive.h"
#include <vector>

using namespace std;

/**
 * class used to define a torus
 */
class MyTorus: public MyPrimitive {
	float inner, outer;
	unsigned int slices, loops;
	float **px, **py, *pz, ***norm, **ts, **tt;
public:
	MyTorus();
	MyTorus(float inner, float outer, unsigned int slices, unsigned int loops);
	virtual ~MyTorus();
	void draw();
	void calcPoints();
	void calcNormals();
	void calcTextCoords();
};

#endif /* MYTORUS_H_ */
