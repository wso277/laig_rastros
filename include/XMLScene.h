#ifndef _XMLSCENE_H_
#define _XMLSCENE_H_

#include "TinyXML/tinyxml.h"
#include <vector>
#include <string>
#include <stack>

using namespace std;

class XMLScene {
	bool valid;
	TiXmlDocument* doc;
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightingElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearencesElement;
	TiXmlElement* animationsElement;
	TiXmlElement* graphElement;
	string yaf_path;
	string texture_base_path;
	stack<string> app_stck;
	vector<string> nodes_being_processed;
	vector<string> nodes_finished_processing;

public:
	XMLScene();
	~XMLScene();
	void loadFile();
	TiXmlElement *findChildByAttribute(TiXmlElement *parent,
	        const char * attr,
	        const char *val);
	bool parseGlobals();
	bool parseCameras();
	bool parseLighting();
	bool parseTextures();
	bool parseAppearences();
	bool parseAnimations();
	bool parseGraph();
	bool parseNode(TiXmlElement *curr_node, bool is_inside_dl);
	void setPaths();
	string getTexturePath(string texture_path, string texture_file);
};

#endif
