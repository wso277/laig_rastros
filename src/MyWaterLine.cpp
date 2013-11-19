#include "MyWaterLine.h"
#include <GL/glew.h>
#include "GL/gl.h"
#include <GL/glut.h>
#include "Scene.h"
#include <time.h>
#include <stdio.h>

float MyWaterLine::delta = 0;
float MyWaterLine::prev_delta = 0;
float MyWaterLine::time = 0;

MyWaterLine::MyWaterLine() :
		Plane(100) {
	delta = 0;
	prev_delta = 0;
	time = 0;
}

MyWaterLine::MyWaterLine(string heightmap, string texturemap, string vert_shader, string frag_shader) :
		Plane(100) {
	GLenum err = glewInit();
	shader.init(vert_shader.c_str(), frag_shader.c_str());
	Scene::getInstance()->addTexture("wl_height", heightmap);
	Scene::getInstance()->addTexture("wl_text", texturemap);
	height.setTextProp("wl_height", 1, 1);
	text.setTextProp("wl_text", 1, 1);
	delta = 0;
	prev_delta = 0;
	time = 0;
	glutTimerFunc(ANIMATION_TIME, updateWaterLine, 0);
}

MyWaterLine::~MyWaterLine() {

}

void MyWaterLine::setAppearance(string appearance) {
	MyPrimitive::setAppearance(appearance);
}

void MyWaterLine::draw() {
	shader.bind();
	GLint delta_loc = glGetUniformLocation(shader.id(), "delta");
	glUniform1f(delta_loc, time);

	GLint text_loc = glGetUniformLocation(shader.id(), "texturemap");
	glUniform1i(text_loc, 0);

	GLint height_loc = glGetUniformLocation(shader.id(), "heightmap");
	glUniform1i(height_loc, 1);

	GLint tex_unit = 0;

	glGetIntegerv(GL_ACTIVE_TEXTURE, &tex_unit);

	glActiveTexture(GL_TEXTURE1);
	height.apply();
	glDisable(GL_TEXTURE_2D);	//used to prevent the scene from going dark
	glActiveTexture(GL_TEXTURE0);
	text.apply();
	Plane::draw();
	shader.unbind();

	glActiveTexture(tex_unit);
}

void updateWaterLine(int i) {
	struct timespec t;
	float timer = 0;

	clock_gettime(CLOCK_MONOTONIC, &t);
	timer = t.tv_nsec * 0.000000001;

	if (timer > MyWaterLine::prev_delta) {
		MyWaterLine::delta += timer - MyWaterLine::prev_delta;
	} else {
		MyWaterLine::delta += (1 - MyWaterLine::prev_delta) + timer;
	}

	MyWaterLine::time = MyWaterLine::delta / 32.0;

	if (MyWaterLine::time >= 1) {
		MyWaterLine::delta = 0;
		MyWaterLine::time -= 1;
	}

	MyWaterLine::prev_delta = timer;

	glutTimerFunc(ANIMATION_TIME, updateWaterLine, 0);
}
