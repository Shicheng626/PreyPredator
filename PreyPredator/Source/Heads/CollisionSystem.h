#pragma once
#include "EntityManager.h"

class CollisionSystem {
public:
	void CheckBlock(EntityManager * entitymanager, unsigned int entity);
	void Update(EntityManager * entitymanager);
	void CollisionEvent(EntityManager * entitymanager, unsigned int entity, unsigned int OtherEntity);
};
