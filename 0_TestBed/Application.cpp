#include "ApplicationClass.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "Steve");
	
	//vector4 stevePos = static_cast<vector4>(m_pMeshMngr->GetModelMatrix("Steve"));
	
	//= m_pMeshMngr->GetInstanceByName("Steve");
	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Pig.obj", "Pig");


	for (int nCreepers = 0; nCreepers  < 2; nCreepers ++)
	{
		//m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper");
	//}
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper");
		m_sSelectedObject = "Creeper";
	}

}
void ApplicationClass::Update (void)
{
	
	m_pSystem->UpdateTime();//Update the system
	m_pMeshMngr->Update(); //Update the mesh information
	float fTimeSpan = m_pSystem->LapClock();
	static float fRunTime = 0.0f;
	fRunTime += fTimeSpan;

	// Time counter
	static float fTotalTime = 0.0f;
	float fLapDifference = m_pSystem->StopClock();
	fTotalTime += fLapDifference;

	// Steve and Pig Bounding Object Classes
	BoundingObjectClass* steveObj = m_pMeshMngr->GetBoundingObject("Steve");
	BoundingObjectClass* pigObj = m_pMeshMngr->GetBoundingObject("Pig");
	vector3 pigPos = steveObj->GetCentroidGlobal();
	pigPos.y = pigPos.y - steveObj->GetHalfWidth().y;

	// Pig translate matrixes
	matrix4 sTranslate = glm::translate(vector3(2.0f, 2.0f, 0.0f));
	matrix4 sRotate = glm::rotate(matrix4(IDENTITY), fTotalTime * 15, vector3(0.0f, 0.0f, 1.0f));
	matrix4 sOrbit = glm::rotate(matrix4(IDENTITY), -fTotalTime * 15, vector3(0.0f, 0.0f, 1.0f));

	// Pig combined translate matrix
	matrix4 m_m4Pig = sRotate * sTranslate * sOrbit * glm::translate(pigPos);
	
	// Set Pig Model Matrix
	m_pMeshMngr->SetModelMatrix(m_m4Pig, "Pig");

	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();

	if(m_bArcBall == true)
	{
		ArcBall();
		m_pMeshMngr->SetModelMatrix(m_m4ObjectTranslation * m_m4ObjectOrientation, m_sSelectedObject); //Setting up the Model Matrix
	}

	float ScreenLength = 3.0f;
	if(fRunTime > ScreenLength)
	{
		fRunTime = 0.0f; //Resets run time
	}
	float ScreenPercent = MapValue(fRunTime, 0.0f, ScreenLength, 0.0f, 1.0f);

	float windowH = MapValue((float)m_pSystem->GetWindowHeight(), 0.0f, 720.0f, 0.0f, (22.5f/1.8f));
	float windowW = MapValue((float)m_pSystem->GetWindowWidth(), 0.0f, 1280.0f, 0.0f, 22.5f);
	float CreeperY = windowH/2;
	float CreeperX = windowW/2;
	int randX = rand() % (int)windowW;
	int arrayVal[2];
	for(int i = 0; i < 2; i++)
	{
		arrayVal[i] = rand() % (int)windowW;
	}

	for(int creepernum = 0; creepernum < 2; creepernum++)
	{
		vector3 Start(arrayVal[creepernum], 0.0f,  0.0f);
		vector3 Finish(-arrayVal[creepernum], 0.0f, 0.0f );
	
	
	
	#ifdef USINGLERP
	vector3 v3Lerp;
	v3Lerp.x = MapValue(ScreenPercent, 0.0f, 1.0f, Start.x, Finish.x);
	v3Lerp.y = MapValue(ScreenPercent, 0.0f, 1.0f, Start.y, Finish.y);
	v3Lerp.z = MapValue(ScreenPercent, 0.0f, 1.0f, Start.z, Finish.z);
	m_m4ObjectOrientation = glm::translate(v3Lerp);
	#endif  USINGLERP 
	

	
	//v3Lerp = glm::lerp(Start,Finish,ScreenPercent);
	/*m_m4ObjectOrientation =	glm::rotate(MEIDENTITY, v3Lerp.x, MEAXISX) *
								glm::rotate(MEIDENTITY, v3Lerp.y, MEAXISY) *
								glm::rotate(MEIDENTITY, v3Lerp.z, MEAXISZ);*
								glm::translate(v3Lerp);
	#endif  USINGLERP */

	m_pMeshMngr->SetModelMatrix(m_m4ObjectTranslation * m_m4ObjectOrientation, m_sSelectedObject);
	m_pMeshMngr->AddAxisToQueue(m_m4ObjectTranslation * m_m4ObjectOrientation * glm::scale(vector3(3.0f)));

	m_pMeshMngr->AddInstanceToRenderList("ALL");
	}

	//m_pMeshMngr->AddAxisToQueue(glm::translate(vector3(0,0,0)));
	
	float width = MapValue((float)m_pSystem->GetWindowWidth(), 0.0f, 1280.0f, 0.0f, 22.5f);
	float height = MapValue((float)m_pSystem->GetWindowHeight(), 0.0f, 720.0f, 0.0f, (22.5f/1.8f));
	//float height = m_pSystem->GetWindowHeight;
	
	float half_height = height/2;
	float half_width  = width/2;

	//Top-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, height/4,0))* glm::scale(vector3(width/2, height/2, 0)), MERED, WIRE);
	//Cubes inside top right
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, height/8,0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, (half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), (half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), (half_height/4),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);

	//Top-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, height/4,0))* glm::scale(vector3(width/2, height/2, 0)), MERED, WIRE);
	//Cubes inside top left
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, height/8,0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, (half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), (half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), (half_height/4),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);

	//Bottom-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, -height/4,0))* glm::scale(vector3(width/2, height/2, 0)), MERED, WIRE);
	//Cubes inside bottom right
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, -height/8,0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, -(half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), -(half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), -(half_height/4),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);

	//Bottom-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, -height/4,0))* glm::scale(vector3(width/2, height/2, 0)), MERED, WIRE);
	//Cubes inside bottom left
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, -height/8,0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, -(half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), -(half_height-(half_height/4)),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), -(half_height/4),0))* glm::scale(vector3(width/4, height/4, 0)), MERED, WIRE);


	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}

