#include "ApplicationClass.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "Steve");
	
	//vector4 stevePos = static_cast<vector4>(m_pMeshMngr->GetModelMatrix("Steve"));
	
	//= m_pMeshMngr->GetInstanceByName("Steve");
	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Pig.obj", "Pig");

	
}
void ApplicationClass::Update (void)
{
	m_pSystem->UpdateTime(); //Update the system
	m_pMeshMngr->SetModelMatrix(m_m4SelectedObject, m_sSelectedObject); //Setting up the Model Matrix
	m_pMeshMngr->Update(); //Update the mesh information

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
		m_pMeshMngr->SetModelMatrix(m_m4SelectedObject, m_sSelectedObject); //Setting up the Model Matrix
	}

	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}

