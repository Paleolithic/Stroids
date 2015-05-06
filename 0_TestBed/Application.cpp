#include "ApplicationClass.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "Steve");
	
	//vector4 stevePos = static_cast<vector4>(m_pMeshMngr->GetModelMatrix("Steve"));
	
	//= m_pMeshMngr->GetInstanceByName("Steve");
	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Pig.obj", "Pig");

	srand (time(NULL));

	for (int nAsteroid = 0; nAsteroid < numAsteroids; nAsteroid++)
	{
		/*float yPos = rand() % (int)(height) + (int)(-half_height);
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper", glm::translate(vector3(half_width, yPos, 0.0f)));*/
		m_sShipObject = "Steve";
	}
	 
}
void ApplicationClass::Update (void)
{
	srand (time(NULL));
	// Time stuff 
	// TODO: REWRITE
	m_pSystem->UpdateTime();//Update the system
	m_pMeshMngr->Update(); //Update the mesh information

	float width = MapValue((float)m_pSystem->GetWindowWidth(), 0.0f, 1280.0f, 0.0f, 22.5f);
	float height = MapValue((float)m_pSystem->GetWindowHeight(), 0.0f, 720.0f, 0.0f, (22.5f/1.8f));
	
	float half_height = height/2;
	float half_width  = width/2;

	//Variable for run time reset 
	static float fRunTime = 0.0f;
	//Variable for translate matrix
	static float fTotalTime = 0.0f;
	static float stroidTime = 0.0f;
	float fLapDifference = m_pSystem->StopClock();
	fTotalTime += fLapDifference;
	fRunTime += fLapDifference;
	stroidTime += fLapDifference;

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
	m_pMeshMngr->SetModelMatrix(m_m4ShipObject, m_sShipObject);
	m_pMeshMngr->SetModelMatrix(m_m4Pig, "Pig");
	
	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();

	if(m_bArcBall == true)
	{ 
		//ArcBall();
	}

	// CATHY SHIT
	/*float ScreenLength = rand() % (int)width + 1;
	if(fRunTime > ScreenLength)
	{
		fRunTime = 0.0f; //Resets run time
	}

	float ScreenPercent = MapValue(fRunTime, 0.0f, ScreenLength, 0.0f, 1.0f);*/

	
	if(stroidTime > 2.0f)
	{
		float xPos = rand() % (int)(width) + (int)(-width);
		float yPos = rand() % (int)(height) + (int)(-half_height);
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper", glm::translate(vector3(xPos, yPos, 0.0f)));
		stroidTime = 0.0f;
	}


	int stroidCount = m_pMeshMngr->GetNumberOfInstances();

	for(int nAsteroid = 2; nAsteroid < stroidCount; nAsteroid++)
	{
		float ScreenLength = rand() % (int)width + 1;
		if(fRunTime > ScreenLength)
		{
			fRunTime = 0.0f; //Resets run time
		}
		float ScreenPercent = MapValue(fRunTime, 0.0f, ScreenLength, 0.0f, 1.0f);

		vector3 tempBOCentroid = vector3(0.0f, 0.0f, 0.0f);
		String tempName = m_pMeshMngr->GetNameOfInstanceByIndex(nAsteroid);
		BoundingObjectClass* tempBO = m_pMeshMngr->GetBoundingObject(tempName);
		tempBOCentroid.y = tempBO->GetCentroidGlobal().y - tempBO->GetCentroidLocal().y;
		std::cout << tempBOCentroid.y << std::endl;
		
		#ifdef USINGLERP
		vector3 v3Lerp;
		v3Lerp.x = MapValue(ScreenPercent, 0.0f, 1.0f, -half_width, half_width);
		v3Lerp.y = tempBOCentroid.y;
		v3Lerp.z = 0.0f;
		#endif  USINGLERP 
	
		m_pMeshMngr->SetModelMatrix(glm::translate(v3Lerp), tempName);

		m_pMeshMngr->AddInstanceToRenderList("ALL");
	}
	
	// Jared's shit
	//OCTREE
	//Top-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, height/4,-2.5))* glm::scale(vector3(width/2, height/2, 10)), MERED, WIRE);
	//Cubes inside top right
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), (half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);

	//Top-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, height/4,-2.5))* glm::scale(vector3(width/2, height/2, 10)), MERED, WIRE);
	//Cubes inside top left
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), (half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);

	//Bottom-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, -height/4,-2.5))* glm::scale(vector3(width/2, height/2, 10)), MERED, WIRE);
	//Cubes inside bottom right
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, -height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), -(half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);

	//Bottom-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, -height/4,-2.5))* glm::scale(vector3(width/2, height/2, 10)), MERED, WIRE);
	//Cubes inside bottom left
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, -height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), -(half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);


	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}
