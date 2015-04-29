#include "ApplicationClass.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "Steve");

	for (int nCreepers = 0; nCreepers  < 20; nCreepers ++)
	{
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper", glm::translate(glm::sphericalRand(5.0f)));
	}
	
}
void ApplicationClass::Update (void)
{
	m_pSystem->UpdateTime(); //Update the system
	m_pMeshMngr->SetModelMatrix(m_m4SelectedObject, m_sSelectedObject); //Setting up the Model Matrix
	m_pMeshMngr->Update(); //Update the mesh information

	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();

	if(m_bArcBall == true)
	{
		ArcBall();
		m_pMeshMngr->SetModelMatrix(m_m4SelectedObject, m_sSelectedObject); //Setting up the Model Matrix
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

