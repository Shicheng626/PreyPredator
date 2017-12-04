#include "../Heads/SensingSystem.h"

void SensingSystem::Update(EntityManager * entitymanager)
{
	for (int entity = 0; entity < entitymanager->SensingList.size();entity++) {
		if (entitymanager->SensingList[entity]!= nullptr) {

			entitymanager->SensingList[entity]->HeardEntities.clear();
			entitymanager->SensingList[entity]->SeenEntities.clear();
			UpdateHeardList(entitymanager, entity);
			UpdateSeenList(entitymanager, entity);
		
		}
	}
}

void SensingSystem::UpdateHeardList(EntityManager * entitymanager, unsigned int entity)
{
	for (int i = 0; i < entitymanager->AIList.size(); i++) {
		if (entitymanager->AIList[i] != nullptr && i != entity) {
			if (entitymanager->AIList[i]->type!= entitymanager->AIList[entity]->type) {
				vmath::vec3 Distance = entitymanager->TransformList[i]->Translation - entitymanager->TransformList[entity]->Translation;
				float DistanceSquare = Distance[0] * Distance[0] + Distance[2] * Distance[2];
				if (DistanceSquare <= entitymanager->SensingList[entity]->RadiusSquare) {
					entitymanager->SensingList[entity]->HeardEntities.push_back(i);
				}
			}
		}
	}
}

void SensingSystem::UpdateSeenList(EntityManager * entitymanager, unsigned int entity)
{
	if (entitymanager->SensingList[entity]->HeardEntities.size() > 0) {
		for (int i = 0; i < entitymanager->SensingList[entity]->HeardEntities.size(); i++) {
			int TargetEntity = entitymanager->SensingList[entity]->HeardEntities[i];
			if (RayCheck(entitymanager,entitymanager->TransformList[entity]->Translation, entitymanager->TransformList[TargetEntity]->Translation)) {
				entitymanager->SensingList[entity]->HeardEntities.erase(entitymanager->SensingList[entity]->HeardEntities.begin() + i);
				i--;
				entitymanager->SensingList[entity]->SeenEntities.push_back(TargetEntity);
			}
		}
	}
}

bool SensingSystem::RayCheck(EntityManager * entitymanager, vmath::vec3 source, vmath::vec3 target)
{
	vmath::vec3 Dir = target - source;
	int CurrentNodeIndexX = source[0] + 20 + 0.5;
	int CurrentNodeIndexY = source[2] + 20 + 0.5;
	float CurrentPositionX = source[0];
	float CurrentPositionY = source[2];
	PathFindingNode* CurrentNode = &entitymanager->map[CurrentNodeIndexX][CurrentNodeIndexY];
	for (float t = 0; t <= 1; t += 0.01) {
		if (CurrentNode->block == true) {
			return false;
		}
		CurrentPositionX = t* Dir[0] + source[0];
		CurrentPositionY = t* Dir[2] + source[2];
		CurrentNodeIndexX = CurrentPositionX + 20 + 0.5;
		CurrentNodeIndexY = CurrentPositionY + 20 + 0.5;
		CurrentNode = &entitymanager->map[CurrentNodeIndexX][CurrentNodeIndexY];
	}
	return true;


/*

	int CurrentNodeIndexX = source[0] + 20 + 0.5;
	int CurrentNodeIndexY = source[2] + 20 + 0.5;
	int TargetNodeXIndex = target[0] + 20 + 0.5;
	int TargetNodeYIndex = target[2] + 20 + 0.5;
	float CurrentPositionX = source[0];
	float CurrentPositionY = source[2];

	PathFindingNode* CurrentNode = &entitymanager->map[CurrentNodeIndexX][CurrentNodeIndexY];
	PathFindingNode* TargetNode = &entitymanager->map[TargetNodeXIndex][TargetNodeYIndex];
	while (CurrentNode!= TargetNode) {
		float dx, dy,dt;
		if (Dir[0] > 0) {
			dx = abs(CurrentNode->x + 0.5 - CurrentPositionX);
		}
		else {
			dx = abs(CurrentNode->x - 0.5 - CurrentPositionX);
		}
		if (Dir[2] > 0) {
			dy = abs(CurrentNode->y + 0.5 - CurrentPositionY);
		}
		else {
			dy = abs(CurrentNode->y - 0.5 - CurrentPositionY);
		}

		if (dx<dy) {
			dt = dx/CosDir;
		}
		else {
			dt = dy/SinDir;
		}
		CurrentPositionX = dt*CosDir + CurrentPositionX;
		CurrentPositionY = dt*SinDir + CurrentPositionY;
	    CurrentNodeIndexX = CurrentPositionX + 20 + 0.5;
		CurrentNodeIndexY = CurrentPositionY + 20 + 0.5;
		CurrentNode = &entitymanager->map[CurrentNodeIndexX][CurrentNodeIndexY];
		if (CurrentNode->block == true) {
			return false;
		}
	}
	return true;
/*	int SourceNodeXIndex = source[0] + 20 + 0.5;
	int SourceNodeYIndex = source[2] + 20 + 0.5;
	int TargetNodeXIndex = target[0] + 20 + 0.5;
	int TargetNodeYIndex = target[2] + 20 + 0.5;
	vmath::vec3 Ray = target - source;
	PathFindingNode SourceNode = entitymanager->map[SourceNodeXIndex][SourceNodeYIndex];
	PathFindingNode TargetNode = entitymanager->map[TargetNodeXIndex][TargetNodeYIndex];
	int DeltaX = TargetNodeXIndex - SourceNodeXIndex;
	if (DeltaX>=0) {
		int LastcheckNodeYindex =  SourceNodeYIndex;
		for (int i = 0; i < DeltaX; i++) {
			float t = (SourceNode.x + 0.5 + i - source[0]) / DeltaX;
			vmath::vec3 CheckingPoint = t * Ray + source;
			int checkNodeYindex = CheckingPoint[2] + 20 + 0.5;
			if (entitymanager->map[SourceNodeXIndex + i][checkNodeYindex].block || entitymanager->map[SourceNodeXIndex + i + 1][checkNodeYindex].block) {
				return false;
			}
			if (checkNodeYindex - LastcheckNodeYindex >= 0) {
				for (int j = 0; j < checkNodeYindex - LastcheckNodeYindex; j++) {
					if (entitymanager->map[SourceNodeXIndex + i][checkNodeYindex + j].block) {
						return false;
					}
				}
			}
			else {
				for (int j = 0; j > checkNodeYindex - LastcheckNodeYindex; j--) {
					if (entitymanager->map[SourceNodeXIndex + i][checkNodeYindex + j].block) {
						return false;
					}
				}
			}
			LastcheckNodeYindex = checkNodeYindex;
		}
		return true;
	}
	else {
		int LastcheckNodeYindex = SourceNodeYIndex;
		for (int i = 0; i > DeltaX; i--) {
			float t = (SourceNode.x - 0.5 + i - source[0]) / DeltaX;
			vmath::vec3 CheckingPoint = t * Ray + source;
			int checkNodeYindex = CheckingPoint[2] + 20 + 0.5;

			if (entitymanager->map[SourceNodeXIndex + i][checkNodeYindex].block || entitymanager->map[SourceNodeXIndex + i - 1][checkNodeYindex].block) {
				return false;
			}
			if (checkNodeYindex - LastcheckNodeYindex>=0) {
				for (int j = 0; j < checkNodeYindex - LastcheckNodeYindex; j++) {
					if (entitymanager->map[SourceNodeXIndex + i][checkNodeYindex + j].block) {
						return false;
					}
				}
			}
			else {
				for (int j = 0; j > checkNodeYindex - LastcheckNodeYindex; j--) {
					if (entitymanager->map[SourceNodeXIndex + i][checkNodeYindex + j].block) {
						return false;
					}
				}
			}
			LastcheckNodeYindex = checkNodeYindex;
		}
		return true;
	}*/
}

