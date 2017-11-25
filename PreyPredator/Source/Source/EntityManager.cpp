#include "../Heads/EntityManager.h"
#include <ctime>

void EntityManager::InitializeComponentLists()
{
	FlagList = std::vector<unsigned int>(1, C_None);
	RenderList = std::vector<RenderComponent*>(1);
	TransformList = std::vector<TransformComponent*>(1);
	MovementList = std::vector<MovementComponent*>(1);
}

void EntityManager::Initialize() {
	srand(time(0));
	MeshData.Initialize();
	MaterialData.Initialize();
	InitializeComponentLists();
	GeneratePathFindingMap();
	ResetPathfindingMap();
	int i = 2;
	while (i>0) {
		int xi = rand() % 40;
		int yi = rand() % 40;
		if (map[xi][yi].block != true) {
			TransformComponent* trans = new TransformComponent();
			trans->Translation[0] = map[xi][yi].x;
			trans->Translation[2] = map[xi][yi].y;
			SpawnCreature(trans, AIComponent::Predator);
			i--;
		}
	}
}

void EntityManager::ResetPathfindingMap()
{
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++)
		{
			map[i][j].G = 9999999;
			map[i][j].H = 9999999;
			map[i][j].Parent = NULL;
			map[i][j].flag = PathFindingNode::NotVisited;
		}
	}
}



void EntityManager::GeneratePathFindingMap()
{
	srand(time(0));
	ResetPathfindingMap();
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++)
		{
			map[i][j].x = i - 20;
			map[i][j].y = j - 20;
			map[i][j].block = false;
			int ran = rand() % 8;
			if (ran == 0) {
				map[i][j].block = true;
			}
		}
	}
}

void EntityManager::Update() {
	PreySpawner();
}

unsigned int EntityManager::GetEmptyFlag()
{
	for (int i = 0; i < FlagList.size(); i++) {
		if (FlagList[i] == C_None) {
			return i;
		}
	}

	FlagList.push_back(C_None);
	return FlagList.size() - 1;
}

unsigned int EntityManager::CreatEntity()
{
	int Entity;
	Entity = GetEmptyFlag();
	return Entity;

}

void EntityManager::DestoryEntity(unsigned int entity)
{
	FlagList[entity] = C_None;
	delete RenderList[entity];
	RenderList[entity] = nullptr;
	delete TransformList[entity];
	TransformList[entity] = nullptr;
	delete MovementList[entity];
	MovementList[entity] = nullptr;
	delete SensingList[entity];
	SensingList[entity] = nullptr;
	delete AIList[entity];
	AIList[entity] = nullptr;
	delete ColliderList[entity];
	ColliderList[entity] = nullptr;


}

unsigned int EntityManager::SpawnCreature(TransformComponent* trans, AIComponent::AIType type)
{
	int Pawn;
	if (type == AIComponent::Predator) {
		Pawn = SpawnObject(CreatEntity(), MeshData.Pawn, MaterialData.Ruby, trans);
		AddAI(Pawn, type);
	}
	if (type == AIComponent::Prey) {
		Pawn = SpawnObject(CreatEntity(), MeshData.Pawn, MaterialData.Pearl, trans);
		AddAI(Pawn, type);
	}
	AddMovementComponent(Pawn);
	AddCollider(Pawn);

	return Pawn;
}

unsigned int EntityManager::SpawnObject(unsigned int entity, Mesh mesh, Material material, TransformComponent * trans)
{
	if (RenderList.size() <= entity) {
		RenderList.resize(entity + 1);
	}
	RenderComponent* render = new RenderComponent();
	render->mesh = mesh;
	render->material = material;
	RenderList[entity] = render;

	if (TransformList.size() <= entity) {
		TransformList.resize(entity + 1);
	}
	TransformList[entity] = trans;

	FlagList[entity] = C_Render | C_Transform;

	return entity;

}

unsigned int EntityManager::AddMovementComponent(unsigned int entity)
{
	MovementComponent* movement = new MovementComponent();
	movement->TargetPosition = { -20,0.0,-20 };
	if (MovementList.size() <= entity) {
		MovementList.resize(entity + 1);
	}
	MovementList[entity] = movement;
	FlagList[entity] = FlagList[entity] | C_Movement;
	return entity;
}

unsigned int EntityManager::AddAI(unsigned int entity,AIComponent::AIType type)
{
	AIComponent* AI = new AIComponent();
	AI->type = type;
	SensingComponent* sensing = new SensingComponent();
	if (AIList.size() <= entity) {
		AIList.resize(entity + 1);
	}
	AIList[entity] = AI;
	FlagList[entity] = FlagList[entity] | C_AI;

	if (SensingList.size() <= entity) {
		SensingList.resize(entity + 1);
	}
	SensingList[entity] = sensing;
	FlagList[entity] = FlagList[entity] | C_Sensing;

	return entity;

}

unsigned int EntityManager::AddCollider(unsigned int entity)
{
	SphereColliderComponent* collider = new SphereColliderComponent();
	if (ColliderList.size() <= entity) {
		ColliderList.resize(entity + 1);
	}
	ColliderList[entity] = collider;
	FlagList[entity] = FlagList[entity] | C_Collider;
	return entity;
}

unsigned int EntityManager::PreySpawner()
{
	srand(time(0));
	if (SpawnTimer == 240) {
		SpawnTimer = 0;

		int xi = rand() % 40;
		int yi = rand() % 40;
		if (map[xi][yi].block != true) {
			TransformComponent* trans = new TransformComponent();
			trans->Translation[0] = map[xi][yi].x;
			trans->Translation[2] = map[xi][yi].y;
			return SpawnCreature(trans, AIComponent::Prey);
		}
	}
	SpawnTimer++;
	return NULL;
}



