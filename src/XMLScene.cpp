#include "XMLScene.h"
#include <iostream>
#include <algorithm>
#include "utils.h"
#include "InvalidXMLException.h"
#include "Scene.h"
#include "Camera.h"
#include "Perspective.h"
#include "Ortho.h"
#include "Light.h"
#include "Omnilight.h"
#include "Spotlight.h"
#include "MyRectangle.h"
#include "MyTriangle.h"
#include "MyCylinder.h"
#include "MySphere.h"
#include "MyTorus.h"
#include "Node.h"
#include <string.h>
#include <stack>
#include <iterator>
#include "Animation.h"
#include "DisplayList.h"
#include "Plane.h"
#include "MyPatch.h"
#include "MyWaterLine.h"
#include "MyVehicle.h"

#define MAX_STRING_LEN 256

using namespace std;

XMLScene::XMLScene() {
	valid = false;
}

XMLScene::~XMLScene() {
	delete (doc);
}

void XMLScene::loadFile() {
	bool has_animations = true;
	doc = new TiXmlDocument(yaf_path.c_str());
	bool loadOkay = doc->LoadFile();

	if (!loadOkay) {
		printf("Could not load file '%s'. Error='%s'.\n", yaf_path.c_str(), doc->ErrorDesc());
		throw InvalidXMLException();
	}

	TiXmlElement* yafElement = doc->FirstChildElement("yaf");

	if (yafElement == NULL) {
		printf("Main yaf block element not found!\n");
		throw InvalidXMLException();
	}

	if ((globalsElement = yafElement->FirstChildElement("globals")) == NULL) {
		printf("Globals block not found!\n");
		throw InvalidXMLException();
	}
	if ((camerasElement = yafElement->FirstChildElement("cameras")) == NULL) {
		printf("Cameras block not found!\n");
		throw InvalidXMLException();
	}
	if ((lightingElement = yafElement->FirstChildElement("lighting")) == NULL) {
		printf("Lighting block not found!\n");
		throw InvalidXMLException();
	}
	if ((texturesElement = yafElement->FirstChildElement("textures")) == NULL) {
		printf("Textures block not found!\n");
		throw InvalidXMLException();
	}
	if ((appearencesElement = yafElement->FirstChildElement("appearances")) == NULL) {
		printf("Appearences block not found!\n");
		throw InvalidXMLException();
	}
	if ((animationsElement = yafElement->FirstChildElement("animations")) == NULL) { //block animations is not mandatory??? our choice
		printf("Animations block not found!\n");
		has_animations = false;
	}
	if ((graphElement = yafElement->FirstChildElement("graph")) == NULL) {
		printf("Graph block not found!\n");
		throw InvalidXMLException();
	}

	parseGlobals();
	printf("\n\n");
	parseCameras();
	printf("\n\n");
	parseLighting();
	printf("\n\n");
	parseTextures();
	printf("\n\n");
	parseAppearences();
	printf("\n\n");
	if (has_animations) {
		parseAnimations();
		printf("\n\n");
	}
	parseGraph();
	printf("\n\n");
}

TiXmlElement *XMLScene::findChildByAttribute(TiXmlElement *parent, const char * attr, const char *val) {
	TiXmlElement *child = parent->FirstChildElement();
	bool found = false;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr), val) == 0)
			found = true;
		else
			child = child->NextSiblingElement();
	if (!found)
		return NULL;
	else
		return child;
}

bool XMLScene::parseGlobals() {
	printf("Processing globals...\n\n");
	char background_str[MAX_STRING_LEN];
	char drawmode[MAX_STRING_LEN];
	char shading[MAX_STRING_LEN];
	char cullface[MAX_STRING_LEN];
	char cullorder[MAX_STRING_LEN];
	double back_r = 0, back_g = 0, back_b = 0, back_a = 0;

	if ((strdup(background_str, globalsElement->Attribute("background"))) == NULL) {
		printf("Error parsing background!\n");
		throw InvalidXMLException();
	}

	if (sscanf(background_str, "%lf %lf %lf %lf", &back_r, &back_g, &back_b, &back_a) != 4) {
		printf("Invalid arguments of background!\n");
		throw InvalidXMLException();
	}

	if (strdup(drawmode, globalsElement->Attribute("drawmode")) == NULL) {
		printf("Error parsing drawmode global!\n");
		throw InvalidXMLException();
	}

	if (strdup(shading, globalsElement->Attribute("shading")) == NULL) {
		printf("Error parsing shading global!\n");
		throw InvalidXMLException();
	}

	if (strdup(cullface, globalsElement->Attribute("cullface")) == NULL) {
		printf("Error parsing cullface global!\n");
		throw InvalidXMLException();
	}

	if (strdup(cullorder, globalsElement->Attribute("cullorder")) == NULL) {
		printf("Error parsing cullorder global!\n");
		throw InvalidXMLException();
	}

	Scene::getInstance()->setBackground(back_r, back_g, back_b, back_a);
	Scene::getInstance()->setDrawmode(drawmode);
	Scene::getInstance()->setShading(shading);
	Scene::getInstance()->setCullface(cullface);
	Scene::getInstance()->setCullorder(cullorder);

	printf("Globals\nbackground: (%f,%f,%f,%f)\ndrawmode: %s\nshading: %s\ncullface: %s\ncullorder: %s\n", back_r,
			back_g, back_b, back_a, drawmode, shading, cullface, cullorder);
	return true;
}

bool XMLScene::parseCameras() {
	bool valid_nr_cameras = false;
	printf("Processing cameras...\n\n");
	char init_camera[MAX_STRING_LEN];
	if ((strdup(init_camera, camerasElement->Attribute("initial"))) == NULL) {
		printf("Error parsing background!\n");
		exit(-1);
	}

	Scene::getInstance()->setInitCamera(init_camera);

	printf("Initial camera: %s\n", init_camera);

	TiXmlElement * persp_cam = NULL;
	TiXmlElement * ortho_cam = NULL;

	if ((persp_cam = camerasElement->FirstChildElement("perspective")) != NULL) {
		valid_nr_cameras = true;
		char persp_cam_id[MAX_STRING_LEN];
		char persp_cam_pos[MAX_STRING_LEN];
		char persp_cam_targ[MAX_STRING_LEN];
		double persp_cam_near = 0, persp_cam_far = 0, persp_cam_angle = 0, persp_cam_pos_x = 0, persp_cam_pos_y = 0,
				persp_cam_pos_z = 0, persp_cam_targ_x = 0, persp_cam_targ_y = 0, persp_cam_targ_z = 0;

		do {
			if ((strdup(persp_cam_id, persp_cam->Attribute("id"))) == NULL) {
				printf("Error parsing perspective camera id!\n");
				throw InvalidXMLException();
			}

			if (persp_cam->Attribute("near", &persp_cam_near) == NULL) {
				printf("Error parsing \"%s\" camera near field!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			if (persp_cam->Attribute("far", &persp_cam_far) == NULL) {
				printf("Error parsing \"%s\" camera far field!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			if (persp_cam->Attribute("angle", &persp_cam_angle) == NULL) {
				printf("Error parsing \"%s\" camera angle field!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			if ((strdup(persp_cam_pos, persp_cam->Attribute("pos"))) == NULL) {
				printf("Error parsing \"%s\" camera pos field!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			if (sscanf(persp_cam_pos, "%lf %lf %lf", &persp_cam_pos_x, &persp_cam_pos_y, &persp_cam_pos_z) != 3) {
				printf("Invalid arguments of \"%s\" camera pos!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			if ((strdup(persp_cam_targ, persp_cam->Attribute("target"))) == NULL) {
				printf("Error parsing \"%s\" camera target field!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			if (sscanf(persp_cam_targ, "%lf %lf %lf", &persp_cam_targ_x, &persp_cam_targ_y, &persp_cam_targ_z) != 3) {
				printf("Invalid arguments of \"%s\" camera target!\n", persp_cam_id);
				throw InvalidXMLException();
			}

			Camera *persp_cam = new Perspective(persp_cam_id);
			((Perspective *) persp_cam)->setNear(persp_cam_near);
			((Perspective *) persp_cam)->setFar(persp_cam_far);
			((Perspective *) persp_cam)->setAngle(persp_cam_angle);
			((Perspective *) persp_cam)->setPos(persp_cam_pos_x, persp_cam_pos_y, persp_cam_pos_z);
			((Perspective *) persp_cam)->setTarget(persp_cam_targ_x, persp_cam_targ_y, persp_cam_targ_z);

			Scene::getInstance()->addCamera(persp_cam_id, persp_cam);

			printf("Perspective camera.\nid: %s\nnear: %f\nfar: %f\nangle: %f\npos: (%f,%f,%f)\ntarget: (%f,%f,%f)\n\n",
					persp_cam_id, persp_cam_near, persp_cam_far, persp_cam_angle, persp_cam_pos_x, persp_cam_pos_y,
					persp_cam_pos_z, persp_cam_targ_x, persp_cam_targ_y, persp_cam_targ_z);
		} while ((persp_cam = persp_cam->NextSiblingElement("perspective")) != NULL);
	} else {
		printf("There are no perspective cameras.\n");
	}

	if ((ortho_cam = camerasElement->FirstChildElement("ortho")) != NULL) {
		valid_nr_cameras = true;
		char ortho_cam_id[MAX_STRING_LEN];
		double ortho_cam_near = 0, ortho_cam_far = 0, ortho_cam_left = 0, ortho_cam_right = 0, ortho_cam_top = 0,
				ortho_cam_bottom = 0;
		do {

			if ((strdup(ortho_cam_id, ortho_cam->Attribute("id"))) == NULL) {
				printf("Error parsing ortho camera id!\n");
				throw InvalidXMLException();
			}

			if (ortho_cam->Attribute("near", &ortho_cam_near) == NULL) {
				printf("Error parsing \"%s\" camera near field!\n", ortho_cam_id);
				throw InvalidXMLException();
			}

			if (ortho_cam->Attribute("far", &ortho_cam_far) == NULL) {
				printf("Error parsing \"%s\" camera far field!\n", ortho_cam_id);
				throw InvalidXMLException();
			}

			if (ortho_cam->Attribute("left", &ortho_cam_left) == NULL) {
				printf("Error parsing \"%s\" camera left field!\n", ortho_cam_id);
				throw InvalidXMLException();
			}

			if (ortho_cam->Attribute("right", &ortho_cam_right) == NULL) {
				printf("Error parsing \"%s\" camera right field!\n", ortho_cam_id);
				throw InvalidXMLException();
			}

			if (ortho_cam->Attribute("top", &ortho_cam_top) == NULL) {
				printf("Error parsing \"%s\" camera top field!\n", ortho_cam_id);
				throw InvalidXMLException();
			}

			if (ortho_cam->Attribute("bottom", &ortho_cam_bottom) == NULL) {
				printf("Error parsing \"%s\" camera bottom field!\n", ortho_cam_id);
				throw InvalidXMLException();
			}

			Camera *ortho_cam = new Ortho(ortho_cam_id);
			((Ortho *) ortho_cam)->setNear(ortho_cam_near);
			((Ortho *) ortho_cam)->setFar(ortho_cam_far);
			((Ortho *) ortho_cam)->setLeft(ortho_cam_left);
			((Ortho *) ortho_cam)->setRight(ortho_cam_right);
			((Ortho *) ortho_cam)->setTop(ortho_cam_top);
			((Ortho *) ortho_cam)->setBottom(ortho_cam_bottom);

			Scene::getInstance()->addCamera(ortho_cam_id, ortho_cam);

			printf("Ortho camera.\nid: %s\nnear: %f\nfar: %f\nleft: %f\nright: %f\ntop: %f\nbottom: %f\n\n",
					ortho_cam_id, ortho_cam_near, ortho_cam_far, ortho_cam_left, ortho_cam_right, ortho_cam_top,
					ortho_cam_bottom);
		} while ((ortho_cam = ortho_cam->NextSiblingElement("ortho")) != NULL);

	} else {
		printf("There are no ortho cameras.\n");
	}
	if (!valid_nr_cameras) {
		printf("There must be at least one camera defined!\n");
		throw InvalidXMLException();
	}
	return true;
}

bool XMLScene::parseLighting() {
	unsigned int lights_counter = 0;
	printf("Processing lights...\n\n");
	char tmp_str[MAX_STRING_LEN];
	bool is_doublesided = false, is_local = true, is_enabled = true;
	double light_amb_r = 0, light_amb_g = 0, light_amb_b = 0, light_amb_a = 0;

	if (lightingElement->QueryBoolAttribute("doublesided", &is_doublesided) != TIXML_SUCCESS) {
		printf("Invalid argument of doublesided!\n");
		throw InvalidXMLException();
	}

	if (lightingElement->QueryBoolAttribute("local", &is_local) != TIXML_SUCCESS) {
		printf("Invalid argument of local!\n");
		throw InvalidXMLException();
	}

	if (lightingElement->QueryBoolAttribute("enabled", &is_enabled) != TIXML_SUCCESS) {
		printf("Invalid argument of enabled!\n");
		throw InvalidXMLException();
	}

	if (strdup(tmp_str, lightingElement->Attribute("ambient")) == NULL) {
		printf("Invalid argument of ambient!\n");
		throw InvalidXMLException();
	}

	if (sscanf(tmp_str, "%lf %lf %lf %lf", &light_amb_r, &light_amb_g, &light_amb_b, &light_amb_a) != 4) {
		printf("Invalid argument of ambient!\n");
		throw InvalidXMLException();
	}

	Scene::getInstance()->setGlobalLights(is_doublesided, is_local, is_enabled);
	Scene::getInstance()->setAmb(light_amb_r, light_amb_g, light_amb_b, light_amb_a);

	printf("Global lightign attributes.\ndoublesided: %s\nlocal: %s\nenabled: %s\nambient: (%f,%f,%f,%f)\n\n",
			is_doublesided ? "true" : "false", is_local ? "true" : "false", is_enabled ? "true" : "false", light_amb_r,
			light_amb_g, light_amb_b, light_amb_a);

	TiXmlElement *omni = NULL, *spot = NULL;

	if ((omni = lightingElement->FirstChildElement("omni")) != NULL) {
		do {
			lights_counter++;
			char omni_id[MAX_STRING_LEN], tmp_str[MAX_STRING_LEN];
			bool omni_enabled = false;
			double omni_location_x = 0, omni_location_y = 0, omni_location_z = 0, omni_amb_r = 0, omni_amb_g = 0,
					omni_amb_b = 0, omni_amb_a = 0, omni_dif_r = 0, omni_dif_g = 0, omni_dif_b = 0, omni_dif_a = 0,
					omni_spec_r = 0, omni_spec_g = 0, omni_spec_b = 0, omni_spec_a = 0;
			if (strdup(omni_id, omni->Attribute("id")) == NULL) {
				printf("Invalid id argument!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, omni->Attribute("enabled")) == NULL) {
				printf("Error on atribute \"enabled\" of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (strcmp(tmp_str, "true") == 0) {
				omni_enabled = true;
			} else if (strcmp(tmp_str, "false") == 0) {
				omni_enabled = false;
			} else {
				printf("Invalid argument of local!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, omni->Attribute("location")) == NULL) {
				printf("No attribute \"location\" of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf", &omni_location_x, &omni_location_y, &omni_location_z) != 3) {
				printf("Invalid location argument of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, omni->Attribute("ambient")) == NULL) {
				printf("No attribute \"ambient\" of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &omni_amb_r, &omni_amb_g, &omni_amb_b, &omni_amb_a) != 4) {
				printf("Invalid ambient argument of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, omni->Attribute("diffuse")) == NULL) {
				printf("No attribute \"diffuse\" of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &omni_dif_r, &omni_dif_g, &omni_dif_b, &omni_dif_a) != 4) {
				printf("Invalid diffuse argument of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, omni->Attribute("specular")) == NULL) {
				printf("No attribute \"specular\" of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &omni_spec_r, &omni_spec_g, &omni_spec_b, &omni_spec_a) != 4) {
				printf("Invalid specular argument of %s light!\n", omni_id);
				throw InvalidXMLException();
			}

			Light *omni_light = new Omnilight(omni_id, omni_enabled);
			((Omnilight *) omni_light)->setLocation(omni_location_x, omni_location_y, omni_location_z);
			((Omnilight *) omni_light)->setAmbient(omni_amb_r, omni_amb_g, omni_amb_b, omni_amb_a);
			((Omnilight *) omni_light)->setDiffuse(omni_dif_r, omni_dif_g, omni_dif_b, omni_dif_a);
			((Omnilight *) omni_light)->setSpecular(omni_spec_r, omni_spec_g, omni_spec_b, omni_spec_a);

			Scene::getInstance()->addLight(omni_light);

			printf(
					"Omni light.\nid: %s\nenabled: %s\nlocation: (%f,%f,%f)\nambient: (%f,%f,%f,%f)\ndiffuse: (%f,%f,%f,%f)\nspecular: (%f,%f,%f,%f)\n\n",
					omni_id, omni_enabled ? "true" : "false", omni_location_x, omni_location_y, omni_location_z,
					omni_amb_r, omni_amb_g, omni_amb_b, omni_amb_a, omni_dif_r, omni_dif_g, omni_dif_b, omni_dif_a,
					omni_spec_r, omni_spec_g, omni_spec_b, omni_spec_a);
		} while ((omni = omni->NextSiblingElement("omni")) != NULL);
	} else {
		printf("There are no omni lights.\n");
	}

	if ((spot = lightingElement->FirstChildElement("spot")) != NULL) {
		do {
			lights_counter++;
			char spot_id[MAX_STRING_LEN], tmp_str[MAX_STRING_LEN];
			bool spot_enabled = false;
			double spot_location_x = 0, spot_location_y = 0, spot_location_z = 0, spot_amb_r = 0, spot_amb_g = 0,
					spot_amb_b = 0, spot_amb_a = 0, spot_dif_r = 0, spot_dif_g = 0, spot_dif_b = 0, spot_dif_a = 0,
					spot_spec_r = 0, spot_spec_g = 0, spot_spec_b = 0, spot_spec_a = 0, spot_angle = 0, spot_exp = 0,
					spot_dir_x = 0, spot_dir_y = 0, spot_dir_z = 0;

			if (strdup(spot_id, spot->Attribute("id")) == NULL) {
				printf("Invalid id argument!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, spot->Attribute("enabled")) == NULL) {
				printf("Error on atribute \"enabled\" of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (strcmp(tmp_str, "true") == 0) {
				spot_enabled = true;
			} else if (strcmp(tmp_str, "false") == 0) {
				spot_enabled = false;
			} else {
				printf("Invalid argument of local!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, spot->Attribute("location")) == NULL) {
				printf("No attribute \"location\" of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf", &spot_location_x, &spot_location_y, &spot_location_z) != 3) {
				printf("Invalid location argument of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, spot->Attribute("ambient")) == NULL) {
				printf("No attribute \"ambient\" of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &spot_amb_r, &spot_amb_g, &spot_amb_b, &spot_amb_a) != 4) {
				printf("Invalid ambient argument of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, spot->Attribute("diffuse")) == NULL) {
				printf("No attribute \"diffuse\" of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &spot_dif_r, &spot_dif_g, &spot_dif_b, &spot_dif_a) != 4) {
				printf("Invalid diffuse argument of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, spot->Attribute("specular")) == NULL) {
				printf("No attribute \"specular\" of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &spot_spec_r, &spot_spec_g, &spot_spec_b, &spot_spec_a) != 4) {
				printf("Invalid specular argument of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (spot->Attribute("angle", &spot_angle) == NULL) {
				printf("Invalid angle atribute of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (spot->Attribute("exponent", &spot_exp) == NULL) {
				printf("Invalid exponent atribute of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, spot->Attribute("direction")) == NULL) {
				printf("No attribute \"direction\" of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf", &spot_dir_x, &spot_dir_y, &spot_dir_z) != 3) {
				printf("Invalid location argument of %s light!\n", spot_id);
				throw InvalidXMLException();
			}

			Light *spot_light = new Spotlight(spot_id, spot_enabled);
			((Spotlight *) spot_light)->setLocation(spot_location_x, spot_location_y, spot_location_z);
			((Spotlight *) spot_light)->setAmbient(spot_amb_r, spot_amb_g, spot_amb_b, spot_amb_a);
			((Spotlight *) spot_light)->setDiffuse(spot_dif_r, spot_dif_g, spot_dif_b, spot_dif_a);
			((Spotlight *) spot_light)->setSpecular(spot_spec_r, spot_spec_g, spot_spec_b, spot_spec_a);
			((Spotlight *) spot_light)->setAngle(spot_angle);
			((Spotlight *) spot_light)->setExponent(spot_exp);
			((Spotlight *) spot_light)->setDir(spot_dir_x, spot_dir_y, spot_dir_z);

			Scene::getInstance()->addLight(spot_light);

			printf(
					"Spot light.\nid: %s\nenabled: %s\nlocation: (%f,%f,%f)\nambient: (%f,%f,%f,%f)\ndiffuse: (%f,%f,%f,%f)\nspec: (%f,%f,%f,%f)\nangle: %f\nexponent: %f\ndirection: (%f,%f,%f)\n\n",
					spot_id, spot_enabled ? "true" : "false", spot_location_x, spot_location_y, spot_location_z,
					spot_amb_r, spot_amb_g, spot_amb_b, spot_amb_a, spot_dif_r, spot_dif_g, spot_dif_b, spot_dif_a,
					spot_spec_r, spot_spec_g, spot_spec_b, spot_spec_a, spot_angle, spot_exp, spot_dir_x, spot_dir_y,
					spot_dir_z);
		} while ((spot = spot->NextSiblingElement("spot")) != NULL);
	} else {
		printf("There are no omni lights.\n");
	}

	if (lights_counter > 8 || lights_counter < 1) {
		printf("There are more lights than the ones that can be used. Exiting...\n");
		throw InvalidXMLException();
	}
	return true;
}

bool XMLScene::parseTextures() {

	printf("Processing textures...\n\n");

	TiXmlElement *text = NULL;
	if ((text = texturesElement->FirstChildElement("texture")) != NULL) {
		char text_id[MAX_STRING_LEN], text_file[MAX_STRING_LEN];
		do {
			if (strdup(text_id, text->Attribute("id")) == NULL) {
				printf("Error in \"id\" attribute!\n");
				throw InvalidXMLException();
			}

			if (strdup(text_file, text->Attribute("file")) == NULL) {
				printf("Error in \"file\" attribute!\n");
				throw InvalidXMLException();
			}

			Scene::getInstance()->addTexture(text_id, texture_base_path + "/" + text_file);

			printf("id: %s\nfile: %s\n\n", text_id, text_file);
		} while ((text = text->NextSiblingElement("texture")) != NULL);
	} else {
		printf("There are no defined textures.\n");
	}
	return true;
}

bool XMLScene::parseAppearences() {

	printf("Processing appearences...\n\n");

	bool valid_nr_appear = false;
	char tmp_str[MAX_STRING_LEN], app_id[MAX_STRING_LEN], app_text_ref[MAX_STRING_LEN];
	double app_emiss_r = 0, app_emiss_g = 0, app_emiss_b = 0, app_emiss_a = 0, app_amb_r = 0, app_amb_g = 0, app_amb_b =
			0, app_amb_a = 0, app_dif_r = 0, app_dif_g = 0, app_dif_b = 0, app_dif_a = 0, app_spec_r = 0,
			app_spec_g = 0, app_spec_b = 0, app_spec_a = 0, app_shin = 0, app_text_len_s = 0, app_text_len_t = 0;
	TiXmlElement * app = NULL;
	if ((app = appearencesElement->FirstChildElement("appearance")) != NULL) {
		valid_nr_appear = true;
		do {
			if (strdup(app_id, app->Attribute("id")) == NULL) {
				printf("Error in \"id\" attribute!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, app->Attribute("emissive")) == NULL) {
				printf("Error in \"emissive\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &app_emiss_r, &app_emiss_g, &app_emiss_b, &app_emiss_a) != 4) {
				printf("Error parsing \"emissive\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, app->Attribute("ambient")) == NULL) {
				printf("Error in \"ambient\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &app_amb_r, &app_amb_g, &app_amb_b, &app_amb_a) != 4) {
				printf("Error parsing \"ambient\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, app->Attribute("diffuse")) == NULL) {
				printf("Error in \"diffuse\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &app_dif_r, &app_dif_g, &app_dif_b, &app_dif_a) != 4) {
				printf("Error parsing \"diffuse\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, app->Attribute("specular")) == NULL) {
				printf("Error in \"specular\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf %lf", &app_spec_r, &app_spec_g, &app_spec_b, &app_spec_a) != 4) {
				printf("Error parsing \"specular\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			if (app->Attribute("shininess", &app_shin) == NULL) {
				printf("Error parsing \"shininess\" attribute of %s appearence!\n", app_id);
				throw InvalidXMLException();
			}

			Appearance *new_app = new Appearance(app_id);
			new_app->setEmissivity(app_emiss_r, app_emiss_g, app_emiss_b, app_emiss_a);
			new_app->setAmbient(app_amb_r, app_amb_g, app_amb_b, app_amb_a);
			new_app->setDiffuse(app_dif_r, app_dif_g, app_dif_b, app_dif_a);
			new_app->setSpecular(app_spec_r, app_spec_g, app_spec_b, app_spec_a);

			if (strdup(app_text_ref, app->Attribute("textureref")) != NULL) {
				if (strcmp(app_text_ref, "") != 0) {
					if (app->Attribute("texlength_s", &app_text_len_s) == NULL) {
						printf("Error parsing \"texlength_s\" attribute of %s appearence!\n", app_id);
						throw InvalidXMLException();
					}

					if (app->Attribute("texlength_t", &app_text_len_t) == NULL) {
						printf("Error parsing \"texlength_t\" attribute of %s appearence!\n", app_id);
						throw InvalidXMLException();
					}

					new_app->setTextProp(app_text_ref, app_text_len_s, app_text_len_t);
				} else {
					strdup(app_text_ref, "none");
					app_text_len_s = 0.0;
					app_text_len_t = 0.0;
				}
			} else {
				strdup(app_text_ref, "none");
				app_text_len_s = 0.0;
				app_text_len_t = 0.0;
			}

			Scene::getInstance()->addAppearance(app_id, new_app);

			printf(
					"id: %s\nemissive: (%f,%f,%f,%f)\nambient: (%f,%f,%f,%f)\ndiffuse: (%f,%f,%f,%f)\nspecular: (%f,%f,%f,%f)\nshininess: %f\ntextureref: %s\ntexlength_s: %f\ntextlength_t: %f\n\n",
					app_id, app_emiss_r, app_emiss_g, app_emiss_b, app_emiss_a, app_amb_r, app_amb_g, app_amb_b,
					app_amb_a, app_dif_r, app_dif_g, app_dif_b, app_dif_a, app_spec_r, app_spec_g, app_spec_b,
					app_spec_a, app_shin, app_text_ref, app_text_len_s, app_text_len_t);

		} while ((app = app->NextSiblingElement("appearance")) != NULL);
	}
	if (!valid_nr_appear) {
		printf("There are no defined appearences!\nPlease define at least one\n");
		throw InvalidXMLException();
	}
	return true;
}

bool XMLScene::parseAnimations() {

	printf("Processing animations...\n\n");

	char ani_id[MAX_STRING_LEN], tmp_str[MAX_STRING_LEN];
	char ani_type[MAX_STRING_LEN];
	double ani_span;
	TiXmlElement * ani = NULL;
	if ((ani = animationsElement->FirstChildElement("animation")) != NULL) {
		do {
			if (strdup(ani_id, ani->Attribute("id")) == NULL) {
				printf("Error in \"id\" attribute!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, ani->Attribute("span")) == NULL) {
				printf("Error in \"span\" attribute of %s animation!\n", ani_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf", &ani_span) != 1) {
				printf("Error parsing \"span\" attribute of %s animation!\n", ani_id);
				throw InvalidXMLException();
			}

			if (strdup(ani_type, ani->Attribute("type")) == NULL) {
				printf("Error in \"type\" attribute of %s animation!\n", ani_id);
				throw InvalidXMLException();
			}
			Animation *animation = new Animation(ani_id, ani_span, ani_type);
			TiXmlElement *ctrl_point = NULL;

			if ((ctrl_point = ani->FirstChildElement("controlpoint")) == NULL) {
				printf("Block \"controlpoint\" not found!\n");
				throw InvalidXMLException();
			}

			double xx, yy, zz;
			int num_pt = 0;
			do {
				if (strdup(tmp_str, ctrl_point->Attribute("xx")) == NULL) {
					printf("Error in \"xx\" attribute of %s animation!\n", ani_id);
					throw InvalidXMLException();
				}

				if (sscanf(tmp_str, "%lf", &xx) != 1) {
					printf("Error parsing \"xx\" attribute of %s animation!\n", ani_id);
					throw InvalidXMLException();
				}

				if (strdup(tmp_str, ctrl_point->Attribute("yy")) == NULL) {
					printf("Error in \"yy\" attribute of %s animation!\n", ani_id);
					throw InvalidXMLException();
				}

				if (sscanf(tmp_str, "%lf", &yy) != 1) {
					printf("Error parsing \"yy\" attribute of %s animation!\n", ani_id);
					throw InvalidXMLException();
				}

				if (strdup(tmp_str, ctrl_point->Attribute("zz")) == NULL) {
					printf("Error in \"zz\" attribute of %s animation!\n", ani_id);
					throw InvalidXMLException();
				}

				if (sscanf(tmp_str, "%lf", &zz) != 1) {
					printf("Error parsing \"zz\" attribute of %s animation!\n", ani_id);
					throw InvalidXMLException();
				}

				animation->addPoint(xx, yy, zz);
				num_pt++;
			} while ((ctrl_point = ctrl_point->NextSiblingElement("controlpoint")) != NULL);

			if (num_pt >= 2) {
				animation->calculateDelta();
				printf("delta\n");
				Scene::getInstance()->addAnimation(ani_id, animation);
			} else {
				printf("Error! Animation %s doesn't have enought control points!", ani_id);
				throw InvalidXMLException();
			}

		} while ((ani = ani->NextSiblingElement("animation")) != NULL);
	}

	return true;
}

bool XMLScene::parseGraph() {

	printf("Processing graph...\n\n");

	char root_id[MAX_STRING_LEN];

	if (strdup(root_id, graphElement->Attribute("rootid")) == NULL) {
		printf("Error reading \"rootid\"\n");
		throw InvalidXMLException();
	}

	TiXmlElement *curr_node = NULL;
	if ((curr_node = findChildByAttribute(graphElement, "id", root_id)) == NULL) {
		printf("The root node is not declared in the file!\n");
		throw InvalidXMLException();
	}

	Scene::getInstance()->setRootId(root_id);

	app_stck.push("default");

	if (!parseNode(curr_node, false)) {
		throw InvalidXMLException();
	}
	return true;
}

bool XMLScene::parseNode(TiXmlElement *curr_node, bool is_inside_dl) {

	char node_id[MAX_STRING_LEN];

	if (strdup(node_id, curr_node->Attribute("id")) == NULL) {
		printf("Error reading \"id\" attribute!\n");
		throw InvalidXMLException();
	}

	printf("id: %s\n", node_id);

	bool is_dl = false;
	string dl_node_id;
	if (curr_node->QueryBoolAttribute("displaylist", &is_dl) != TIXML_SUCCESS) {
		printf("No \"displaylist\" attribute\n");
	}

	if (is_dl) {
		printf("Node \"%s\" defined as a display list.\n", node_id);
		dl_node_id = Scene::getInstance()->findNextNameAvail(node_id);
		printf("dl_node_id: %s\n", dl_node_id.c_str());
	}

	Node *n;

	if (is_dl) {
		n = new DisplayList(dl_node_id);
	} else {
		n = new Node(node_id);
	}

	nodes_being_processed.push_back(node_id);

	printf("Processing transformations...\n");

	TiXmlElement *transf_block = NULL;
	if ((transf_block = curr_node->FirstChildElement("transforms")) == NULL) {
		printf("Could not find \"transforms\" block on %s node!\n", node_id);
		throw InvalidXMLException();
	}

	TiXmlElement *transf = NULL;
	while ((transf = (TiXmlElement*) transf_block->IterateChildren(transf))) {
		char t_type[MAX_STRING_LEN];
		if (strdup(t_type, transf->Value()) == NULL) {
			printf("Invalid transformation on node %s\n", node_id);
			throw InvalidXMLException();
		}
		if (strcmp(t_type, "translate") == 0) {
			char tmp_str[MAX_STRING_LEN];
			double t_x = 0, t_y = 0, t_z = 0;

			if (strdup(tmp_str, transf->Attribute("to")) == NULL) {
				printf("Error on translate transformation on node %s!\n", node_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf", &t_x, &t_y, &t_z) != 3) {
				printf("Error parsing translate transformation on node %s!\n", node_id);
				throw InvalidXMLException();
			}

			n->addTranslate(t_x, t_y, t_z);

			printf("Translate\nto: (%f %f %f)\n", t_x, t_y, t_z);

		} else if (strcmp(t_type, "rotate") == 0) {
			char tmp_str[2];
			char r_axis = '\0';
			double r_angle;
			if (strdup(tmp_str, transf->Attribute("axis")) == NULL) {
				printf("Error on rotate transformation on node %s!\n", node_id);
				throw InvalidXMLException();
			}
			r_axis = tmp_str[0];

			if (transf->QueryDoubleAttribute("angle", &r_angle)) {
				printf("Error parsing rotate transformation on node %s!\n", node_id);
				throw InvalidXMLException();
			}

			n->addRotation(r_angle, r_axis);

			printf("Rotate\naxis: %c\nangle: %f\n", r_axis, r_angle);

		} else if (strcmp(t_type, "scale") == 0) {
			char tmp_str[MAX_STRING_LEN];
			double f_x = 0, f_y = 0, f_z = 0;

			if (strdup(tmp_str, transf->Attribute("factor")) == NULL) {
				printf("Error on scale transformation on node %s!\n", node_id);
				throw InvalidXMLException();
			}

			if (sscanf(tmp_str, "%lf %lf %lf", &f_x, &f_y, &f_z) != 3) {
				printf("Error parsing scale transformation on node %s!\n", node_id);
				throw InvalidXMLException();
			}

			n->addScale(f_x, f_y, f_z);

			printf("Scale\nfactor: (%f %f %f)\n", f_x, f_y, f_z);

		} else {
			printf("Invalid transformation on node %s\n", node_id);
			throw InvalidXMLException();
		}
	}

	TiXmlElement *appearance = NULL;
	if ((appearance = curr_node->FirstChildElement("appearanceref"))) {
		char app_id[MAX_STRING_LEN];
		if (strdup(app_id, appearance->Attribute("id")) == NULL) {
			printf("Error on \"appearanceref\" block on node %s!\n", node_id);
			throw InvalidXMLException();
		}

		n->setAppearance(app_id);
		app_stck.push(app_id);

		printf("Appearance\nid: %s\n", app_id);
	} else {
		app_stck.push(app_stck.top());
	}

	TiXmlElement *animation = NULL;
	if ((animation = curr_node->FirstChildElement("animationref"))) {
		if (is_inside_dl || is_dl) {
			printf("Animation defined in \"%s\" is inside a display list!\n", node_id);
			throw InvalidXMLException();
		}

		char ani_id[MAX_STRING_LEN];
		if (strdup(ani_id, animation->Attribute("id")) == NULL) {
			printf("Error on \"animationref\" block on node %s!\n", node_id);
			throw InvalidXMLException();
		}

		n->setAnimation(ani_id);

		printf("Animation\nid: %s\n", ani_id);
	} else {
		printf("No animation defined on node \"%s\".\n", node_id);
	}

	printf("Processing children...\n");

	TiXmlElement *children = NULL;

	if ((children = curr_node->FirstChildElement("children")) == NULL) {
		printf("Block \"children\" not found!\n");
		throw InvalidXMLException();
	}

	TiXmlElement *child = NULL;
	while ((child = (TiXmlElement *) children->IterateChildren(child))) {
		char child_type[MAX_STRING_LEN];
		strdup(child_type, child->Value());

		if (strcmp(child_type, "rectangle") == 0) {
			char tmp_str[MAX_STRING_LEN];
			double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
			if (strdup(tmp_str, child->Attribute("xy1")) == NULL) {
				printf("Error reading \"xy1\" attribute!\n");
				throw InvalidXMLException();
			}
			if (sscanf(tmp_str, "%lf %lf", &x1, &y1) != 2) {
				printf("Error parsing \"xy1\" attribute!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, child->Attribute("xy2")) == NULL) {
				printf("Error reading \"xy2\" attribute!\n");
				throw InvalidXMLException();
			}
			if (sscanf(tmp_str, "%lf %lf", &x2, &y2) != 2) {
				printf("Error parsing \"xy2\" attribute!\n");
				throw InvalidXMLException();
			}

			MyRectangle *rect = new MyRectangle(x1, y1, x2, y2);
			n->addPrimitive(rect);

			printf("Rectangle\nxy1: (%f,%f)\nxy2: (%f,%f)\n", x1, y1, x2, y2);

		} else if (strcmp(child_type, "triangle") == 0) {
			char tmp_str[MAX_STRING_LEN];
			double x1 = 0, x2 = 0, x3 = 0, y1 = 0, y2 = 0, y3 = 0, z1 = 0, z2 = 0, z3 = 0;
			if (strdup(tmp_str, child->Attribute("xyz1")) == NULL) {
				printf("Error reading \"xyz1\" attribute!\n");
				throw InvalidXMLException();
			}
			if (sscanf(tmp_str, "%lf %lf %lf", &x1, &y1, &z1) != 3) {
				printf("Error parsing \"xyz1\" attribute!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, child->Attribute("xyz2")) == NULL) {
				printf("Error reading \"xyz2\" attribute!\n");
				throw InvalidXMLException();
			}
			if (sscanf(tmp_str, "%lf %lf %lf", &x2, &y2, &z2) != 3) {
				printf("Error parsing \"xyz2\" attribute!\n");
				throw InvalidXMLException();
			}

			if (strdup(tmp_str, child->Attribute("xyz3")) == NULL) {
				printf("Error reading \"xyz3\" attribute!\n");
				throw InvalidXMLException();
			}
			if (sscanf(tmp_str, "%lf %lf %lf", &x3, &y3, &z3) != 3) {
				printf("Error parsing \"xyz3\" attribute!\n");
				throw InvalidXMLException();
			}

			MyTriangle *tri = new MyTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
			n->addPrimitive(tri);

			printf("Triangle\nxyz1: (%f,%f,%f)\nxyz2: (%f,%f,%f)\nxyz3: (%f,%f,%f)\n", x1, y1, z1, x2, y2, z2, x3, y3,
					z3);

		} else if (strcmp(child_type, "cylinder") == 0) {
			double cyl_base = 0, cyl_top = 0, cyl_height = 0;
			unsigned int cyl_slices = 0, cyl_stacks = 0;

			if (child->QueryDoubleAttribute("base", &cyl_base) != TIXML_SUCCESS) {
				printf("Error parsing base attribute!\n");
				throw InvalidXMLException();
			}

			if (child->QueryDoubleAttribute("top", &cyl_top) != TIXML_SUCCESS) {
				printf("Error parsing slices attribute!\n");
				throw InvalidXMLException();
			}

			if (child->QueryDoubleAttribute("height", &cyl_height) != TIXML_SUCCESS) {
				printf("Error parsing slices attribute!\n");
				throw InvalidXMLException();
			}

			if (child->QueryUnsignedAttribute("slices", &cyl_slices) != TIXML_SUCCESS) {
				printf("Error parsing slices attribute!\n");
				throw InvalidXMLException();
			}

			if (child->QueryUnsignedAttribute("stacks", &cyl_stacks) != TIXML_SUCCESS) {
				printf("Error parsing stacks attribute!\n");
				throw InvalidXMLException();
			}

			MyCylinder *cyl = new MyCylinder(cyl_base, cyl_top, cyl_height, cyl_slices, cyl_stacks);
			n->addPrimitive(cyl);

			printf("Cylinder\nbase: %f\ntop: %f\nheight: %f\nslices: %d\nstacks: %d\n", cyl_base, cyl_top, cyl_height,
					cyl_slices, cyl_stacks);

		} else if (strcmp(child_type, "sphere") == 0) {
			double sph_rad = 0;
			unsigned int sph_slices = 0, sph_stacks = 0;

			if (child->QueryDoubleAttribute("radius", &sph_rad) != TIXML_SUCCESS) {
				printf("Error parsing radius attribute!\n");
			}

			if (child->QueryUnsignedAttribute("slices", &sph_slices) != TIXML_SUCCESS) {
				printf("Error parsing slices attribute!\n");
			}

			if (child->QueryUnsignedAttribute("stacks", &sph_stacks) != TIXML_SUCCESS) {
				printf("Error parsing stacks attribute!\n");
			}

			MySphere *sph = new MySphere(sph_rad, sph_slices, sph_stacks);
			n->addPrimitive(sph);

			printf("Sphere\nradius: %f\nslices: %d\nstacks: %d\n", sph_rad, sph_slices, sph_stacks);

		} else if (strcmp(child_type, "torus") == 0) {
			double tor_inner = 0, tor_out = 0;
			unsigned int tor_slices = 0, tor_loops = 0;

			if (child->QueryDoubleAttribute("inner", &tor_inner) != TIXML_SUCCESS) {
				printf("Error parsing inner attribute!\n");
			}

			if (child->QueryDoubleAttribute("outer", &tor_out) != TIXML_SUCCESS) {
				printf("Error parsing outer attribute!\n");
			}

			if (child->QueryUnsignedAttribute("slices", &tor_slices) != TIXML_SUCCESS) {
				printf("Error parsing slices attribute!\n");
			}

			if (child->QueryUnsignedAttribute("loops", &tor_loops) != TIXML_SUCCESS) {
				printf("Error parsing loops attribute!\n");
			}

			MyTorus *tor = new MyTorus(tor_inner, tor_out, tor_slices, tor_loops);
			n->addPrimitive(tor);

			printf("Torus\ninner: %f\nouter: %f\nslices: %d\nloops: %d\n", tor_inner, tor_out, tor_slices, tor_loops);
		} else if (strcmp(child_type, "plane") == 0) {
			int parts = 0;

			if (child->QueryIntAttribute("parts", &parts) != TIXML_SUCCESS) {
				printf("Error parsing \"parts\" attribute on plane!\n");
				throw InvalidXMLException();
			}

			if (parts <= 0) {
				printf("Invalid value on parts attribute of plane!\n");
				throw InvalidXMLException();
			}

			printf("Plane\nparts: %i\n", parts);

			Plane *p = new Plane(parts);
			n->addPrimitive(p);
		} else if (strcmp(child_type, "patch") == 0) {
			int order = 0;
			int parts_u = 0;
			int parts_v = 0;
			char compute[MAX_STRING_LEN];

			if (child->QueryIntAttribute("order", &order) != TIXML_SUCCESS) {
				printf("Error parsing \"order\" attribute on patch!\n");
				throw InvalidXMLException();
			}

			if (child->QueryIntAttribute("partsU", &parts_u) != TIXML_SUCCESS) {
				printf("Error parsing \"partsU\" attribute on patch!\n");
				throw InvalidXMLException();
			}

			if (child->QueryIntAttribute("partsV", &parts_v) != TIXML_SUCCESS) {
				printf("Error parsing \"partsV\" attribute on patch!\n");
				throw InvalidXMLException();
			}

			if (strdup(compute, child->Attribute("compute")) == NULL) {
				printf("Error parsing \"compute\" attribute on patch!\n");
				throw InvalidXMLException();
			}

			printf("Patch\norder: %d\npartsU: %d\npartsV: %d\ncompute: %s\n", order, parts_u, parts_v, compute);

			MyPatch *patch = new MyPatch(order, parts_u, parts_v, compute);
			n->addPrimitive(patch);

			TiXmlElement *ctrl_p = child->FirstChildElement("controlpoint");
			do {
				float x = 0;
				float y = 0;
				float z = 0;

				if (ctrl_p->QueryFloatAttribute("x", &x) != TIXML_SUCCESS) {
					printf("Error parsing \"x\" attribute on patch!\n");
					throw InvalidXMLException();
				}

				if (ctrl_p->QueryFloatAttribute("y", &y) != TIXML_SUCCESS) {
					printf("Error parsing \"y\" attribute on patch!\n");
					throw InvalidXMLException();
				}
				if (ctrl_p->QueryFloatAttribute("z", &z) != TIXML_SUCCESS) {
					printf("Error parsing \"z\" attribute on patch!\n");
					throw InvalidXMLException();
				}

				printf("controlpoint: (%f %f %f)\n", x, y, z);

				patch->addControlPoint(x, y, z);

			} while ((ctrl_p = ctrl_p->NextSiblingElement("controlpoint")) != NULL);
		} else if (strcmp(child_type, "waterline") == 0) {
			char heightmap[MAX_STRING_LEN];
			char texturemap[MAX_STRING_LEN];
			char vert_shader[MAX_STRING_LEN];
			char frag_shader[MAX_STRING_LEN];

			if (is_inside_dl || is_dl) {
				printf("Waterline defined in \"%s\" is inside a display list!\n", node_id);
				throw InvalidXMLException();
			}

			if (strdup(heightmap, child->Attribute("heightmap")) == NULL) {
				printf("Error parsing \"heightmap\" attribute of waterline!");
				throw InvalidXMLException();
			}

			if (strdup(texturemap, child->Attribute("texturemap")) == NULL) {
				printf("Error parsing \"texturemap\" attribute of waterline!");
				throw InvalidXMLException();
			}

			if (strdup(frag_shader, child->Attribute("fragmentshader")) == NULL) {
				printf("Error parsing \"fragmentshader\" attribute of waterline!");
				throw InvalidXMLException();
			}

			if (strdup(vert_shader, child->Attribute("vertexshader")) == NULL) {
				printf("Error parsing \"vertexshader\" attribute of waterline!");
				throw InvalidXMLException();
			}

			MyWaterLine *wl = new MyWaterLine(heightmap, texturemap, vert_shader, frag_shader);
			n->addPrimitive(wl);

		} else if (strcmp(child_type, "vehicle") == 0) {
			MyVehicle *v = new MyVehicle();
			n->addPrimitive(v);

		} else if (strcmp(child_type, "noderef") == 0) {
			char next_node_id[MAX_STRING_LEN];
			if (strdup(next_node_id, child->Attribute("id")) == NULL) {
				printf("Error reading noderef's id!\n");
				throw InvalidXMLException();
			}
			if (find(nodes_being_processed.begin(), nodes_being_processed.end(), next_node_id)
					!= nodes_being_processed.end()) {
				if (find(nodes_finished_processing.begin(), nodes_finished_processing.end(), next_node_id)
						!= nodes_finished_processing.end()) {
					printf("Node has already been processed.\n");
					string last_node_name = Scene::getInstance()->findLastNameAvail(next_node_id);
					if (Scene::getInstance()->getNode(last_node_name)->getType() != DISPLAY_LIST) { // normal node
						n->addRef(next_node_id);
					} else {
						TiXmlElement *next_node = NULL;
						if ((next_node = findChildByAttribute(graphElement, "id", next_node_id))) {
							printf("\n\n");
							parseNode(next_node, is_dl || is_inside_dl);
							n->addRef(Scene::getInstance()->findLastNameAvail(next_node_id));
						}
					}
					continue;
				} else {
					printf("Cyclic definition found in node \"%s\"", next_node_id);
					throw InvalidXMLException();
				}
			} else {
				TiXmlElement *next_node = NULL;
				if ((next_node = findChildByAttribute(graphElement, "id", next_node_id))) {
					printf("\n\n");
					parseNode(next_node, is_dl || is_inside_dl);
					string last_node_name = Scene::getInstance()->findLastNameAvail(next_node_id);
					if (Scene::getInstance()->getNode(last_node_name)->getType() == DISPLAY_LIST) {
						n->addRef(last_node_name);
					} else {
						n->addRef(next_node_id);
					}

				} else {
					printf("Node %s does not exist!\n", next_node_id);
					throw InvalidXMLException();
				}
			}
		} else {
			printf("Invalid block inside children of node %s\n", node_id);
			throw InvalidXMLException();
		}
	}
	printf("Finished processing %s node children.\n\n", node_id);

	if (is_dl) {
		Scene::getInstance()->addNode(dl_node_id, n);
	} else {
		Scene::getInstance()->addNode(node_id, n);
	}
	printf("closing %s\n", node_id);
	n->closeDefinition(app_stck);
	printf("closed %s\n", node_id);
	app_stck.pop();
	nodes_finished_processing.push_back(node_id);
	return true;
}

void XMLScene::setPaths() {
	cout << "Insert path to yaf file: ";
	cin >> yaf_path;
	cout << endl << "Insert path to texture folder: ";
	cin >> texture_base_path;
	if (texture_base_path[texture_base_path.size() - 1] == '/') {
		texture_base_path = texture_base_path.substr(0, texture_base_path.size() - 1);
	}
}
