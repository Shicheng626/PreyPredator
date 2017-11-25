#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <vector>
#include <array>

//*** In this part make sure the include directory contain those head files ***//
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glu.h"

#include "vmath.h"

using namespace std;

typedef enum { PTS, WIREFRAME, SOLID } primitive;
typedef enum { FLAT, SMOOTH } shademode;

class Camera {

public:
	vmath::vec3 Rotation; // This is rotation for camera
	vmath::vec3 Translation; // This is translation for camera
	float near_;
	float far_;
	vmath::vec3 LastRotation;
	vmath::vec3 LastTranslation;
	// transfromation
	vmath::vec3 eye;
	vmath::vec3 u;
	vmath::vec3 v;
	vmath::vec3 w;
	//vmath::mat4 Mc;
	vmath::mat4 M;
	void init();

	void Reset();

	void update();
};


class Scene {
public:
	bool usingCW;
	primitive prim;
	shademode shading;

	bool bReset;
	Camera MyCamera;



	// light para
	bool lightson;

	Scene() {
		Initialize();
	}

	void Initialize();



	void Reset();
};
