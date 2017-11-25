#pragma once
#include "EntityManager.h"

class SensingSystem {
public:
	void Update(EntityManager * entitymanager);
	void UpdateHeardList(EntityManager * entitymanager, unsigned int entity);
	void UpdateSawList(EntityManager * entitymanager, unsigned int entity);
	bool RayCheck(EntityManager * entitymanager,vmath::vec3 source,vmath::vec3 target);
};