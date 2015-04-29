#include "ApplicationClass.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "Steve");

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

	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}

