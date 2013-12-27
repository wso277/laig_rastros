#include "Interface.h"
#include "Scene.h"
#include <limits>
#include <iostream>
#include <unistd.h>
#include "GameLogic.h"

#define DEFAULT_GRAPH_DEPTH 100

extern int main_window;

using namespace std;

bool inSelectMode = false;

extern float obj_pos[3];

GLUI_Panel *Buttons;

int skybox_selected = 0;
int GAME_MODE = 0;
int DIFFICULTY_LEVEL = 0;

int Interface::modifiers = 0;
map<string, int*> Interface::cams_rb;
map<string, int*> Interface::lights_cb;
int *Interface::cams_vars = new int[MAX_CAMS];
int Interface::id_counter = 0;
int Interface::radio_id = 0;
GLUI_RadioGroup *Interface::cams_group = NULL;
int Interface::drawmd_id = 0;
GLUI_RadioGroup *Interface::drawmd_grp = NULL;
int Interface::difficulty_id = 0;
GLUI_RadioGroup *Interface::difficulty_grp = NULL;
int *Interface::drawmode_vars = new int[3];
int *Interface::difficulty_vars = new int[5];
float Interface::displacementX = 0;
float Interface::displacementY = 0;
bool Interface::pressing_left = 0;
bool Interface::pressing_middle = 0;
bool Interface::pressing_right = 0;
float Interface::prev_X = 0;
float Interface::prev_Y = 0;
GLuint *Interface::selectBuf = NULL;
int Interface::selectBufSize = 0;

Interface::Interface() {
}
Interface::~Interface() {
}

void Interface::init(int parent) {
	glui_window = GLUI_Master.create_glui_subwindow(parent,
	GLUI_SUBWINDOW_BOTTOM);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutIdleFunc(idle);
	glui_window->set_main_gfx_window(main_window);
	GLUI_Master.set_glutKeyboardFunc(Interface::processKeyboard);

	GLUI_Master.set_glutMouseFunc(Interface::processMouse);
	glutMotionFunc(Interface::processMouseMoved);
	glutPassiveMotionFunc(Interface::processPassiveMouseMoved);
	displacementX = 0;
	displacementY = 0;

	pressing_left = false;
	pressing_right = false;
	pressing_middle = false;

	prev_X = 0;
	prev_Y = 0;

	selectBufSize = (DEFAULT_GRAPH_DEPTH + 3)
			* Scene::getInstance()->getSceneSize();
	Interface::selectBuf = new GLuint[selectBufSize];
}

void Interface::initGUI() {

	// create cameras panel
	CameraElem::iterator cam_it;
	int i = 0;
	int init_camera_pos = 0;
	for (cam_it = Scene::getInstance()->cameras.begin();
			cam_it != Scene::getInstance()->cameras.end(); cam_it++) {
		int live_var = 0;
		int id = id_counter++;
		if (cam_it->first == Scene::getInstance()->getInitCamera()) {
			live_var = -1;
			init_camera_pos = i;
		}
		int* ptr = new int[2];
		ptr[0] = live_var;
		ptr[1] = i;
		cams_vars[i++] = live_var;
		cams_rb.insert(map<string, int*>::value_type(cam_it->first, ptr));
	}

	GLUI_Panel *camsPanel = glui_window->add_panel("Cameras");
	radio_id = id_counter++;
	cams_group = glui_window->add_radiogroup_to_panel(camsPanel, cams_vars,
			radio_id, Interface::processGUI);

	map<string, int*>::iterator cb_it;
	for (cb_it = cams_rb.begin(); cb_it != cams_rb.end(); cb_it++) {
		glui_window->add_radiobutton_to_group(cams_group, cb_it->first.c_str());
	}

	cams_group->set_int_val(init_camera_pos);

	GLUI_Translation *trans_x = glui_window->add_translation("Animation",
	GLUI_TRANSLATION_X, &obj_pos[0]);
	trans_x->set_speed(.02);

	glui_window->add_column(true);

	// create lights panel
	vector<Light *>::iterator light_it;
	for (light_it = Scene::getInstance()->lights.begin();
			light_it != Scene::getInstance()->lights.end(); light_it++) {
		int live_var = 0;
		int id = id_counter++;
		if ((*light_it)->isEnabled())
			live_var = -1;

		int* ptrL = new int[2];
		ptrL[0] = live_var;
		ptrL[1] = id;
		lights_cb.insert(
				map<string, int*>::value_type((*light_it)->getId(), ptrL));
	}

	GLUI_Panel *lightsPanel = glui_window->add_panel("Lights");

	map<string, int*>::iterator rb_it;
	for (rb_it = lights_cb.begin(); rb_it != lights_cb.end(); rb_it++) {
		glui_window->add_checkbox_to_panel(lightsPanel, rb_it->first.c_str(),
				&rb_it->second[0], rb_it->second[1], Interface::processGUI);
	}

	glui_window->add_column(true);

	// create drawmode panel

	switch (Scene::getInstance()->getDrawmode()) {
	case GL_FILL:
		drawmode_vars[0] = 1;
		drawmode_vars[1] = 0;
		drawmode_vars[2] = 0;
		break;
	case GL_LINE:
		drawmode_vars[0] = 0;
		drawmode_vars[1] = 1;
		drawmode_vars[2] = 0;
		break;
	case GL_POINT:
		drawmode_vars[0] = 0;
		drawmode_vars[1] = 0;
		drawmode_vars[2] = 1;
		break;
	}

	GLUI_Panel *drawmodePanel = glui_window->add_panel("Drawmode");
	drawmd_id = id_counter++;
	drawmd_grp = glui_window->add_radiogroup_to_panel(drawmodePanel,
			drawmode_vars, drawmd_id, Interface::processGUI);
	glui_window->add_radiobutton_to_group(drawmd_grp, "Fill");
	glui_window->add_radiobutton_to_group(drawmd_grp, "Wireframe");
	glui_window->add_radiobutton_to_group(drawmd_grp, "Points");
	drawmd_grp->set_int_val(0);

	glui_window->add_column(true);

	GLUI_Panel *difficultyPanel = glui_window->add_panel("Difficulty Level");
	difficulty_grp = glui_window->add_radiogroup_to_panel(difficultyPanel,
			&DIFFICULTY_LEVEL, 1, radiogroup_handler);
	glui_window->add_radiobutton_to_group(difficulty_grp, "Easy");
	glui_window->add_radiobutton_to_group(difficulty_grp, "Medium");
	glui_window->add_radiobutton_to_group(difficulty_grp, "Hard");
	glui_window->add_radiobutton_to_group(difficulty_grp, "Extreme");

	glui_window->add_column(true);

	Buttons = glui_window->add_panel("Game Flow Controls");
	glui_window->add_button_to_panel(Buttons, "Undo", 1, button_handler);
	glui_window->add_button_to_panel(Buttons, "Repeat", 2, button_handler);

	glui_window->add_column(true);

	GLUI_Panel *Menu = glui_window->add_panel("Menu");
	glui_window->add_button_to_panel(Menu, "New Game", 3, button_handler);

	glui_window->add_column(true);

	GLUI_Panel *modePanel = glui_window->add_panel("Game Mode");
	GLUI_RadioGroup *mode_grp = glui_window->add_radiogroup_to_panel(modePanel,
			&GAME_MODE, 2, radiogroup_handler);
	glui_window->add_radiobutton_to_group(mode_grp, "Human vs Human");
	glui_window->add_radiobutton_to_group(mode_grp, "Human vs Computer");
	glui_window->add_radiobutton_to_group(mode_grp, "Computer vs Human");
	glui_window->add_radiobutton_to_group(mode_grp, "Computer vs Computer");

	glui_window->add_column(true);

	GLUI_Panel *Skybox = glui_window->add_panel("Skybox");
	GLUI_Listbox *sky_list = glui_window->add_listbox_to_panel(Skybox, "Skybox", &skybox_selected, 1, skybox_handler);
	for (int i = 0; i < Scene::getInstance()->getSkyboxes().size(); i++) {
		sky_list->add_item(i, Scene::getInstance()->getSkyboxes()[i]->getId().c_str());
	}

	glui_window->add_column(true);

}

void radiogroup_handler(int id) {

}

void skybox_handler(int id) {
	Scene::getInstance()->setSkyboxNum(skybox_selected);
}

void button_handler(int id) {
	switch (id) {
	case 1:
		GameLogic::getInstance()->undo();
		break;
	case 2:
		Buttons->disable();
		GameLogic::getInstance()->repeat();
		break;
	case 3:
		Scene::getInstance()->setInitCamera("player1");
		GameLogic::getInstance()->resetGame();
		break;
	}
}

void Interface::processKeyboard(unsigned char key, int x, int y) {
	modifiers = glutGetModifiers();
}

void Interface::processMouse(int button, int state, int x, int y) {
	modifiers = glutGetModifiers();

	prev_X = x;
	prev_Y = y;

	pressing_left = (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN);
	pressing_right = (button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN);
	pressing_middle = (button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN);

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		performPicking(x, y);
	}

	glutPostRedisplay();

}

void Interface::performPicking(int x, int y) {

	Scene::getInstance()->initCamera();

	glSelectBuffer(selectBufSize, selectBuf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projmat);
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix((GLdouble) x, (GLdouble) (Scene::HEIGHT - y), 5.0, 5.0,
			viewport);
	glMultMatrixf(projmat);
	inSelectMode = true;
	glLoadName(-1);
	glMatrixMode(GL_MODELVIEW);
	display();
	inSelectMode = false;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void Interface::processHits(GLint hits, GLuint buffer[]) {
	GLuint *ptr = buffer;
	GLuint mindepth = numeric_limits<GLuint>::max();
	GLuint *selected = NULL;
	GLuint nselected;

	vector<GLuint> selected_names;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i = 0; i < hits; i++) {
		int num = *ptr;

		ptr++;
		GLuint z1 = *ptr;
		ptr += 2;
		if (z1 < mindepth && num > 0) {
			mindepth = z1;
			selected = ptr;
			nselected = num;
		}
		ptr += num;
	}

	if (selected != NULL) {
		for (unsigned int i = 0; i < nselected; i++) {
			selected_names.push_back(selected[i]);
		}
		Scene::getInstance()->processPickedNodes(selected_names);
	}

}

void Interface::processMouseMoved(int x, int y) {
	modifiers = glutGetModifiers();
}

void Interface::processPassiveMouseMoved(int x, int y) {

}

void Interface::processGUI(GLUI_Control *ctrl) {
	int id = ctrl->user_id;
	map<string, int*>::iterator it;

	if (id == radio_id) {
		int val = cams_group->get_int_val();
		for (it = cams_rb.begin(); it != cams_rb.end(); it++) {
			if (it->second[1] == val) {
				Scene::getInstance()->setInitCamera(it->first);
				return;
			}
		}
	}
	if (id == drawmd_id) {
		switch (drawmd_grp->get_int_val()) {
		case 0:
			Scene::getInstance()->setDrawmode("fill");
			break;
		case 1:
			Scene::getInstance()->setDrawmode("line");
			break;
		case 2:
			Scene::getInstance()->setDrawmode("point");
			break;
		}

	}

	for (it = lights_cb.begin(); it != lights_cb.end(); it++) {
		if (it->second[1] == id) {
			Scene::getInstance()->getLight(it->first)->toggleLight();
			return;
		}
	}
}

