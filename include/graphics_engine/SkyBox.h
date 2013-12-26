/*
 * SkyBox.h
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "CGFtexture.h"
#include "string"

class SkyBox {
private:
	string id;
	CGFtexture* SkyboxTexture[6];
public:
	SkyBox(string id, string folder);
	void loadSkybox(string prefix);
	void draw();
	string getId();
	virtual ~SkyBox();
};

#endif /* SKYBOX_H_ */
