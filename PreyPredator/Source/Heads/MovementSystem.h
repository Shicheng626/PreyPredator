#pragma once
#include "EntityManager.h"
#include "scene.h"


class MovementSystem {
protected:
	std::vector<PathFindingNode*> OpenList;
	int PathFindingFailedCount;
public:
	void Initialize(EntityManager* entitymanager);

	void Update(EntityManager * entitymanager);

	void UpdateOrientation(EntityManager * entitymanager, unsigned int entity);

	void UpdatePosition(EntityManager * entitymanager, unsigned int entity);

	void UpdateVelocity(EntityManager * entitymanager, unsigned int entity);

	void SimpleMoveTo(EntityManager * entitymanager, unsigned int entity, vmath::vec3 Target);

	bool AstarPathFinding(EntityManager * entitymanager, unsigned int entity, vmath::vec3 Target);

	bool PathFindingRecursion(EntityManager * entitymanager, PathFindingNode* currentNode,int entity, PathFindingNode* TargetNode);

	void MakePath(EntityManager * entitymanager, unsigned int entity,PathFindingNode * TargetNode);


};