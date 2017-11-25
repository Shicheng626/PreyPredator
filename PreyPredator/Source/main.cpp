#include "Heads/RenderSystem.h"
#include "Heads/MovementSystem.h"
#include "Heads/SensingSystem.h"
#include "Heads/AISystem.h"
#include "Heads/CollisionSystem.h"

using namespace std;

#define PI 3.1415926
#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MSperFrame 32


Scene scene;
RenderSystem rendersystem;
EntityManager entitymanager;
MovementSystem movementsystem;
SensingSystem sensingsystem;
AISystem AIsystem;
CollisionSystem collisionsystem;

void Terminate(void) {
	TwTerminate();
}

void Reshape(int width, int height) {
	glViewport(0, 0, width, height);
	TwWindowSize(width, height);
}

void TW_CALL CopyStdStringToClient(std::string & dst, const std::string & src) {
	dst = src;
}

void InitializeGUI() {
	TwBar *operbar = TwNewBar("ControlBar");
	TwDefine(" GLOBAL help='CubeAnimation' ");
	TwDefine(" ControlBar position='16 16' size='200 300' color='96 216 224' ");
	TwAddVarRW(operbar, "CW/CCW", TW_TYPE_BOOL8, &scene.usingCW, NULL);
	TwEnumVal primitivesEV[] = { { PTS,"Point" },{ WIREFRAME,"Wireframe" },{ SOLID,"Solid" } };
	TwType primtiveType = TwDefineEnum("PrimitiveType", primitivesEV, 3);
	TwAddVarRW(operbar, "Primitives", primtiveType, &scene.prim, NULL);
	TwType shadingType;
	shadingType = TwDefineEnum("shadingType", NULL, 0);
	TwAddVarRW(operbar, "Shading", shadingType, &scene.shading, " enum='0{Flat},1{Smooth}' ");
	TwAddSeparator(operbar, "", "");
	TwAddVarRW(operbar, "Rot X", TW_TYPE_FLOAT, &scene.MyCamera.Rotation[0], "group='Camera Transfromation' min=-360 max=360 step=5");
	TwAddVarRW(operbar, "Rot Y", TW_TYPE_FLOAT, &scene.MyCamera.Rotation[1], "group='Camera Transfromation' min=-360 max=360 step=5");
	TwAddVarRW(operbar, "Rot Z", TW_TYPE_FLOAT, &scene.MyCamera.Rotation[2], "group='Camera Transfromation' min=-360 max=360 step=5");
	TwAddSeparator(operbar, "", "");
	TwAddVarRW(operbar, "Trans X", TW_TYPE_FLOAT, &scene.MyCamera.Translation[0], "group='Camera Transfromation' min=-20 max=20 step=0.5");
	TwAddVarRW(operbar, "Trans Y", TW_TYPE_FLOAT, &scene.MyCamera.Translation[1], "group='Camera Transfromation' min=-20 max=20 step=0.5");
	TwAddVarRW(operbar, "Trans Z", TW_TYPE_FLOAT, &scene.MyCamera.Translation[2], "group='Camera Transfromation' min=-50 max=50 step=0.5");
	TwAddSeparator(operbar, "", "");
	TwAddVarRW(operbar, "Reset", TW_TYPE_BOOL8, &scene.bReset, " help='reset to the initial status'");
	TwAddSeparator(operbar, "", "");
	TwAddVarRW(operbar, "LightON", TW_TYPE_BOOL8, &scene.lightson, "");
}

void Update() {
	entitymanager.Update();
	sensingsystem.Update(&entitymanager);
	AIsystem.Update(&entitymanager);
	movementsystem.Update(&entitymanager);
	rendersystem.Update(&entitymanager);
	collisionsystem.Update(&entitymanager);
}

void InitializeGlut(int argc, char ** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("PreyPredator");
	glutCreateMenu(NULL);

	TwInit(TW_OPENGL, NULL);

	// Set GLUT event callbacks
	// - Directly redirect GLUT mouse button events to AntTweakBar
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	// - Directly redirect GLUT mouse motion events to AntTweakBar
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	// - Directly redirect GLUT key events to AntTweakBar
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);

	// - Directly redirect GLUT special key events to AntTweakBar
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	// - Send 'glutGetModifers' function pointer to AntTweakBar;
	//   required because the GLUT key event functions do not report key modifiers states.
	TwGLUTModifiersFunc(glutGetModifiers);
	glutDisplayFunc(Update);
	glutReshapeFunc(Reshape);


}

GLvoid Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(MSperFrame, Timer, 1);
}

int main(int argc, char ** argv){

	InitializeGlut(argc, argv);

	entitymanager.Initialize();
	rendersystem.scene = &scene;
	InitializeGUI();
	glutTimerFunc(MSperFrame, Timer, 1);
	glutMainLoop();
	atexit(Terminate);
	return 0;
}
