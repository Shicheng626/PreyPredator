Prey-Predator Behavior Model
Shicheng Zhou

Description:

This is a student training project, which involved several basic technical problems in game programming area. In this model, there are two kinds of NPC, the prey and the predator. The predator would try to find and eat the prey and the prey would try to hind and escape when the prey is coming. This model is built up in C++ and OpenGL and no additional game engine libraries are used in this model.

Importance:

As a training project, this model implemented several technical problems, which are pretty important in game programming. 
1.	ECS (Entity-Component-System) Framework.
2.	Fixed Rendering Pipeline in OpenGL.
3.	Grid-based map with obstacle.
4.	A* Path-Finding Algorithm.
5.	Ray-Casting for NPC`s visual sensing.
6.	Collision detection with map obstacles and other NPC.
7.	Status Machine style AI that could hunt and escape.

Files:

1.	Components.h/.cpp : Declare all the components that might be used.
2.	EntityManager.h/.cpp: The core of the ECS Framework. Store the lists of the components and handle the spawning and destroying of the entities.
3.	Scene.h/.cpp: Declare and define the camera and scene.
4.	RenderSystem.h/.cpp: Deal with the render components with OpenGL fixed pipeline.
5.	MovementSystem.h/.cpp: Update the velocity, orientation, and position of transform components and movement components. Pathfinding Algorithm is also implemented here.
8.	CollisionSystem.h/.cpp: Handle the collision between the NPC and map obstacles or other NPC.
6.	SensingSystem.h/.cpp: Ray-Casting.
9.	AISystem.h/.cpp: Status Machine style AI that could hunt and escape.

How to run:

If you are using visual studio, just open the project and run it in debug or release mode. For other environment, please make sure you have AntweakBar, OpenGL and GLUT.
