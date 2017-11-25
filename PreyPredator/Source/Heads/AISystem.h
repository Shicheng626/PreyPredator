#pragma once
#include "EntityManager.h"

class AISystem {
	int blockCount=0;
public:
	unsigned int GetClosestHeardTarget(EntityManager * entitymanager, unsigned int entity);
	unsigned int GetClosestSeenTarget(EntityManager * entitymanager, unsigned int entity);

	//bool IsChecked(EntityManager * entitymanager, unsigned int entity, unsigned int target);
	bool IsSeen(EntityManager * entitymanager, unsigned int entity, unsigned int target);

	void Update(EntityManager * entitymanager);
	void UpdateStatus(EntityManager * entitymanager, unsigned int entity);
	void UpdateAction(EntityManager * entitymanager, unsigned int entity);
	//void UpdateFocus(EntityManager * entitymanager, unsigned int entity);

//	void ClearCheckedEntities(EntityManager * entitymanager, unsigned int entity);

	void IdleAction(EntityManager * entitymanager, unsigned int entity);
	void SearchingAction(EntityManager * entitymanager, unsigned int entity);
	void ChasingAction(EntityManager * entitymanager, unsigned int entity);
	void EscapingAction(EntityManager * entitymanager, unsigned int entity);
};