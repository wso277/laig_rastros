
#include <iostream>
#include <exception>

#include "XMLScene.h"
#include "Scene.h"
#include "InvalidXMLException.h"
#include "InvalidPreAttrException.h"
#include "InvalidTexFile.h"
#include "InvalidTexRef.h"
#include "Interface.h"
#include "InvalidNumLights.h"

using std::cout;
using std::exception;

int main_window;

int main(int argc, char* argv[]) {

	XMLScene parser;
	parser.setPaths();

	//Application window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(Scene::WIDTH, Scene::HEIGHT);
	glutInitWindowPosition(100, 100);
	main_window = glutCreateWindow(Scene::getInstance()->getRootId().c_str());

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	try {
		parser.loadFile();
	} catch (InvalidXMLException &e) {
		return -1;
	} catch (InvalidPreAttrException &e) {
		cout << e.getErrMessg();
		return -1;
	} catch (InvalidTexFile &e) {
		cout << "Invalid path to texture file: " << e.getFilePath() << endl;
		return -1;
	} catch (InvalidTexRef &e) {
		cout << "Invalid reference to texture: " << e.getRef() << endl;
	} catch (InvalidNumLights &e) {
		cout << "Excessive number of lights defined!" << endl;
	} catch (exception &e) {
		cout << "Unknown exception occured. Exiting...\n" << e.what() << endl;
		return -1;
	}

	Scene::getInstance()->initScene();

	Interface interface;

	interface.init(main_window);
	interface.initGUI();

	glutMainLoop();

	return 0;
}
