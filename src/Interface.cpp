#include "Interface.h"
#include "Scene.h"

extern int main_window;

using namespace std;

extern float view_rotate[16];

extern float obj_pos[3];

int Interface::modifiers = 0;
map<string, int*> Interface::cams_rb;
map<string, int*> Interface::lights_cb;
int *Interface::cams_vars = new int[MAX_CAMS];
int Interface::id_counter = 0;
int Interface::radio_id = 0;
GLUI_RadioGroup *Interface::cams_group = NULL;
int Interface::drawmd_id = 0;
GLUI_RadioGroup *Interface::drawmd_grp = NULL;
int *Interface::drawmode_vars = new int[3];
float Interface::displacementX = 0;
float Interface::displacementY = 0;
bool Interface::pressing_left = 0;
bool Interface::pressing_middle = 0;
bool Interface::pressing_right = 0;
float Interface::prev_X = 0;
float Interface::prev_Y = 0;
GLuint* Interface::selectBuf = new GLuint[(Scene::getInstance()->getDepth() + 3) * Scene::getInstance()->getSceneSize()];

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
}

void Interface::initGUI() {

	// create cameras panel
	CameraElem::iterator cam_it;
	int i = 0;
	int init_camera_pos = 0;
	for (cam_it = Scene::getInstance()->cameras.begin(); cam_it != Scene::getInstance()->cameras.end(); cam_it++) {
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
	cams_group = glui_window->add_radiogroup_to_panel(camsPanel, cams_vars, radio_id, Interface::processGUI);

	map<string, int*>::iterator cb_it;
	for (cb_it = cams_rb.begin(); cb_it != cams_rb.end(); cb_it++) {
		glui_window->add_radiobutton_to_group(cams_group, cb_it->first.c_str());
	}

	cams_group->set_int_val(init_camera_pos);

	glui_window->add_column(true);

	// create lights panel
	vector<Light *>::iterator light_it;
	for (light_it = Scene::getInstance()->lights.begin(); light_it != Scene::getInstance()->lights.end(); light_it++) {
		int live_var = 0;
		int id = id_counter++;
		if ((*light_it)->isEnabled())
			live_var = -1;

		int* ptrL = new int[2];
		ptrL[0] = live_var;
		ptrL[1] = id;
		lights_cb.insert(map<string, int*>::value_type((*light_it)->getId(), ptrL));
	}

	GLUI_Panel *lightsPanel = glui_window->add_panel("Lights");

	map<string, int*>::iterator rb_it;
	for (rb_it = lights_cb.begin(); rb_it != lights_cb.end(); rb_it++) {
		glui_window->add_checkbox_to_panel(lightsPanel, rb_it->first.c_str(), &rb_it->second[0], rb_it->second[1],
				Interface::processGUI);
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
	drawmd_grp = glui_window->add_radiogroup_to_panel(drawmodePanel, drawmode_vars, drawmd_id, Interface::processGUI);
	glui_window->add_radiobutton_to_group(drawmd_grp, "Fill");
	glui_window->add_radiobutton_to_group(drawmd_grp, "Wireframe");
	glui_window->add_radiobutton_to_group(drawmd_grp, "Points");
	drawmd_grp->set_int_val(0);

	glui_window->add_column(true);
	GLUI_Rotation *view_rot = glui_window->add_rotation("Rotacao", view_rotate);
	view_rot->set_spin(1.0);

	GLUI_Translation *trans_z = glui_window->add_translation("Zoom", GLUI_TRANSLATION_Z, &obj_pos[2]);
	trans_z->set_speed(.02);

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
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer((Scene::getInstance()->getDepth() + 3) * Scene::getInstance()->getSceneSize(), selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix((GLdouble) x, (GLdouble) (Scene::HEIGHT - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	display();

	// restore original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void Interface::processHits(GLint hits, GLuint buffer[]) {
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected = NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i = 0; i < hits; i++) {
		int num = *ptr;
		ptr++;
		GLuint z1 = *ptr;
		ptr++;
		ptr++;
		if (z1 < mindepth && num > 0) {
			mindepth = z1;
			selected = ptr;
			nselected = num;
		}
		for (int j = 0; j < num; j++)
			ptr++;
	}

	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected != NULL) {
		// this should be replaced by code handling the picked object's ID's (stored in "selected"),
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (int i = 0; i < nselected; i++)
			printf("%d ", selected[i]);
		printf("\n");
	} else
		printf("Nothing selected while picking \n");
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

