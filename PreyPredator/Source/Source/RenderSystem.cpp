
#include "../Heads/RenderSystem.h"

using namespace std;


void RenderSystem::SetMaterial(Material material) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, material.Ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.Diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.Specular);
	glMaterialf(GL_FRONT, GL_SHININESS, material.Shine*128.0f);
}

void RenderSystem::lightsInitialize() {
	GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };


	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
}

void RenderSystem::lightsPosition() {
	GLfloat light0_position[] = { 0.0,40.0,0.0, 1.0 };
	GLfloat light1_position[] = { 0.0,40.0,0.0,1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
}

void RenderSystem::DrawMesh(Mesh mesh, TransformComponent transform, Material material) {
	glPushMatrix();
	SetMaterial(material);
	glTranslatef(transform.Translation[0], transform.Translation[1], transform.Translation[2]);

	glRotatef(transform.Rotation[0], 1.0f, 0.0f, 0.0f);
	glRotatef(transform.Rotation[2], 0.0f, 0.0f, 1.0f);
	glRotatef(transform.Rotation[1], 0.0f, 1.0f, 0.0f);

	glScalef(transform.Scale[0], transform.Scale[1], transform.Scale[2]);

	glBegin(GL_TRIANGLES);
	for (unsigned i = 0; i < mesh.Faces.size(); ++i) {
		glNormal3f(mesh.Normals[i][0], mesh.Normals[i][1], mesh.Normals[i][2]);
		for (int j = 0; j < 3; ++j) {
			glVertex3f(mesh.Vetices[mesh.Faces[i][j]][0], mesh.Vetices[mesh.Faces[i][j]][1], mesh.Vetices[mesh.Faces[i][j]][2]);
		}
	}
	glEnd();
	glPopMatrix();
}

void RenderSystem::SetupScene() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);

	switch (scene->prim) {
	case PTS:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(1.0f);
		break;
	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		break;
	case SOLID:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		std::cout << "error prim type!\n";
	}

	if (scene->usingCW) {
		glFrontFace(GL_CW);
	}
	else {
		glFrontFace(GL_CCW);
	}
	if (scene->bReset) {
		scene->Reset();
		scene->bReset = false;
	}

	switch (scene->shading) {
	case FLAT:
		glShadeModel(GL_FLAT);
		break;
	case SMOOTH:
		glShadeModel(GL_SMOOTH);
		break;
	default:
		std::cout << "error shading mode\n";
	}
	if (scene->lightson) {
		lightsInitialize();
		lightsPosition();
	}
	else {
		glDisable(GL_LIGHTING);
		glColor3f(0.4f, 0.0f, 0.6f);
	}
}

void RenderSystem::SetProjectionMatrix() {
	//projection matrix
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)width / (double)height, scene->MyCamera.near_, scene->MyCamera.far_);
}

void RenderSystem::SetViewMatrix() {
	// set modelviewmatrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// set viewing matrix
	scene->MyCamera.update();
	vmath::mat4 Mlookat = vmath::lookat(scene->MyCamera.eye, scene->MyCamera.eye - vmath::vec3(0, 0, 1), vmath::vec3(0, 1, 0));
	Mlookat = scene->MyCamera.M * Mlookat;
	GLfloat m[16];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[4 * i + j] = Mlookat[i][j];
		}
	}
	glMultMatrixf(m);
}

void RenderSystem::DrawGround(EntityManager* entitymanager)
{
	TransformComponent transPlane;
	transPlane.Translation = { 0.0f,0.0f,0.0f };
	transPlane.Scale = { 20.5,1,20.5 };
	DrawMesh(entitymanager->MeshData.Plane, transPlane, entitymanager->MaterialData.Brass);
}

void RenderSystem::DrawWalls(EntityManager* entitymanager)
{
	TransformComponent transWall;

	for (int x = 0; x < 40; x++) {
		for (int y = 0; y < 40; y++) {
			float xf = x-20;
			float yf = y-20;
			if (entitymanager->map[x][y].block==true) {
				transWall.Translation = { xf,0,yf };
				DrawMesh(entitymanager->MeshData.Cube, transWall, entitymanager->MaterialData.Brass);
			}
		}
	}
}

void RenderSystem::DrawSensingLine(EntityManager * entitymanager)
{
	for (int i = 0; i < entitymanager->SensingList.size(); i++) {
		if (entitymanager->SensingList[i]!= nullptr) {
			for (int j = 0; j < entitymanager->SensingList[i]->HeardEntities.size(); j++) {
				vmath::vec4 color = { 0.0,1.0,0.0,0.0 };
				vmath::vec3 startpoint = entitymanager->TransformList[i]->Translation;
				startpoint[1] = 0.6f;
				vmath::vec3 targetpoint = entitymanager->TransformList[entitymanager->SensingList[i]->HeardEntities[j]]->Translation;
				targetpoint[1] = 0.6f;
				DrawLine(startpoint, targetpoint,color);
			}
			for (int j = 0; j < entitymanager->SensingList[i]->SeenEntities.size(); j++) {
				vmath::vec4 color = { 1.0,0.0,0.0,0.0 };
				vmath::vec3 startpoint = entitymanager->TransformList[i]->Translation;
				startpoint[1] = 0.6f;
				vmath::vec3 targetpoint = entitymanager->TransformList[entitymanager->SensingList[i]->SeenEntities[j]]->Translation;
				targetpoint[1] = 0.6f;
				DrawLine(startpoint, targetpoint, color);
			}
		}
	}

}

void RenderSystem::DrawLine(vmath::vec3 point1, vmath::vec3 point2,vmath::vec4 color)
{
	glPushMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glLineWidth(4);
	glBegin(GL_LINE_LOOP);

	glVertex3f(point1[0], point1[1], point1[2]);
	glVertex3f(point2[0], point2[1], point2[2]);

	glEnd();
	glPopMatrix();
}

void RenderSystem::Update(EntityManager* entitymanager) {
	SetupScene();
	SetProjectionMatrix();
	SetViewMatrix();
	DrawGround(entitymanager);
	DrawWalls(entitymanager);
	for (int i = 0; i<entitymanager->RenderList.size();i++)
	{
		if (entitymanager->RenderList[i] != nullptr) {
			DrawMesh(entitymanager->RenderList[i]->mesh, *entitymanager->TransformList[i], entitymanager->RenderList[i]->material);
		}
	}
	DrawSensingLine(entitymanager);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	TwDraw();
	glFlush();
	glutSwapBuffers();

	GetFPS();
}

void RenderSystem::GetFPS()
{
	static float framesPerSecond = 0.0f;       // This will store our fps
	static float lastTime = 0.0f;       // This will hold the time from the last frame
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{

		lastTime = currentTime;
		cout<<"Current Frames Per Second: %d"<<int(framesPerSecond)<<endl;
		FPS = framesPerSecond;
		framesPerSecond = 0;
	}
}

void RenderSystem::reset() {
	scene->Reset();
}