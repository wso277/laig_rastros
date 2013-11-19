#ifndef SCENE_H_
#define SCENE_H_

#include <string>
#include <vector>
#include <map>
#include "Light.h"
#include "Camera.h"
#include "Node.h"
#include "CGFtexture.h"
#include "Animation.h"

using namespace std;

#define ANIMATION_TIME 16
/**
 * definition of the maps types
 */
typedef map<string, Node*> GraphElem;
typedef map<string, CGFtexture *> TexElem;
typedef map<string, Appearance *> AppearanceElem;
typedef map<string, Camera *> CameraElem;
typedef map<string, Animation *> AnimationElem;

/**
 * main glut and glui functions
 */
void display();
void reshape(int h, int w);
void updateValues(int index);
void idle();

/**
 * class containing all the information needed for a scene, lights, cameras, global variables...
 */
class Scene {
private:
	static Scene *instance;

	float bckg_r, bckg_g, bckg_b, bckg_a;
	unsigned int drawmode;
	unsigned int shading;
	unsigned int cullface;
	unsigned int cullorder;
	string rootId;
	string init_camera;
	vector<Light*> lights;
	CameraElem cameras;
	TexElem textures;
	AppearanceElem appearances;
	AnimationElem animations;
	GraphElem graph;
	vector<string> animation_index;
	bool doublesided;
	bool local;
	bool enabled;
	float amb_r, amb_g, amb_b, amb_a;
	Scene();
public:
	static unsigned int HEIGHT;
	static unsigned int WIDTH;
	static Scene *getInstance();
	void setBackground(float bckg_r, float bckg_g, float bckg_b, float bckg_a);
	void setDrawmode(string drawmode);
	void setShading(string shading);
	void setCullface(string cullface);
	void setCullorder(string cullorder);
	void setRootId(string rootId);
	void setInitCamera(string init_camera);
	void addLight(Light* light);
	void addCamera(string key, Camera* camera);
	void addTexture(string key, string path);
	void addAppearance(string key, Appearance* appearance);
	void addAnimation(string key, Animation* animation);
	void addNode(string key, Node* node);
	void setGlobalLights(bool doublesided, bool local, bool enabled);
	void setAmb(float amb_r, float amb_g, float amb_b, float amb_a);
	unsigned int getDrawmode();
	unsigned int getShading();
	const unsigned int getCullface();
	const unsigned int getCullorder();
	string getRootId();
	Light* getLight(int index);
	Light* getLight(string id);
	Camera* getCamera(string key);
	CGFtexture *getTexture(string key);
	Appearance* getAppearance(string key);
	Animation* getAnimation(string key);
	Node* getNode(string key);
	void applyLights();
	void initCamera();
	void initScene();
	friend void display();
	friend void reshape(int, int);
	friend void idle();
	virtual ~Scene();
	void drawScene();
	friend void updateValues(int index);
	string getAnimationIndex(int index);
	friend class Interface;
	string getInitCamera();
	string findNextNameAvail(string id);
	string findLastNameAvail(string id);
};

#endif /* SCENE_H_ */
