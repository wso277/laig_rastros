
#ifndef MYPRIMITIVE_H_
#define MYPRIMITIVE_H_

#include "Node.h"

/**
 * super class for all the primitives used by the program
 */
class MyPrimitive: public Node {
public:
	MyPrimitive();
	virtual void draw() = 0;
	virtual ~MyPrimitive();
	virtual void setAppearance(string appearance);
	void clearAppearance();
};

#endif /* MYPRIMITIVE_H_ */
