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
class Piece;
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
	static int node_name_index;
	int node_name;
	bool is_selectable;
	bool is_visible;

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
	vector<MyPrimitive *> getPrims();
	int getName();
	void setName(int name);
	static int getNextName();
	bool isSelectable();
	virtual void setSelectable(bool sel);
	void processPick(vector<unsigned int> names);
	void setVisibility(bool is_visible);
	bool isVisible();
	void removePiece(Piece* piece);
	void removePrims();
};

#endif /* NODE_H_ */
