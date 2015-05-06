/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/10
*/
#ifndef __APPLICATION_H_
#define __APPLICATION_H_
#define USINGLERP
#pragma warning(disable:4251)

#include "ME\MyEngine.h"
#include <SFML\Graphics.hpp>
#include <chrono>
#include "Asteroid.h"

using namespace MyEngine; //Using MyEngine namespace to use all the classes in the dll

class ApplicationClass
{
	bool m_bFPC;// First Person Camera flag
	bool m_bArcBall;// Arcball flag

	String m_sShipObject;//Ship Object String
	matrix4 m_m4ShipObject;//Ship Model Matrix
	matrix4 m_m4ShipOrientation;//Model Matrix
	matrix4 m_m4ShipTranslation;

	String m_sShieldObject;//Shield Object String
	matrix4 m_m4ShieldObject;//Shield Model Matrix

	// Number of asteroids
	int numAsteroids;
	int maxAsteroids; 

	// Shield spin amount
	float degreeSpin;

	// Custom height and width variables
	float width;
	float height;
	float half_w;
	float half_h;

	// Asteroid array
	BoundingObjectClass* m_pAsteroids;
	std::vector<Asteroid> asteroids;

	std::vector<vector3> lerp_vectors;
	std::vector<float> asteroid_lt;
	std::vector<float> asteroid_sp;

	//Standard variables
	static ApplicationClass* m_pInstance; // Singleton for this class
	SystemSingleton* m_pSystem;// Singleton of the system
	
	WindowClass* m_pWindow;// Window class
	GLSystemSingleton* m_pGLSystem;// Singleton of the OpenGL rendering context

	LightManagerSingleton* m_pLightMngr;// Singleton for the Light Manager
	MeshManagerSingleton* m_pMeshMngr;//Mesh Manager

	GridClass* m_pGrid; // Grid that represents the Coordinate System
	CameraSingleton* m_pCamera; // Singleton for the camera that represents our scene
	vector4 m_v4ClearColor;//Color of the scene
	
public:
	/*
	GetInstance
		Access the singleton that handles this class.
	*/
	static ApplicationClass* GetInstance( HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);

	/*
	ReleaseInstance
		Destroys the singleton of this class
	*/
	static void ReleaseInstance(void);

	/*
	Run
		Runs the main loop of this class
	*/
	void Run (void);

	/*
	GenerateBoundingSphere
		Calculates a sphere that involves the model specified by name under the provided matrix
	*/
	void GenerateBoundingSphere(matrix4 a_m4ToWorld, String a_sInstanceName);

private:
	/* Constructor	*/
	ApplicationClass();
	/* Copy Constructor - Defined for singleton so it cant be redefined in the header */
	ApplicationClass(ApplicationClass const& input);
	/* Copy Assignment Operator - Defined for singleton so it cant be redefined in the header */
	ApplicationClass& operator=(ApplicationClass const& input);
	/* Destructor */
	~ApplicationClass();

	/*
	Update
		Updates the scene
	*/
	void Update (void);

	/*
	Display
		Displays the scene
	*/
	void Display (void);

	/*
	Idle
		Runs faster than the update
	*/
	void Idle (void);

	/*
	Reshape
		Resizes the window
	*/
	void Reshape(int a_nWidth, int a_nHeight);
	
	/*
	InitAppSystem
		Initialize MyEngine variables
	*/
	void InitAppSystem(void);

	/*
	InitAppInternalVariables
		Behaves like the InitAppVariables method: it initializes this application variables,
		the difference is that this one is hidden from the student, initializing the fields that
		are not the focus of the lesson.
	*/
	void InitInternalAppVariables(void);

	/*
	InitAppVariables
		Initializes this application variables
	*/
	void InitUserAppVariables(void);

	/*
	ProcessKeyboard
		Manage the response of key presses
	*/
	void ProcessKeyboard(void);

	/*
	ProcessMouse
		Manage the response of key presses and mouse position
	*/
	void ProcessMouse(void);

	/*
	ProcessJoystick
		Manage the response of gamepad controllers
	*/
	void ProcessJoystick(void);
	
	/*
	Init
		Initializes the MyEngine window and rendering context
	*/
	void Init( HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);

	/*
	Release
		Releases the application
	*/
	void Release(void);

	/*
	ArcBall
		Process the arcball of the scene, rotating an object in the center of it
		a_fSensitivity is a factor of change
	*/
	void ArcBall(float a_fSensitivity = 0.1f);

	/*
	CameraRotation
		Manages the rotation of the camera
		a_fSpeed is a factor of change
	*/
	void CameraRotation(float a_fSpeed = 0.001f);

	/*
	ReadConfig
		Reads the configuration of the application to a file
	*/
	void ReadConfig(void);

	/*
	WriteConfig
		Writes the configuration of the application to a file
	*/
	void WriteConfig(void);

	/*
	OctTree detection
	*/
	vector3 OctDectection(/*BoundingObjectClass* dude*/);

};
#endif //__APPLICATION_H_