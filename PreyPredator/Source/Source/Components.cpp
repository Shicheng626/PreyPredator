#include "../Heads/Components.h"

void StaticMaterial::Initialize() {
	Pearl.Ambient = vmath::vec4(0.25, 0.20725, 0.20725, 1.0);
	Pearl.Diffuse = vmath::vec4(1, 0.829, 0.829, 1.0);
	Pearl.Specular = vmath::vec4(0.296648, 0.296648, 0.296648, 1.0);
	Pearl.Shine = 0.088;

	Rock.Ambient = vmath::vec4(0.05375, 0.05, 0.06625, 1.0);
	Rock.Diffuse = vmath::vec4(0.18275, 0.17, 0.22525, 1.0);
	Rock.Specular = vmath::vec4(0.332741, 0.328634, 0.346435, 1.0);
	Rock.Shine = 0.3;

	Ruby.Ambient = vmath::vec4(0.05, 0.0, 0.0, 0.078125);
	Ruby.Diffuse = vmath::vec4(0.5, 0.4, 0.4, 0.078125);
	Ruby.Specular = vmath::vec4(0.7, 0.04, 0.04, 0.078125);
	Ruby.Shine = 0.6;

	Brass.Ambient = vmath::vec4(0.0, 0.05, 0.0, 0.078125);
	Brass.Diffuse = vmath::vec4(0.4, 0.5, 0.4, 0.078125);
	Brass.Specular = vmath::vec4(0.04, 0.7, 0.04, 0.078125);
	Brass.Shine = 0.21794872;

	Emerald.Ambient = vmath::vec4(0.0215, 0.1745, 0.0215, 1.0);
	Emerald.Diffuse = vmath::vec4(0.07568, 0.61424, 0.07568, 1.0);
	Emerald.Specular = vmath::vec4(0.633, 0.727811, 0.633, 1.0);
	Emerald.Shine = 0.6;
}

void StaticMesh::InitializeNormal(Mesh* mesh) {
	mesh->Normals.resize(mesh->Faces.size());
	for (unsigned i = 0; i < mesh->Faces.size(); ++i) {
		vmath::vec3 ab = mesh->Vetices[mesh->Faces[i][1]] - mesh->Vetices[mesh->Faces[i][0]];
		vmath::vec3 ac = mesh->Vetices[mesh->Faces[i][2]] - mesh->Vetices[mesh->Faces[i][0]];
		mesh->Normals[i] = normalize(cross(ab, ac));
	}
}

void StaticMesh::Initialize() {
	//Cube
	Cube.Vetices = {
		{ -0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f, 0.5f },
		{ -0.5f, 1.0f, -0.5f },
		{ -0.5f, 1.0f, 0.5f },
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ 0.5f, 1.0f, -0.5f },
		{ 0.5f, 1.0f, 0.5f } };
	Cube.Faces = {
		{ 0,6,4 },{ 0,2,6 },{ 0,3,2 },{ 0,1,3 },{ 2,7,6 },{ 2,3,7 },{ 4,6,7 },{ 4,7,5 },{ 0,4,5 },{ 0,5,1 },{ 1,5,7 },{ 1,7,3 } };

	InitializeNormal(&Cube);

	//Plane
	Plane.Vetices = {
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f,-1.0f },
		{ -1.0f, 0.0f,-1.0f },
		{ -1.0f, 0.0f, 1.0f }
	};

	Plane.Faces = {
		{0,1,2},{0,2,3}
	};

	InitializeNormal(&Plane);
	//Pawn
	Pawn.Vetices = { {0.3f,0.0f,-0.5f},
	{-0.3f,0.0f,-0.5f},
	{0.0f,0.0f,0.5f },

	{ 0.3f,0.5f,-0.5f },
	{ -0.3f,0.5f,-0.5f },
	{ 0.0f,0.5f,0.5f }
	};

	
	Pawn.Faces = { {0,5,2},{0,3,5},{0,4,3},{0,1,4},{4,5,3},{1,5,4},{1,2,5},{1,0,2}};
	InitializeNormal(&Pawn);
}