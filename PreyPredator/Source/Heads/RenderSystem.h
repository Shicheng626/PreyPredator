#pragma once

#include "AntTweakBar.h"
#include "Scene.h"
#include "EntityManager.h"

class RenderSystem{


public:
	Scene* scene;


	float currentTime;
	float FPS;
	float LastTime;

	void SetMaterial(Material material);

	void lightsInitialize();

	void lightsPosition();

	void DrawMesh(Mesh mesh, TransformComponent transform, Material material);

	void SetupScene();

	void SetProjectionMatrix();

	void SetViewMatrix();

	void DrawGround(EntityManager* entitymanager);

	void DrawWalls(EntityManager* entitymanager);

	void DrawSensingLine(EntityManager* entitymanager);

	void DrawLine(vmath::vec3 point1, vmath::vec3 point2, vmath::vec4 color);

	void Update(EntityManager* entitymanager);

	void GetFPS();

	void reset();
};

