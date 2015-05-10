#include "ApplicationClass.h"
#include "Asteroid.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\MC_Ship.obj", "Ship");
	m_sShipObject = "Ship";
	//vector4 stevePos = static_cast<vector4>(m_pMeshMngr->GetModelMatrix("Steve"));
	
	//= m_pMeshMngr->GetInstanceByName("Steve");
	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\Shield_updated.obj", "Shield");
	m_sShieldObject = "Shield";
	bool clockwise = true;

	srand (time(NULL));	 

	// Steve and Pig Bounding Object Classes
	shipObject = m_pMeshMngr->GetBoundingObject("Ship");
	shipObject->SetVisibleAABB(true);
	shieldObject = m_pMeshMngr->GetBoundingObject("Shield");
	shieldObject->SetVisibleAABB(true);

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
	float fLapDifference = m_pSystem->LapClock();
	fTotalTime += fLapDifference;
	fRunTime += fLapDifference;
	stroidTime += fLapDifference;

	if(fTotalTime >= 60.0f){
		maxAsteroids = 10;
	}

	
	vector3 pigPos = shipObject->GetCentroidGlobal();
	//pigPos.y = pigPos.y - shipObject->GetHalfWidth().y;

	// Pig translate matrixes
	matrix4 sTranslate = glm::translate(vector3(2.0f, 2.0f, 0.0f));
	matrix4 sRotate = glm::rotate(matrix4(IDENTITY), degreeSpin * 2, vector3(0.0f, 0.0f, 1.0f));
	matrix4 sOrbit = glm::rotate(matrix4(IDENTITY), -degreeSpin * 2, vector3(0.0f, 0.0f, 1.0f));
	
	// Pig combined translate matrix
	matrix4 m_m4Pig = sRotate * sTranslate * sOrbit * glm::translate(pigPos);
	
	// Set Pig Model Matrix
	m_pMeshMngr->SetModelMatrix(m_m4ShipObject, m_sShipObject);
	m_pMeshMngr->SetModelMatrix(m_m4Pig, "Shield");
	
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
		float scale = rand() % 5 + 3;
		asteroids.push_back(Asteroid(pos, speed, scale, direction));
	
		//Add new asteroid to the screen and add a lifetime float and screen percent float to the arrays
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\Asteroid.obj", "Asteroid" + std::to_string(asteroids.size()), glm::translate(vector3(xPos, yPos, 0.0f))));
		//std::cout << "Creeper" + std::to_string(asteroids.size()) << std::endl;

		//Reset timer
		stroidTime = 0.0f;
	}

	//Init local variables
	vector3 color = MERED;
	float ScreenLength = 1.0f;

	//For each asteroid
	for(int nAsteroid = 0; nAsteroid < numAsteroids; nAsteroid++){
		/*
			Initialization for this loops variables
		*/
		String tempName = m_pMeshMngr->GetNameOfInstanceByIndex(nAsteroid+2);
		BoundingObjectClass* tempBO = m_pMeshMngr->GetBoundingObject(tempName);
		tempBO->SetVisibleAABB(true);
		float randomY = tempBO->GetCentroidGlobal().y - tempBO->GetCentroidLocal().y;

		/*
			Logic to handle asteroid map value for screen movement
			Add to current asteroids lifetime value based on fLapDifference
			If current asteroids lifetime is greater than screen length, reset lifetime and allow for it to collide again
			Then map current asteroids screen percent to its lifetime
		*/
		asteroids[nAsteroid].life_time += fLapDifference;
		if(asteroids[nAsteroid].GetLT() > asteroids[nAsteroid].speed)
		{
			asteroids[nAsteroid].life_time = 0.0f; //Resets run time
			asteroids[nAsteroid].colliding = false;
		}
		asteroids[nAsteroid].screen_percentage = MapValue(asteroids[nAsteroid].life_time, 0.0f, asteroids[nAsteroid].speed, 0.0f, 1.0f);

		/*
			Logic to handle ship collision with asteroid
			Checks for collision with current asteroid
			Updates color based on ship's health
		*/
		if(shipObject->IsColliding(*tempBO) && !asteroids[nAsteroid].colliding){
			asteroids[nAsteroid].colliding = true;
			shipHealth--;
		}

		switch(shipHealth){
		case 2:
			m_pMeshMngr->SetShaderProgramByName(m_sShipObject, "MonoChrome", MEYELLOW);
			break;
		case 1:
			m_pMeshMngr->SetShaderProgramByName(m_sShipObject, "MonoChrome", MERED);
			break;
		case 0:
			m_pMeshMngr->SetShaderProgramByName(m_sShipObject, "MonoChrome", MEBLACK);
			break;
		}

		/*
			Logic to handle shield collision with asteroid
			Checks for collision with current asteroid
			Resets asteroids values to "create a new asteroid"
		*/
		if(shieldObject->IsColliding(*tempBO) && !asteroids[nAsteroid].colliding)
		{
			color = MEBLACK;

			asteroids[nAsteroid].life_time = 0.0f;
			asteroids[nAsteroid].screen_percentage = 0.0f;

			float speed = rand() % 5 + 3;
			float direction = rand() % 2;
			asteroids[nAsteroid].speed = speed;
			asteroids[nAsteroid].go_right = direction;

			randomY = rand() % (int)(height) + (int)(-half_height);
		}

		// Make asteroid blink right after colliding
		// TODO: Rework
		if(asteroids[nAsteroid].colliding){
			if((int)asteroids[nAsteroid].life_time % 2 == 0){
				m_pMeshMngr->SetShaderProgramByName(tempName, "MonoChrome", MEBLACK);
			}
			else{
				m_pMeshMngr->SetShaderProgramByName(tempName);
			}
		}
		else{
			m_pMeshMngr->SetShaderProgramByName(tempName);
		}
		
		/*
			Create vector3 variable which will be set to the translation coordinates of the asteroid model
			Map the vector3 variables x component to the screen percentage
			Make x component go from left to right or right to left depending on the asteroids direction boolean
			Set y component to randomY, which is the the asteroids y value, either already set or reset due to prior collision
		*/
		vector3 v3Lerp;
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

	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}

void ApplicationClass::OctDectection(/*BoundingObjectClass* dude*/)
{		
	int numModels = m_pMeshMngr->GetNumberOfInstances();

	vector3 m_v3Centroid = vector3(0,0,0);
	vector3 v3Max = vector3(0,0,0);
	vector3 v3Min = vector3(0,0,0);

	vector3 boxSize = vector3(0,0,0);
	float halfWidthX;
	float halfWidthY;
	float halfWidthZ;

	for(int model = 0; model < numModels; model++){
		String bs = m_pMeshMngr->GetNameOfInstanceByIndex(model);
		BoundingObjectClass* bo = m_pMeshMngr->GetBoundingObject(bs);

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

	//Creating first subdivision
	vector3 subdivision1;
	subdivision1.x = (m_v3Centroid.x + boxSize.x/4);
	subdivision1.y = (m_v3Centroid.y + boxSize.y/4);
	subdivision1.z = (boxSize.z/4);

	vector3 subdivision2;
	subdivision2.x = boxSize.x/8;
	subdivision2.y = boxSize.y/8;
	subdivision2.z = boxSize.z/8;


	//Giving cubes boudning boxes
	//m_m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(2)), MERED, WIRE);
	BoundingObjectClass* box1 = new BoundingObjectClass(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4)),2);
	BoundingObjectClass* box2 = new BoundingObjectClass(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4)),2);
	BoundingObjectClass* box3 = new BoundingObjectClass(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4)),2);
	BoundingObjectClass* box4 = new BoundingObjectClass(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4)),2);

	//Add cubes to a vector
	std::vector<BoundingObjectClass*> bBoxes;
	bBoxes.push_back(box1);
	bBoxes.push_back(box2);
	bBoxes.push_back(box3);
	bBoxes.push_back(box4);

	//Whole cube
	m_pMeshMngr->AddCubeToQueue(glm::translate(m_v3Centroid) * glm::scale(boxSize), MERED, WIRE);

	//Top Right
	if(b1Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
	else if(!b1Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);

		//Second subdivision
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + subdivision2.x, m_v3Centroid.x + subdivision2.y, subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);


	//Top-Left corner
	if(b2Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
	else if(!b2Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);

		//Second subdivision
		//m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - subdivision2.x, m_v3Centroid.x + subdivision2.y, subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);

	//Bottom-Right corner
	if(b3Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
	else if(!b3Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);

	//Bottom-Left corner
	if(b4Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
	else if(!b4Color)
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);


	//Getting ship object
	BoundingObjectClass* shipObject = m_pMeshMngr->GetBoundingObject("Ship");
	
	//Checking if ship and asteroid are in the same area in the octTree
	for(int i = 0; i < 4; i++)
	{
		if(shipObject->IsColliding(*bBoxes[i]))
		{
			//If it is num i box then change color
			if(i == 0)
			{
				b1Color = true;
				b2Color = false;
				b3Color = false;
				b4Color = false;
			}
			else if(i == 1)
			{
				b2Color = true;
				b1Color = false;
				b3Color = false;
				b4Color = false;
			}
			else if(i == 2)
			{
				b3Color = true;
				b1Color = false;
				b2Color = false;
				b4Color = false;
			}
			else if(i == 3)
			{
				b4Color = true;
				b1Color = false;
				b2Color = false;
				b3Color = false;
			}
		}
	}

}

