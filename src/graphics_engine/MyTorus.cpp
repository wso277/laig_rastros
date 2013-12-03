#include "MyTorus.h"
#include "GL/glut.h"
#include <math.h>
#include "utils.h"
#include <vector>

extern float deg2rad;

MyTorus::MyTorus() :
		MyPrimitive() {
	this->inner = 0.5;
	this->outer = 1.5;
	this->slices = 10;
	this->loops = 10;

	this->px = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		px[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	this->py = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		py[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	pz = (float *) malloc(sizeof(float) * (loops + 1));

	this->norm = (float ***) malloc(sizeof(float **) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		norm[i] = (float **) malloc(sizeof(float *) * (slices + 1));
	}

	this->ts = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		ts[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	this->tt = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		tt[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	calcPoints();
	calcNormals();
	calcTextCoords();

}

MyTorus::MyTorus(float inner, float outer, unsigned int slices,
		unsigned int loops) :
		MyPrimitive() {
	this->inner = inner;
	this->outer = outer;
	this->slices = slices;
	this->loops = loops;

	this->px = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		px[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	this->py = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		py[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	pz = (float *) malloc(sizeof(float) * (loops + 1));

	this->norm = (float ***) malloc(sizeof(float **) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		norm[i] = (float **) malloc(sizeof(float *) * (slices + 1));
	}

	this->ts = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		ts[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	this->tt = (float **) malloc(sizeof(float *) * (loops + 1));
	for (unsigned int i = 0; i <= loops; i++) {
		tt[i] = (float *) malloc(sizeof(float) * (slices + 1));
	}

	calcPoints();
	calcNormals();
	calcTextCoords();
}

MyTorus::~MyTorus() {
}

void MyTorus::draw() {
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	for (unsigned int i = 0; i < loops; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(norm[i][0][0], norm[i][0][1], norm[i][0][2]);
		glTexCoord2f(ts[i][0], tt[i][0]);
		glVertex3f(px[i][0], py[i][0], pz[i]);
		for (unsigned int j = 0; j < slices; j++) {
			glNormal3f(norm[i + 1][j][0], norm[i + 1][j][1], norm[i + 1][j][2]);
			glTexCoord2f(ts[i + 1][j], tt[i + 1][j]);
			glVertex3f(px[i + 1][j], py[i + 1][j], pz[i + 1]);
			glNormal3f(norm[i][j + 1][0], norm[i][j + 1][1], norm[i][j + 1][2]);
			glTexCoord2f(ts[i][j + 1], tt[i][j + 1]);
			glVertex3f(px[i][j + 1], py[i][j + 1], pz[i]);
		}
		glNormal3f(norm[i + 1][slices][0], norm[i + 1][slices][1],
				norm[i + 1][slices][2]);
		glVertex3f(px[i + 1][slices], py[i + 1][slices], pz[i + 1]);
		glEnd();
	}
	glEnd();
	glPopMatrix();
}

void MyTorus::calcPoints() {
	float phi_step = 360.0 / loops;
	float teta_step = 360.0 / slices;

	float main_radius = (inner + outer) / 2.0;
	float sec_radius = (outer - inner) / 2.0;

	float phi = -90.0;
	for (unsigned int i = 0; i <= loops; i++) {
		float teta = 0.0;
		pz[i] = sec_radius * cosf(phi * deg2rad);
		for (unsigned int j = 0; j <= slices; j++) {
			px[i][j] = (main_radius + sec_radius * sinf(phi * deg2rad))
					* cosf(teta * deg2rad);
			py[i][j] = (main_radius + sec_radius * sinf(phi * deg2rad))
					* sinf(teta * deg2rad);
			teta += teta_step;
		}
		phi += phi_step;
	}
}

void MyTorus::calcNormals() {
	for (unsigned int i = 0; i <= loops; i++) {
		for (unsigned int j = 0; j <= slices; j++) {
			int c_i = i;
			int n_i = i + 1;
			if (n_i > loops)
				n_i = 1;
			int b_i = i - 1;
			if (b_i < 0)
				b_i = loops - 1;
			int c_j = j;
			int n_j = j + 1;
			if (n_j > loops)
				n_j = 1;
			int b_j = j - 1;
			if (b_j < 0)
				b_j = slices - 1;

			vector<float> x, y, z;
			x.push_back(px[c_i][c_j]);
			x.push_back(px[n_i][c_j]);
			x.push_back(px[c_i][n_j]);

			y.push_back(py[c_i][c_j]);
			y.push_back(py[n_i][c_j]);
			y.push_back(py[c_i][n_j]);

			z.push_back(pz[c_i]);
			z.push_back(pz[n_i]);
			z.push_back(pz[c_i]);

			norm[i][j] = getNewellsMethod(x, y, z);

		}
	}
}

void MyTorus::calcTextCoords() {
	for (unsigned int i = 0; i <= loops; i++) {
		for (unsigned int j = 0; j <= slices; j++) {
			float tmp = (float) j * 2.0 / (float) slices;
			if (tmp > 1) {
				tmp = 2 - tmp;
			}
			ts[i][j] = tmp;
			tt[i][j] = (float) i / (float) loops;
		}
	}
}

