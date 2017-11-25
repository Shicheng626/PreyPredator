#include "../Heads/Scene.h"


void Camera::init() {
	near_ = std::min(0.1, eye[2] + 0.1);
	far_ = std::max(50.0, eye[2] + 40.0);
	Reset();
}

void Camera::Reset() {
	Rotation = vmath::vec3(-60, 0, 0);
	Translation = vmath::vec3(0, 0, 25);
	u = vmath::vec3(1, 0, 0);
	v = vmath::vec3(0, 1, 0);
	w = vmath::vec3(0, 0, 1);
	eye = vmath::vec3(0, 0, 4.5);

	LastRotation = vmath::vec3(0, 0, 0);
	LastTranslation = vmath::vec3(0, 0, 0);
	M = vmath::mat4::identity();
}

void Camera::update() {
	if (Rotation[0] != LastRotation[0] || Rotation[1] != LastRotation[1] || Rotation[2] != LastRotation[2]) {
		vmath::mat4 rt = vmath::mat4::identity();
		if (Rotation[0] != LastRotation[0]) {
			rt = vmath::rotate(-Rotation[0] + LastRotation[0], vmath::vec3(1, 0, 0));
		}
		else if (Rotation[1] != LastRotation[1]) {
			rt = vmath::rotate(-Rotation[1] + LastRotation[1], vmath::vec3(0, 1, 0));
		}
		else if (Rotation[2] != LastRotation[2]) {
			rt = vmath::rotate(-Rotation[2] + LastRotation[2], vmath::vec3(0, 0, 1));
		}
		M = rt * M;
		LastRotation = Rotation;
	}
	if (Translation[0] != LastTranslation[0] || Translation[1] != LastTranslation[1] || Translation[2] != LastTranslation[2]) {
		vmath::vec3 tt = vmath::vec3(0, 0, 0);
		if (Translation[0] != LastTranslation[0]) {
			tt = -(Translation[0] - LastTranslation[0]) * u;
		}
		else if (Translation[1] != LastTranslation[1]) {
			tt = -(Translation[1] - LastTranslation[1]) * v;
		}
		else if (Translation[2] != LastTranslation[2]) {
			tt = -(Translation[2] - LastTranslation[2]) * w;
		}
		vmath::mat4 mt = vmath::translate(tt);
		M = mt * M;
		LastTranslation = Translation;
	}
}

void Scene::Initialize() {
	usingCW = false; // using GL_CW
	prim = SOLID;
	shading = SMOOTH;
	bReset = false;
	MyCamera.init();
	Reset();
}



void Scene::Reset() {

	MyCamera.Reset();

	lightson = true;

}
