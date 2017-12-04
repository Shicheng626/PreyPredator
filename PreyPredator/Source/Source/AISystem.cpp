#include "../Heads/AISystem.h"
#include <time.h>

unsigned int AISystem::GetClosestHeardTarget(EntityManager * entitymanager, unsigned int entity)
{
	float ShortestDistance = 9999999;
	unsigned int result = entity;
	for (int i = 0; i < entitymanager->SensingList[entity]->HeardEntities.size(); i++) {
		unsigned int Target = entitymanager->SensingList[entity]->HeardEntities[i];
		//if (IsChecked(entitymanager, entity, i) != true) {
			vmath::vec3 Distance = entitymanager->TransformList[entity]->Translation - entitymanager->TransformList[Target]->Translation;
			float Distancef = Distance[0] * Distance[0] + Distance[1] * Distance[1];
			if (Distancef < ShortestDistance) {
				ShortestDistance = Distancef;
				result = Target;

			}
	//	}
	}
	return result;
}

unsigned int AISystem::GetClosestSeenTarget(EntityManager * entitymanager, unsigned int entity)
{
	float ShortestDistance = 9999999;
	unsigned int result = entity;
	for (int i = 0; i < entitymanager->SensingList[entity]->SeenEntities.size(); i++) {
		unsigned int Target = entitymanager->SensingList[entity]->SeenEntities[i];
		if (entitymanager->AIList[Target]!=nullptr){
			if (entitymanager->AIList[entity]->type != entitymanager->AIList[Target]->type) {
				vmath::vec3 Distance = entitymanager->TransformList[entity]->Translation - entitymanager->TransformList[Target]->Translation;
				float Distancef = Distance[0] * Distance[0] + Distance[1] * Distance[1];
				if (Distancef < ShortestDistance) {
					ShortestDistance = Distancef;
					result = Target;
				}
			}
		}
		
	}
	return result;
}

/*bool AISystem::IsChecked(EntityManager * entitymanager, unsigned int entity, unsigned int target)
{

	for (int i = 0; i < entitymanager->AIList[entity]->CheckedEntities.size(); i++) {
		if (entitymanager->AIList[entity]->CheckedEntities[i] == target) {
			return true;
		}
		else {
			return false;
		}
	}
}*/

bool AISystem::IsSeen(EntityManager * entitymanager, unsigned int entity, unsigned int target)
{
	for (int i = 0; i < entitymanager->SensingList[entity]->SeenEntities.size(); i++) {
		if (entitymanager->SensingList[entity]->SeenEntities[i] == target) {
			return true;
		}
		else {
			return false;
		}
	}
}

void AISystem::Update(EntityManager * entitymanager)
{
	for (int entity = 0; entity < entitymanager->AIList.size(); entity++) {
		if (entitymanager->AIList[entity] != nullptr) {
			UpdateStatus(entitymanager, entity);
			UpdateAction(entitymanager, entity);
		}
	}
}

void AISystem::UpdateStatus(EntityManager * entitymanager, unsigned int entity)
{
	if (blockCount>10) {
		AIComponent::AISatus newStatus;
		if (entitymanager->SensingList[entity]->SeenEntities.size() > 0) {
			if (entitymanager->AIList[entity]->type == AIComponent::Predator) {
				newStatus = AIComponent::Chasing;
			}
			else if (entitymanager->AIList[entity]->type == AIComponent::Prey) {
				newStatus = AIComponent::Escaping;
			}
		}
		else if (entitymanager->SensingList[entity]->HeardEntities.size() > 0 && entitymanager->AIList[entity]->type == AIComponent::Predator) {
			newStatus = AIComponent::Searching;
		}
		else {
			newStatus = AIComponent::Idle;
		}
		if (newStatus != entitymanager->AIList[entity]->status) {
			entitymanager->MovementList[entity]->PathToTarget.clear();
			entitymanager->AIList[entity]->status = newStatus;
		}
		blockCount = 0;
	}
	blockCount++;
}

void AISystem::UpdateAction(EntityManager * entitymanager, unsigned int entity)
{
	switch (entitymanager->AIList[entity]->status)
	{
	case AIComponent::Idle:
		IdleAction(entitymanager, entity);
		break;
	case AIComponent::Searching:
		SearchingAction(entitymanager, entity);
		break;
	case AIComponent::Chasing:
		ChasingAction(entitymanager, entity);
		break;
	case AIComponent::Escaping:
		EscapingAction(entitymanager, entity);
		break;
	default:
		break;
	}
}

void AISystem::IdleAction(EntityManager * entitymanager, unsigned int entity)
{
	srand(time(0)+ entity);
	entitymanager->MovementList[entity]->speed = 0.05f;
	if (entitymanager->MovementList[entity]->PathToTarget.size() > 0) {
		return;
	}
	else {
		float x = rand() % 40 - 20;
		float y = rand() % 40 - 20;
		entitymanager->MovementList[entity]->TargetPosition = { x,0,y };
		entitymanager->MovementList[entity]->DoPathFinding = true;
	}
}

void AISystem::SearchingAction(EntityManager * entitymanager, unsigned int entity)
{
	entitymanager->MovementList[entity]->speed = 0.05f;
	if (entitymanager->SensingList[entity]->HeardEntities.size() > 0) {
		unsigned int old = entitymanager->AIList[entity]->Focus;
		unsigned int target = GetClosestHeardTarget(entitymanager, entity);

		if (entitymanager->MovementList[entity]->PathToTarget.size() == 0|| old!= target) {

			entitymanager->MovementList[entity]->TargetPosition = entitymanager->TransformList[target]->Translation;
			entitymanager->MovementList[entity]->DoPathFinding = true;
			entitymanager->AIList[entity]->Focus = target;
		}
	}
	else {
		return;
	}
}

void AISystem::ChasingAction(EntityManager * entitymanager, unsigned int entity)
{
	if (entitymanager->SensingList[entity]->SeenEntities.size() > 0) {
		unsigned int target = GetClosestSeenTarget(entitymanager, entity);
		unsigned int old = entitymanager->AIList[entity]->Focus;
	//	if (old != target) {
		if (target!=entity) {
			entitymanager->MovementList[entity]->speed = 0.07f;
			entitymanager->MovementList[entity]->PathToTarget.clear();
			entitymanager->MovementList[entity]->PathToTarget.push_back(entitymanager->TransformList[target]->Translation);
			entitymanager->AIList[entity]->Focus = target;
			//	}
		}
	}
}

void AISystem::EscapingAction(EntityManager * entitymanager, unsigned int entity)
{
	srand(time(0)+ entity);
	entitymanager->MovementList[entity]->speed = 0.05f;
	if (entitymanager->SensingList[entity]->SeenEntities.size() > 0) {
		unsigned int old = entitymanager->AIList[entity]->Focus;
		unsigned int target = GetClosestSeenTarget(entitymanager, entity);
		if (entitymanager->MovementList[entity]->PathToTarget.size() == 0 || old != target) {
			float targetX = entitymanager->TransformList[target]->Translation[0];
			float targetY = entitymanager->TransformList[target]->Translation[2];
			float selfX = entitymanager->TransformList[entity]->Translation[0];
			float selfY = entitymanager->TransformList[entity]->Translation[2];

			int Xindex = selfX + 0.5 + 20 + 1;
			int x;

			if (targetX > selfX) {

				x = rand() % Xindex;
			}
			else if (targetX < selfX) {

				x = rand() % (41 - Xindex) + Xindex;
			}
			else {
				x = rand() % 40;
			}

			int Yindex = selfY + 0.5 + 20 + 1;
			int y;

			if (targetY > selfY) {

				y = rand() % Yindex;
			}
			else if (targetY < selfY) {

				y = rand() % (41 - Yindex) + Yindex;
			}
			else {
				y = rand() % 40;
			}
			float xf = entitymanager->map[x][y].x;
			float yf = entitymanager->map[x][y].y;
			entitymanager->MovementList[entity]->TargetPosition = { xf, 0,yf };
			entitymanager->MovementList[entity]->DoPathFinding = true;
			entitymanager->AIList[entity]->Focus = target;
		}
	}
}
