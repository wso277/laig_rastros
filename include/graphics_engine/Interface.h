#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <GL/glui.h>
#include <map>
#include <string>

#define MAX_CAMS 100

using namespace std;

void button_handler(int id);

//Based on CGFinterface. Simpler version
class Interface {
private:
	static int modifiers; ///< Stores the state of modifier keys (CTRL, ALT, SHIFT) for use in mouse and keyboard event handlers
	GLUI* glui_window;
	static float displacementX;
	static float displacementY;
	static bool pressing_left;
	static bool pressing_middle;
	static bool pressing_right;
	static float prev_X;
	static float prev_Y;

	// 1-> live_var
	// 2-> id
	static map<string, int*> cams_rb;
	static map<string, int*> lights_cb;
	static int *cams_vars;
	static int radio_id;
	static GLUI_RadioGroup *cams_group;
	static int drawmd_id;
	static GLUI_RadioGroup *drawmd_grp;
	static int difficulty_id;
	static GLUI_RadioGroup *difficulty_grp;
	static int *drawmode_vars;
	static int *difficulty_vars;

	static int id_counter;

	static GLuint *selectBuf;
	static int selectBufSize;

public:
	Interface();
	virtual ~Interface();

	void init(int parent);///< Initializes the interface, in terms of handler setup and other low-level operations. Should not be overriden by subclasses
	void initGUI(); ///< Initializes the graphical interface itself, i.e. creating controls and establishing relations with the scene. Should be overriden by subclasses. Default is empty.

	/** @name Static input (keyboard, mouse) handlers to be registered with GLUT
	 * These methods are global handlers that are registered by the application as the keyboard and mouse handlers.
	 * When invoked, they will route execution to the corresponding instance methods of the active CGFinterface instance.
	 */
	//@{
	static void processKeyboard(unsigned char key, int x, int y); ///< Called when a key is pressed. Receives as parameters the key pressed, and the mouse coordinates at the time of the press.
	static void processMouse(int button, int state, int x, int y); ///< Called when a mouse button event occurs.
	static void processMouseMoved(int x, int y); ///< Called when the mouse moves while *some* mouse button is pressed
	static void processPassiveMouseMoved(int x, int y);	///< Called when the mouse moves while *no* mouse button is pressed
	//@}

	static void processGUI(GLUI_Control *ctrl);	///< Static handler to be registered as callback for every control created. When a control is interacted with, this function will route execution to the active interface's processGUI() method, providing a pointer to the control.

	static void performPicking(int x, int y);
	static void processHits (GLint hits, GLuint buffer[]);
};

#endif /* INTERFACE_H_ */
