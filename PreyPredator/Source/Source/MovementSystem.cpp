#include "../Heads/MovementSystem.h"
#include <algorithm>
#include <ctime>

void MovementSystem::Initialize(EntityManager * entitymanager)
{
	//OpenList = std::vector<PathFindingNode*>(0);


}

void MovementSystem::Update(EntityManager * entitymanager)
{
	for (int i = 0; i < entitymanager->MovementList.size(); i++)
	{
		if (entitymanager->MovementList[i] != nullptr) {
			if (entitymanager->MovementList[i]->DoPathFinding == true) {
				int r = 3;
				while(!AstarPathFinding(entitymanager, i, entitymanager->MovementList[i]->TargetPosition)) {//if targetplace is unreachable, ramdomly choose another place nearby
					srand(time(0));
					float x = rand() % r - r/2;
					float y = rand() % r - r/2;
					entitymanager->MovementList[i]->TargetPosition = entitymanager->MovementList[i]->TargetPosition + vmath::vec3{x,0,y};
					r++;
				}
				entitymanager->MovementList[i]->DoPathFinding = false;
			}
		UpdateVelocity(entitymanager,i);
		UpdateOrientation(entitymanager, i);
		UpdatePosition(entitymanager, i);

		}
	}
}

void MovementSystem::UpdateOrientation(EntityManager * entitymanager, unsigned int entity)
{
	float tan;
	float angle;
	if (entitymanager->MovementList[entity]->Velocity[0] != 0.0f || entitymanager->MovementList[entity]->Velocity[2] != 0.0f)
	{
		entitymanager->MovementList[entity]->Velocity = vmath::normalize(entitymanager->MovementList[entity]->Velocity);
		if (entitymanager->MovementList[entity]->Velocity[2] != 0.0f) {
			tan = entitymanager->MovementList[entity]->Velocity[0] / entitymanager->MovementList[entity]->Velocity[2];
			angle = atanf(tan) / 0.0174532925f;
		}
		else {
			angle = 90;
		}
		if (entitymanager->MovementList[entity]->Velocity[2] <= 0) {
			angle += 180;
		}
		if (entitymanager->TransformList[entity]->Rotation[1] >= 360) {
			entitymanager->TransformList[entity]->Rotation[1] -= 360;
		}
		else if (entitymanager->TransformList[entity]->Rotation[1] <= 0) {
			entitymanager->TransformList[entity]->Rotation[1] += 360;
		}

		if (angle >= 360) {
			angle -= 360;
		}
		else if (angle <= 0) {
			angle += 360;
		}

		if (angle > entitymanager->TransformList[entity]->Rotation[1]) {
			if ((angle - entitymanager->TransformList[entity]->Rotation[1])<(entitymanager->TransformList[entity]->Rotation[1]+360-angle)) {
				entitymanager->TransformList[entity]->Rotation[1] += 10;
			}
			else {
				entitymanager->TransformList[entity]->Rotation[1] -= 10;
			}
		}
		else if(angle < entitymanager->TransformList[entity]->Rotation[1]) {
			if ((entitymanager->TransformList[entity]->Rotation[1]-angle)<(angle + 360 - entitymanager->TransformList[entity]->Rotation[1])) {
				entitymanager->TransformList[entity]->Rotation[1] -= 10;
			}
			else {
				entitymanager->TransformList[entity]->Rotation[1] += 10;
			}
		}
		if (abs(entitymanager->TransformList[entity]->Rotation[1] - angle) < 15 || abs(entitymanager->TransformList[entity]->Rotation[1] + 360 - angle < 15) || abs(angle + 360 - entitymanager->TransformList[entity]->Rotation[1] < 15)) {
			entitymanager->TransformList[entity]->Rotation[1] = angle;
		}
	}
}



void MovementSystem::UpdatePosition(EntityManager * entitymanager, unsigned int entity)
{
	entitymanager->MovementList[entity]->Velocity = entitymanager->MovementList[entity]->speed*entitymanager->MovementList[entity]->Velocity;
	entitymanager->TransformList[entity]->Translation += entitymanager->MovementList[entity]->Velocity;
}



void MovementSystem::UpdateVelocity(EntityManager * entitymanager, unsigned int entity)
{
	if (entitymanager->MovementList[entity]->PathToTarget.size() > 0) {
		vmath::vec3 NextTarget = entitymanager->MovementList[entity]->PathToTarget.back();
		vmath::vec3 delta = entitymanager->TransformList[entity]->Translation - NextTarget;
		float deltadistance = sqrt(delta[0] * delta[0] + delta[2] * delta[2]);
		if (deltadistance <= 0.5) {
			entitymanager->MovementList[entity]->PathToTarget.pop_back();
		}
		entitymanager->MovementList[entity]->Velocity = NextTarget - entitymanager->TransformList[entity]->Translation;
	}
	else {
		entitymanager->MovementList[entity]->Velocity = { 0.0f,0.0f,0.0f };
	}
}

void MovementSystem::SimpleMoveTo(EntityManager * entitymanager, unsigned int entity, vmath::vec3 Target)
{
	entitymanager->MovementList[entity]->PathToTarget.clear();
	entitymanager->MovementList[entity]->PathToTarget.push_back(Target);
}

bool MovementSystem::AstarPathFinding(EntityManager * entitymanager, unsigned int entity, vmath::vec3 Target)
{
	entitymanager->ResetPathfindingMap();
	entitymanager->MovementList[entity]->PathToTarget.clear();
	PathFindingFailedCount = 0;
	int sx = entitymanager->TransformList[entity]->Translation[0]+20;//real coodinate = index - 20, index = coodinate + 20
	int sy = entitymanager->TransformList[entity]->Translation[2]+20;
	int tx = Target[0] + 20 + 0.5;
	int ty = Target[2] + 20 + 0.5;

	if (sx>=40) sx = 39; // clamp
	if (sy>=40) sy = 39;
	if (tx>=40) tx = 39;
	if (ty>=40) ty = 39;

	if (sx<0) sx = 0;
	if (sy<0) sy = 0;
	if (tx<0) tx = 0;
	if (ty<0) ty = 0;
	

	PathFindingNode* startNode= &entitymanager->map[sx][sy];
	PathFindingNode* TargetNode = &entitymanager->map[tx][ty];
	
	OpenList.clear();
	OpenList.push_back(startNode);
	bool i = PathFindingRecursion(entitymanager, startNode,entity, TargetNode);
	MakePath(entitymanager, entity, TargetNode);
	return i;
}

bool MovementSystem::PathFindingRecursion(EntityManager * entitymanager, PathFindingNode * currentNode,int entity, PathFindingNode * TargetNode)
{
	//Do PF
	//set current position as parent add it into close list
	while (OpenList.size() > 0) {

		if (currentNode == TargetNode) {
			return true;
		}
		if (PathFindingFailedCount >= 8) {
			return false;
		}
		if (OpenList.size() <= 0) {
			return false;
		}


		for (int i = 0; i < OpenList.size(); i++) {
			if (OpenList[i] == currentNode) {
				OpenList.erase(OpenList.begin() + i);
				i--;
			}
		}

		currentNode->flag = PathFindingNode::Closed;
		if (abs(currentNode->x - TargetNode->x) < 1.5 && abs(currentNode->y - TargetNode->x) < 1.5) {
			PathFindingFailedCount++;
		}
		//search enterable adjecent nodes
		//add these nodes into open list
		for (int xi = 0; xi < 3; xi++) {
			for (int yi = 0; yi < 3; yi++) {
				if (xi != 1 || yi != 1) {
					int x = currentNode->x + (xi - 1) + 20;
					int y = currentNode->y + (yi - 1) + 20;
					if ((x >= 0 && x < 40) && (y >= 0 && y < 40)) {
						if ((entitymanager->map[x][y].block != true) && (entitymanager->map[x][(int)(currentNode->y + 20 + 0.5)].block != true) && (entitymanager->map[(int)(currentNode->x + 20 + 0.5)][y].block != true)) {//if not accessable
							if (entitymanager->map[x][y].flag != PathFindingNode::Closed) {							  //if not in closed list
								if (entitymanager->map[x][y].flag == PathFindingNode::NotVisited) {// if not in open list
									OpenList.push_back(&entitymanager->map[x][y]);//put into open list
									entitymanager->map[x][y].flag = PathFindingNode::Open;
								}
								//get G
								float G, H;
								if (entitymanager->map[x][y].Parent == NULL) {
									G = 0;
								}
								else {
									float dx1 = entitymanager->TransformList[entity]->Translation[0] - entitymanager->map[x][y].x;
									float dy1 = entitymanager->TransformList[entity]->Translation[2] - entitymanager->map[x][y].y;
									vmath::vec2 delta1 = { dx1,dy1 };

									G = sqrt(delta1[0] * delta1[0] + delta1[1] * delta1[1]);
								}

								//get H
								float dx2 = TargetNode->x - entitymanager->map[x][y].x;
								float dy2 = TargetNode->y - entitymanager->map[x][y].y;
								vmath::vec2 delta2 = { dx2,dy2 };

								H = sqrt(delta2[0] * delta2[0] + delta2[1] * delta2[1]);
								//H = abs( dx + dy);
								//set G,H F

								if ((G + H) < (entitymanager->map[x][y].G + entitymanager->map[x][y].H)) {
									entitymanager->map[x][y].G = G;
									entitymanager->map[x][y].H = H;
									entitymanager->map[x][y].Parent = currentNode;
								}
							}
						}
						else if (abs(entitymanager->map[x][y].x - TargetNode->x) < 1.5 && abs(entitymanager->map[x][y].y - TargetNode->x) < 1.5) {
							PathFindingFailedCount++;
						}
					}
				}
			}
		}
		PathFindingNode* smallestNode;
		if (OpenList.size() > 0) {
			smallestNode = OpenList[0];
		}
		else {
			return false;
		}
		//take smallest G+H node a
		for (int i = 0; i < OpenList.size(); i++) {
			if ((OpenList[i]->H + OpenList[i]->G) < (smallestNode->H + smallestNode->G)) {
				smallestNode = OpenList[i];
			}
		}
		currentNode = smallestNode;
	}

	//return PathFindingRecursion(entitymanager, smallestNode,entity, TargetNode);
	//yes recursive
}

void MovementSystem::MakePath(EntityManager * entitymanager, unsigned int entity, PathFindingNode * TargetNode)
{
	//
	vmath::vec3 TargetPosition;
	TargetPosition[0] = TargetNode->x;
	TargetPosition[1] = 0;
	TargetPosition[2] = TargetNode->y;
	entitymanager->MovementList[entity]->PathToTarget.push_back(TargetPosition);
	if (TargetNode->Parent == NULL) {
		return;
	}
	MakePath(entitymanager, entity, TargetNode->Parent);
}
