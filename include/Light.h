#ifndef LIGHT_H_
#define LIGHT_H_

#include <string>

using namespace std;

/**
 * class used to define an opengl light
 */
class Light {
protected:
	string id;
	bool ready;
	bool enabled;
	float loc_x, loc_y, loc_z;
	float amb_r, amb_g, amb_b, amb_a;
	float dif_r, dif_g, dif_b, dif_a;
	float spec_r, spec_g, spec_b, spec_a;
	static int index;
	int light_flag;
public:
	Light();
	Light(string id);
	Light(string id, bool enabled);
	Light(string id, bool enabled, int index);
	void setLocation(float loc_x, float loc_y, float loc_z);
	void setAmbient(float amb_r, float amb_g, float amb_b, float amb_a);
	void setDiffuse(float dif_r, float dif_g, float dif_b, float dif_a);
	void setSpecular(float spec_r, float spec_g, float spec_b, float spec_a);
	virtual void readyLight();
	void toggleLight();
	void displayLight();
	virtual ~Light();
	bool isEnabled();
	string getId();
	void setIndex(int i);
	int getIndex();
};

#endif /* LIGHT_H_ */
