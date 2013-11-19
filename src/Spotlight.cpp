
#include "Spotlight.h"
#include "GL/glut.h"

Spotlight::Spotlight():Light() {
	angle = 0;
	exponent = 0;
	dir_x = 0;
	dir_y = 0;
	dir_z = 0;
}

Spotlight::Spotlight(string id):Light(id) {
	angle = 0;
	exponent = 0;
	dir_x = 0;
	dir_y = 0;
	dir_z = 0;
}

Spotlight::Spotlight(string id, bool enabled):Light(id, enabled) {
	angle = 0;
	exponent = 0;
	dir_x = 0;
	dir_y = 0;
	dir_z = 0;
}

void Spotlight::setAngle(float angle) {
	this->angle = angle;
}

void Spotlight::setExponent(float exponent) {
	this->exponent = exponent;
}

void Spotlight::setDir(float dir_x, float dir_y, float dir_z) {
	this->dir_x = dir_x;
	this->dir_y = dir_y;
	this->dir_z = dir_z;
}

void Spotlight::readyLight(int index) {
	float loc[4] = { loc_x, loc_y, loc_z, 1 };
	float amb[4] = { amb_r, amb_g, amb_b, amb_a };
	float dif[4] = { dif_r, dif_g, dif_b, dif_a };
	float spec[4] = { spec_r, spec_g, spec_b, spec_a };
	float dir[3] = { dir_x, dir_y, dir_z};
	switch (index) {
	case 0:
		glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, loc);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 1:
		glPushMatrix();
		glLightfv(GL_LIGHT1, GL_POSITION, loc);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 2:
		glPushMatrix();
		glLightfv(GL_LIGHT2, GL_POSITION, loc);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT2, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT2, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 3:
		glPushMatrix();
		glLightfv(GL_LIGHT3, GL_POSITION, loc);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT3, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT3, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 4:
		glPushMatrix();
		glLightfv(GL_LIGHT4, GL_POSITION, loc);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT4, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT4, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 5:
		glPushMatrix();
		glLightfv(GL_LIGHT5, GL_POSITION, loc);
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT5, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT5, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 6:
		glPushMatrix();
		glLightfv(GL_LIGHT6, GL_POSITION, loc);
		glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT6, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT6, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT6, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	case 7:
		glPushMatrix();
		glLightfv(GL_LIGHT7, GL_POSITION, loc);
		glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, dir);
		glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, angle);
		glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, exponent);
		glLightfv(GL_LIGHT7, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT7, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT7, GL_SPECULAR, spec);
		glPopMatrix();
		break;
	}

}

Spotlight::~Spotlight() {
}

