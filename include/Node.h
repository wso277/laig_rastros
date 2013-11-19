#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>
#include "Appearance.h"
#include <map>
#include <stack>

using namespace std;

enum nodeTypes { NONE, DISPLAY_LIST};

class MyPrimitive;

/**
 * super class containing all the information about one node, including appearance, children, and primitives
 */
class Node {
protected:

	string id;
	float transforms[16];
	vector<string> refs;
	string nodeAppearance;
	string nodeAnimation;
	vector<MyPrimitive *> prims;

public:
	Node();
	Node(string id);
	Node(string id, float transforms[16]);
	void addRef(string ref);
	virtual void setAppearance(string appearance);
	void setAnimation(string animation);
	void setTransform(float transforms[16]);
	void resetTransform();
	void addTranslate(float x, float y, float z);
	void addScale(float x, float y, float z);
	void addRotation(float angle, char axis);
	void addPrimitive(MyPrimitive *prim);
	const float* getTransform();
	Appearance *getAppearance();
	vector<string> getRefs();
	string getAnimation();
	string getId();
	virtual ~Node();
	virtual void processNode(stack<string> apps_stack, stack<string> ani_stack);
	virtual void drawPrims(string appearance);
	virtual void closeDefinition(stack<string> apps_stack);
	virtual int getType();
};

#endif /* NODE_H_ */
