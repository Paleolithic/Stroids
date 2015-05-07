#include "ApplicationClass.h"
#include "Asteroid.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "Steve");
	m_sShipObject = "Steve";
	//vector4 stevePos = static_cast<vector4>(m_pMeshMngr->GetModelMatrix("Steve"));
	
	//= m_pMeshMngr->GetInstanceByName("Steve");
	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Pig.obj", "Pig");
	m_sShieldObject = "Pig";
	bool clockwise = true;

	srand (time(NULL));	 

	/*float xPos = rand() % (int)(width) + (int)(-width);
	float yPos = rand() % (int)(height) + (int)(-height/2);
	numAsteroids++;
	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper" + numAsteroids, glm::translate(vector3(xPos, yPos, 0.0f)));*/
}

void ApplicationClass::Update (void)
{
	srand (time(NULL));
	// Time stuff 
	m_pSystem->UpdateTime();//Update the system
	m_pMeshMngr->Update(); //Update the mesh information

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

	if(fTotalTime >= 60.0f){
		maxAsteroids = 10;
	}

	// Steve and Pig Bounding Object Classes
	BoundingObjectClass* steveObj = m_pMeshMngr->GetBoundingObject("Steve");
	BoundingObjectClass* pigObj = m_pMeshMngr->GetBoundingObject("Pig");
	vector3 pigPos = steveObj->GetCentroidGlobal();
	pigPos.y = pigPos.y - steveObj->GetHalfWidth().y;

	// Pig translate matrixes
	matrix4 sTranslate = glm::translate(vector3(2.0f, 2.0f, 0.0f));
	matrix4 sRotate = glm::rotate(matrix4(IDENTITY), degreeSpin * 5, vector3(0.0f, 0.0f, 1.0f));
	matrix4 sOrbit = glm::rotate(matrix4(IDENTITY), -degreeSpin * 5, vector3(0.0f, 0.0f, 1.0f));
	
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
	// Do the following every two seconds as long as number of asteroids is less than max
	if(stroidTime > 2.0f && numAsteroids < maxAsteroids)
	{
		
		//Set initial random x and y position for asteroids
		float xPos = rand() % (int)(width) + (int)(-width);
		float yPos = rand() % (int)(height) + (int)(-half_height);
		vector3 pos = vector3(xPos, yPos, 0.0f);

		//Add to number of asteroids
		numAsteroids++;

		float speed = rand() % 5 + 3;
		float direction = rand() % 2;
		asteroids.push_back(Asteroid(pos, speed, direction));
	
		//Add new asteroid to the screen and add a lifetime float and screen percent float to the arrays
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Creeper.obj", "Creeper" + std::to_string(asteroids.size()), glm::translate(vector3(xPos, yPos, 0.0f)));
		//std::cout << "Creeper" + std::to_string(asteroids.size()) << std::endl;

		//Reset timer
		stroidTime = 0.0f;
	}


	vector3 color = MERED;
	float ScreenLength = 1.0f;
	//For each asteroid
	for(int nAsteroid = 0; nAsteroid < numAsteroids; nAsteroid++){
		//Add to current asteroids lifetime value
		asteroids[nAsteroid].life_time += fLapDifference;

		// If current asteroids lifetime is greater than screen length, reset
		if(asteroids[nAsteroid].GetLT() > asteroids[nAsteroid].speed)
		{
			asteroids[nAsteroid].life_time = 0.0f; //Resets run time
		}

		// Map current asteroids screen percent to its lifetime
		asteroids[nAsteroid].screen_percentage = MapValue(asteroids[nAsteroid].life_time, 0.0f, asteroids[nAsteroid].speed, 0.0f, 1.0f);

		// Get current asteroids bounding object class and see if it's colliding with the shield
		String tempName = m_pMeshMngr->GetNameOfInstanceByIndex(nAsteroid+2);
		//std::cout << tempName << std::endl;

		
		BoundingObjectClass* tempBO = m_pMeshMngr->GetBoundingObject(tempName);
		BoundingObjectClass* shieldObject = m_pMeshMngr->GetBoundingObject("Pig");

		float randomY = tempBO->GetCentroidGlobal().y - tempBO->GetCentroidLocal().y;
		if(shieldObject->IsColliding(*tempBO))
		{
			color = MEBLACK;
			std::cout << "Colliding with: "<< tempName << std::endl;
			asteroids[nAsteroid].life_time = 0.0f;
			asteroids[nAsteroid].screen_percentage = 0.0f;

			float speed = rand() % 5 + 3;
			float direction = rand() % 2;
			asteroids[nAsteroid].speed = speed;
			asteroids[nAsteroid].go_right = direction;

			randomY = rand() % (int)(height) + (int)(-half_height);
			//numAsteroids--;
			//tempBO->SetVisible(false);
		}
		
		vector3 v3Lerp;
		// Map the lerp vectors x value with the current asteroids screen percentage
		if(asteroids[nAsteroid].go_right){
			v3Lerp.x = MapValue(asteroids[nAsteroid].screen_percentage, 0.0f, 1.0f, -half_width, half_width);
		} else{
			v3Lerp.x = MapValue(asteroids[nAsteroid].screen_percentage, 0.0f, 1.0f, half_width, -half_width);
		}
		v3Lerp.y = randomY;
		v3Lerp.z = 0.0f;
		
		// Send the mesh manager the current asteroids lerp vector position
		m_pMeshMngr->SetModelMatrix(glm::translate(v3Lerp), tempName);
	}

	// Jared's shit
	 OctDectection();

	//OCTREE
	//Top-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, height/4,2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, height/4,-2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	//Cubes inside top right
	/*m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), (half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);*/

	//Top-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, height/4,2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, height/4,-2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	//Cubes inside top left
	/*m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), (half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), (half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);*/

	//Bottom-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, -height/4,2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/4, -height/4,-2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	//Cubes inside bottom right
	/*m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, -height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(width/8, -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3((half_width -(half_width/4)), -(half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), color, WIRE);*/

	//Bottom-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, -height/4,2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/4, -height/4,-2.5))* glm::scale(vector3(width/2, height/2, 5)), color, WIRE);
	//Cubes inside bottom left
	/*m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, -height/8,-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-width/8, -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), -(half_height-(half_height/4)),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(-(half_width -(half_width/4)), -(half_height/4),-2.5))* glm::scale(vector3(width/4, height/4, 10)), MERED, WIRE);*/


	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}

void ApplicationClass::OctDectection(/*BoundingObjectClass* dude*/)
{
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
		
	int numModels = pMeshMngr->GetNumberOfModels();

	vector3 m_v3Centroid = vector3(0,0,0);
	vector3 v3Max = vector3(0,0,0);
	vector3 v3Min = vector3(0,0,0);

	vector3 boxSize = vector3(0,0,0);
	float halfWidthX;
	float halfWidthY;
	float halfWidthZ;

	for(int model = 0; model < numModels; model++){
		String bs = pMeshMngr->GetNameOfInstanceByIndex(model);
		BoundingObjectClass* bo = pMeshMngr->GetBoundingObject(bs);

		if(v3Min.x > bo->GetCentroidGlobal().x)
			v3Min.x = bo->GetCentroidGlobal().x;
		else if(v3Max.x < bo->GetCentroidGlobal().x)
			v3Max.x = bo->GetCentroidGlobal().x;
			
		if(v3Min.y > bo->GetCentroidGlobal().y)
			v3Min.y = bo->GetCentroidGlobal().y;
		else if(v3Max.y < bo->GetCentroidGlobal().y)
			v3Max.y = bo->GetCentroidGlobal().y;

		if(v3Min.z > bo->GetCentroidGlobal().z)
			v3Min.z = bo->GetCentroidGlobal().z;
		else if(v3Max.z < bo->GetCentroidGlobal().z)
			v3Max.z = bo->GetCentroidGlobal().z;

		halfWidthX = bo->HalfWidth.x;
		halfWidthY = bo->HalfWidth.y;
		halfWidthZ = bo->HalfWidth.z;

	}

	m_v3Centroid = (v3Min + v3Max) / 2.0f;

	boxSize.x = glm::distance(vector3(v3Min.x, 0.0f, 0.0f), vector3(v3Max.x, 0.0f, 0.0f));
	boxSize.y = glm::distance(vector3(0.0f, v3Min.y, 0.0f), vector3(0.0f, v3Max.y, 0.0f));
	boxSize.z = glm::distance(vector3(0.0f, 0.0f, v3Min.z), vector3(0.0f, 0.0f, v3Max.z));

	//boxSize;

	m_pMeshMngr->AddCubeToQueue(glm::translate(m_v3Centroid) * glm::scale(boxSize), MERED, WIRE);

	//Top Right
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z/2)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z)), MERED, WIRE);

	//Top-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z/2)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z)), MERED, WIRE);

	//Bottom-Right corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z/2)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z)), MERED, WIRE);

	//Bottom-Left corner
	m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z/2)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z)), MERED, WIRE);
}

