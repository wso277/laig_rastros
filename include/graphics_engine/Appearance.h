#ifndef APPEARANCE_H_
#define APPEARANCE_H_

#include <string>

using namespace std;

/**
 * class used to define objects appearance
 */
class Appearance {
protected:
	string id;
	float emi_r, emi_g, emi_b, emi_a;
	float amb_r, amb_g, amb_b, amb_a;
	float dif_r, dif_g, dif_b, dif_a;
	float spec_r, spec_g, spec_b, spec_a;
	float shin;
	string textRef;
	float sWrap, tWrap;
	bool isTextDefined;

public:
	Appearance();
	Appearance(string id);
	void setId(string id);
	void setEmissivity(float emi_r, float emi_g, float emi_b, float emi_a);
	void setAmbient(float amb_r, float amb_g, float amb_b, float amb_a);
	void setDiffuse(float dif_r, float dif_g, float dif_b, float dif_a);
	void setSpecular(float spec_r, float spec_g, float spec_b, float spec_a);
	void setShinniness(float shin);
	void setTextProp(string text_id, float s_wrap, float t_wrap);
	float getSWrap();
	float getTWrap();
	void applyEmissive();
	virtual ~Appearance();
	string getId();

	void apply();
};

#endif /* APPEARANCE_H_ */
