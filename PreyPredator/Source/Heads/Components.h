#pragma once

#include <vector>
#include <list>
#include "vmath.h"

struct Material {
public:
	vmath::vec4 Ambient;
	vmath::vec4 Diffuse;
	vmath::vec4 Specular;

	float Shine;

	Material() {
		Ambient = vmath::vec4(0, 0.5, 0, 1);
		Diffuse = vmath::vec4(0, 0.5, 0, 1);
		Specular = vmath::vec4(0, 0, 0, 1);
		Shine = 1.0;
	}
};

class StaticMaterial {
public:
	Material Pearl;
	Material Rock;
	Material Brass;
	Material Ruby;
	Material Emerald;

	void Initialize();
};

struct Mesh {
public:
	std::vector<vmath::vec3> Vetices;
	std::vector<vmath::vec3> Faces;
	std::vector<vmath::vec3> Normals;
};

class StaticMesh {

public:
	Mesh Cube;
	Mesh Plane;
	Mesh Pawn;


	void InitializeNormal(Mesh* mesh);

	void Initialize();
};

struct RenderComponent {
public:
	Mesh mesh;
	Material material;
};


struct TransformComponent
{
public:
	vmath::vec3 Rotation = {0,0,0};
	vmath::vec3 Scale = { 1,1,1 };
	vmath::vec3 Translation = { 0,0,0 };
};

struct PathFindingNode {
	enum PFFlag {
		NotVisited,Open,Closed
	};
public:
	PathFindingNode* Parent;
	float x;
	float y;
	bool block;
	PFFlag flag= NotVisited;//0:not visited 1:open 2:closed
	float H=9999999999.0f;
	float G=9999999999.0f;
};


struct MovementComponent
{
public:
	float speed = 0.05f;

	bool DoPathFinding = true;

	vmath::vec3 Velocity = {0.0f,0.0f,0.0f };

	vmath::vec3 TargetPosition;

	std::vector<vmath::vec3> PathToTarget;

	MovementComponent() {
		std::vector<vmath::vec2> PathToTarget(0);
		speed = 0.05f;
		Velocity = { 0.0f,0.0f,0.0f };
	}
};

struct SphereColliderComponent {
public:
	float Radius = 0.3f;
};

struct SensingComponent {
	float RadiusSquare = 100.0;
	float ViewAngle = 100.0f;
	std::vector<unsigned int> HeardEntities;
	std::vector<unsigned int> SeenEntities;


};

struct AIComponent {
enum AIType {
	Prey = 0,
	Predator = 1
};
enum AISatus {
	Idle,
	Searching,
	Chasing,
	Escaping
};
public:
	AIType type;
	AISatus status;
	unsigned int Focus;
	bool SawEntity = false;
//	std::vector<unsigned int> CheckedEntities;
};
