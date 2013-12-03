#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>

using namespace std;

/**
 * class containing the general attributes of a camera
 */
class Camera {
protected:
	string id;
	float near;
	float far;

public:
	Camera();
	Camera(string id);
	void setNear(float near);
	void setFar(float far);
	string getId();
	float getNear();
	float getFar();
	virtual void setCamera();
	virtual ~Camera();
};

#endif /* CAMERA_H_ */
