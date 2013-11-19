#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <GL/glui.h>
#include <map>
#include <string>

#define MAX_CAMS 100

using namespace std;

//Based on CGFinterface. Simpler version
class Interface {
private:
	static int modifiers; ///< Stores the state of modifier keys (CTRL, ALT, SHIFT) for use in mouse and keyboard event handlers
	static Interface * activeInterface;
	GLUI* glui_window;
	float displacementX;
	float displacementY;
	bool pressing_left;
	bool pressing_middle;
	bool pressing_right;
	float prev_X;
	float prev_Y;

	// 1-> live_var
	// 2-> id
	static map<string, int*> cams_rb;
	static map<string, int*> lights_cb;
	static int *cams_vars;
	static int radio_id;
	static GLUI_RadioGroup *cams_group;
	static int drawmd_id;
	static GLUI_RadioGroup *drawmd_grp;
	static int *drawmode_vars;

	static int id_counter;

public:
	Interface();
	virtual ~Interface();

	void init(int parent);///< Initializes the interface, in terms of handler setup and other low-level operations. Should not be overriden by subclasses
	void initGUI(); ///< Initializes the graphical interface itself, i.e. creating controls and establishing relations with the scene. Should be overriden by subclasses. Default is empty.
	static void setActiveInterface(Interface *gli); ///< registers _gli_ as the active interface

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
};

#endif /* INTERFACE_H_ */
