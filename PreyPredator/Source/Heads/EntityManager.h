#pragma once

#include <cmath>
#include <vector>
#include <array>

#include "vmath.h"

#include "Components.h"

using namespace std;

typedef enum
{
	C_None = 0,
	C_Render = 1 << 0,
	C_Transform = 1 << 1,
	C_Movement = 1 << 2,
	C_Collider = 1<<3,
	C_AI = 1<<4,
	C_Sensing = 1<<5
}ComponentFlag;

class EntityManager {
	int SpawnTimer;

public:
	StaticMesh MeshData;
	StaticMaterial MaterialData;

	PathFindingNode map[40][40];

	std::vector<unsigned int> FlagList;
	std::vector<RenderComponent*> RenderList;
	std::vector<TransformComponent*> TransformList;
	std::vector<MovementComponent*> MovementList;
	std::vector<SensingComponent*> SensingList;
	std::vector<AIComponent*> AIList;
	std::vector<SphereColliderComponent*> ColliderList;
	
	void InitializeComponentLists();

	void Initialize();

	void ResetPathfindingMap();

	void GeneratePathFindingMap();

	void Update();

	unsigned int GetEmptyFlag();

	unsigned int CreatEntity();

	void DestoryEntity(unsigned int entity);

	unsigned int SpawnCreature(TransformComponent* trans, AIComponent::AIType type);

	unsigned int SpawnObject(unsigned int entity, Mesh mesh, Material material, TransformComponent* trans);

	unsigned int AddMovementComponent(unsigned int entity);

	unsigned int AddAI(unsigned int entity, AIComponent::AIType type);

	unsigned int AddCollider(unsigned int entity);

	unsigned int PreySpawner();
};