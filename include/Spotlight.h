#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include "Light.h"

using namespace std;

/**
 * class used to define a spotlight
 */
class Spotlight: public Light {
private:
	float angle;
	float exponent;
	float dir_x, dir_y, dir_z;
public:
	Spotlight();
	Spotlight(string id);
	Spotlight(string id, bool enabled);
	void setAngle(float angle);
	void setExponent(float exponent);
	void setDir(float dir_x, float dir_y, float dir_z);
	virtual void readyLight(int index);
	virtual ~Spotlight();
};

#endif /* SPOTLIGHT_H_ */
