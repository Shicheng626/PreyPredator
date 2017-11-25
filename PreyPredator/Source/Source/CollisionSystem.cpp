#include "../Heads/CollisionSystem.h"



void CollisionSystem::Update(EntityManager * entitymanager)
{
	for (int entity = 0; entity < entitymanager->ColliderList.size(); entity++) {
		if (entitymanager->ColliderList[entity] != nullptr&&entitymanager->TransformList[entity]!=nullptr) {
			CheckBlock(entitymanager, entity);
			for (int i = 0; i < entitymanager->ColliderList.size(); i++) {
				if (entitymanager->ColliderList[i] != nullptr&&entitymanager->TransformList[entity] != nullptr) {
					if (i != entity) {
						vmath::vec3 distance = entitymanager->TransformList[entity]->Translation - entitymanager->TransformList[i]->Translation;
						float distancef = distance[0] * distance[0] + distance[2] * distance[2];
						if (distancef <= (entitymanager->ColliderList[entity]->Radius + entitymanager->ColliderList[i]->Radius)*(entitymanager->ColliderList[entity]->Radius + entitymanager->ColliderList[i]->Radius)) {
							CollisionEvent(entitymanager, entity, i);
						}
					}
				}
			}
		}
	}
}

void CollisionSystem::CollisionEvent(EntityManager * entitymanager, unsigned int entity,unsigned int OtherEntity)
{
	if (entitymanager->AIList[entity]->type!= entitymanager->AIList[OtherEntity]->type) {
		if (entitymanager->AIList[entity]->type == AIComponent::Prey) {
			entitymanager->DestoryEntity(entity);
		}
		else if (entitymanager->AIList[OtherEntity]->type == AIComponent::Prey) {
			entitymanager->DestoryEntity(OtherEntity);
		}
	}
}

void CollisionSystem::CheckBlock(EntityManager * entitymanager, unsigned int entity)
{
	vmath::vec3 pos = entitymanager->TransformList[entity]->Translation;


	int xi = pos[0] + 20 + 0.5;
	int yi = pos[2] + 20 + 0.5;

	vmath::vec3 nodepos = { entitymanager->map[xi][yi].x, 0.0, entitymanager->map[xi][yi].y };

	for (int i = 0; i < 6; i++) {
		vmath::vec3 CheckPoint;
		CheckPoint[0] = entitymanager->ColliderList[entity]->Radius * cosf(i * 60 * 0.0174532925f) + pos[0];
		CheckPoint[2] = entitymanager->ColliderList[entity]->Radius * sinf(i * 60 * 0.0174532925f) + pos[2];
		int tx = CheckPoint[0] + 20 + 0.5;
		int ty = CheckPoint[2] + 20 + 0.5;
		if (entitymanager->map[tx][ty].block) {

			vmath::vec3 velocity = vmath::normalize(entitymanager->MovementList[entity]->Velocity);
			vmath::vec3 BlockPos = { entitymanager->map[tx][ty].x,0.0,entitymanager->map[tx][ty].y };
			vmath::vec3 Dir = BlockPos - pos;
			Dir = vmath::normalize(Dir);




			vmath::vec3 Delta = Dir - velocity;

			if (Delta[0] * Delta[0] + Delta[2] * Delta[2] < 0.01) {
				return;// prevent stun;
			/*	entitymanager->MovementList[entity]->PathToTarget.clear();
				vmath::vec3 Up = { entitymanager->map[xi][yi - 1].x, 0.0, entitymanager->map[xi][yi - 1].y };
				vmath::vec3 Down = { entitymanager->map[xi][yi + 1].x, 0.0, entitymanager->map[xi][yi + 1].y };
				vmath::vec3 Left = { entitymanager->map[xi-1][yi].x, 0.0, entitymanager->map[xi-1][yi].y };
				vmath::vec3 Right = { entitymanager->map[xi+1][yi].x, 0.0, entitymanager->map[xi+1][yi].y };
				vector<vmath::vec3> CheckList;
				
				if (abs(velocity[0])>abs(velocity[2])) {
					if (velocity[2]>0) {
						CheckList = { Down,Up,Left,Right };
					}
					else {
						CheckList = { Up,Down,Left,Right };
					}
				}
				else {
					if (velocity[0]>0) {
						CheckList = { Right,Left,Up,Down };
					}
					else {
						CheckList = { Left,Right,Up,Down};
					}
				}

				for (int j = 0; j < 4; j++) {
					int x = CheckList[j][0] + 20 + 0.5;
					int y = CheckList[j][2] + 20 + 0.5;
					if (entitymanager->map[x][y].block!=true) {
						entitymanager->MovementList[entity]->PathToTarget.push_back(CheckList[j]);
						return;
					}
				}*/
			}
			entitymanager->TransformList[entity]->Translation -= Dir*entitymanager->MovementList[entity]->speed;
			return;
		}
	}
}