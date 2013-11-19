#include "Light.h"
#include "GL/glut.h"
#include "InvalidNumLights.h"

using namespace std;

int Light::index = GL_LIGHT0;

Light::Light() {
	id = "";
	enabled = false;
	ready = false;
	loc_x = 0;
	loc_y = 0;
	loc_z = 0;
	amb_r = 0;
	amb_g = 0;
	amb_b = 0;
	amb_a = 0;
	dif_r = 0;
	dif_g = 0;
	dif_b = 0;
	dif_a = 0;
	spec_r = 0;
	spec_g = 0;
	spec_b = 0;
	spec_a = 0;
	light_flag = index++;
	if (light_flag > GL_LIGHT7) {
		throw InvalidNumLights();
	}
}

Light::Light(string id) {
	this->id = id;
	enabled = false;
	ready = false;
	loc_x = 0;
	loc_y = 0;
	loc_z = 0;
	amb_r = 0;
	amb_g = 0;
	amb_b = 0;
	amb_a = 0;
	dif_r = 0;
	dif_g = 0;
	dif_b = 0;
	dif_a = 0;
	spec_r = 0;
	spec_g = 0;
	spec_b = 0;
	spec_a = 0;
	light_flag = index++;
	if (light_flag > GL_LIGHT7) {
		throw InvalidNumLights();
	}
}

Light::Light(string id, bool enabled) {
	this->id = id;
	this->enabled = enabled;
	ready = false;
	loc_x = 0;
	loc_y = 0;
	loc_z = 0;
	amb_r = 0;
	amb_g = 0;
	amb_b = 0;
	amb_a = 0;
	dif_r = 0;
	dif_g = 0;
	dif_b = 0;
	dif_a = 0;
	spec_r = 0;
	spec_g = 0;
	spec_b = 0;
	spec_a = 0;
	light_flag = index++;
	if (light_flag > GL_LIGHT7) {
		throw InvalidNumLights();
	}
}
void Light::setLocation(float loc_x, float loc_y, float loc_z) {
	this->loc_x = loc_x;
	this->loc_y = loc_y;
	this->loc_z = loc_z;
}

void Light::setAmbient(float amb_r, float amb_g, float amb_b, float amb_a) {
	this->amb_r = amb_r;
	this->amb_g = amb_g;
	this->amb_b = amb_b;
	this->amb_a = amb_a;
}

void Light::setDiffuse(float dif_r, float dif_g, float dif_b, float dif_a) {
	this->dif_r = dif_r;
	this->dif_g = dif_g;
	this->dif_b = dif_b;
	this->dif_a = dif_a;
}

void Light::setSpecular(float spec_r, float spec_g, float spec_b,
        float spec_a) {
	this->spec_r = spec_r;
	this->spec_g = spec_g;
	this->spec_b = spec_b;
	this->spec_a = spec_a;
}

void Light::readyLight() {
	float loc[4] = { loc_x, loc_y, loc_z, 1 };
	float amb[4] = { amb_r, amb_g, amb_b, amb_a };
	float dif[4] = { dif_r, dif_g, dif_b, dif_a };
	float spec[4] = { spec_r, spec_g, spec_b, spec_a };

	glPushMatrix();
	glLightfv(light_flag, GL_POSITION, loc);
	glLightfv(light_flag, GL_AMBIENT, amb);
	glLightfv(light_flag, GL_DIFFUSE, dif);
	glLightfv(light_flag, GL_SPECULAR, spec);
	glPopMatrix();

}

void Light::toggleLight() {

	if (!ready) {
		readyLight();
	}

	if (enabled) {
		glEnable(light_flag);
		enabled = false;
	} else {
		glDisable(light_flag);
		enabled = true;
	}

}

/**
 * function used in display function to maintain the lights
 */
void Light::displayLight() {

	if (!ready) {
		readyLight();
	}

	if (enabled) {
		glEnable(light_flag);
	} else {
		glDisable(light_flag);
	}
}

Light::~Light() {
}

bool Light::isEnabled() {
	return enabled;
}

string Light::getId() {
	return id;
}
