#include "ApplicationClass.h"
#include "Asteroid.h"
void ApplicationClass::InitUserAppVariables()
{
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\Ship_Shrunk.obj", "Ship", glm::rotate(matrix4(IDENTITY), -90.0f, vector3(1.0f, 0.0f, 0.0f)));
	m_sShipObject = "Ship";

	m_pMeshMngr->LoadModelUnthreaded("Minecraft\\Shield_updated.obj", "Shield");
	m_sShieldObject = "Shield";

	srand (time(NULL));	 

	// Ship and Shield Bounding Object Classes
	shipObject = m_pMeshMngr->GetBoundingObject("Ship");
	shipObject->SetVisibleAABB(true);
	shieldObject = m_pMeshMngr->GetBoundingObject("Shield");
	shieldObject->SetVisibleAABB(true);
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
		maxAsteroids = 16;
	}
	
	vector3 shipPos = shipObject->GetCentroidGlobal();

	// Shield translate matrixes
	matrix4 sTranslate = glm::translate(vector3(1.15f, 1.15f, 0.0f));
	matrix4 sRotate = glm::rotate(matrix4(IDENTITY), degreeSpin * 2, vector3(0.0f, 0.0f, 1.0f));
	matrix4 sOrbit = glm::rotate(matrix4(IDENTITY), -degreeSpin * 2, vector3(0.0f, 0.0f, 1.0f));
	
	// Shield combined translate matrix
	matrix4 m_m4Shield = sRotate * sTranslate * sOrbit * glm::translate(shipPos);
	
	// Set Shield Model Matrix
	m_pMeshMngr->SetModelMatrix(m_m4ShipObject, m_sShipObject);
	m_pMeshMngr->SetModelMatrix(m_m4Shield, "Shield");

	
	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();

	// CATHY SHIT
	// Do the following every two seconds as long as number of asteroids is less than max
	if(stroidTime > 2.0f && asteroids.size() < maxAsteroids)
	{
		
		//Set initial random x and y position for asteroids
		float xPos = rand() % (int)(width) + (int)(-width);
		float yPos = rand() % (int)(height) + (int)(-half_height);
		vector3 pos = vector3(xPos, yPos, 0.0f);
		
		float speed = rand() % 5 + 3;
		String name = "Asteroid" + std::to_string(asteroids.size());
		float direction = rand() % 2;
		//Add new asteroid to the screen and add a lifetime float and screen percent float to the arrays
		m_pMeshMngr->LoadModelUnthreaded("Minecraft\\Asteroid.obj", name, glm::translate(vector3(xPos, yPos, 0.0f)));
		asteroids.push_back(new Asteroid(speed, name, direction, m_pMeshMngr->GetBoundingObject(name)));

		//Reset timer
		stroidTime = 0.0f;
	}

	//Init local variables
	vector3 color = MERED;
	float ScreenLength = 1.0f;

	// For each asteroid
	for(int nAsteroid = 0; nAsteroid < asteroids.size(); nAsteroid++){
		String tempName = asteroids[nAsteroid]->name;
		BoundingObjectClass* tempBO = asteroids[nAsteroid]->aBO;
		tempBO->SetVisibleAABB(true);
		asteroids[nAsteroid]->rand_Y = tempBO->GetCentroidGlobal().y - tempBO->GetCentroidLocal().y;
		/*
			Logic to handle asteroid map value for screen movement
			Add to current asteroids lifetime value to its lifetime
		*/
		asteroids[nAsteroid]->life_time += fLapDifference;
		if(asteroids[nAsteroid]->GetLT() > asteroids[nAsteroid]->speed)
		{
			asteroids[nAsteroid]->life_time = 0.0f; //Resets run time
			asteroids[nAsteroid]->colliding = false;
		}
		asteroids[nAsteroid]->screen_percentage = MapValue(asteroids[nAsteroid]->life_time, 0.0f, asteroids[nAsteroid]->speed, 0.0f, 1.0f);

		float friction = 0.02f;
		if(asteroids[nAsteroid]->isSlowed){
			vector3 currentPos = asteroids[nAsteroid]->aBO->GetCentroidGlobal();
			currentPos = currentPos * friction;

			if(shieldObject->GetCentroidGlobal().y <= asteroids[nAsteroid]->aBO->GetCentroidGlobal().y){
				asteroids[nAsteroid]->rand_Y += 0.25f;
			}
			else{
				asteroids[nAsteroid]->rand_Y -= 0.25f;
			}

			if(asteroids[nAsteroid]->rand_Y >= height/2 || asteroids[nAsteroid]->rand_Y <= -height/2)
			{
				asteroids[nAsteroid]->life_time = 0.0f;
				asteroids[nAsteroid]->screen_percentage = 0.0f;

				float speed = rand() % 5 + 3;
				float direction = rand() % 2;
				asteroids[nAsteroid]->speed = speed;
				asteroids[nAsteroid]->go_right = direction;
				asteroids[nAsteroid]->rand_Y = rand() % (int)(height) + (int)(-half_height);
				asteroids[nAsteroid]->isSlowed = false;
				asteroids[nAsteroid]->life_time = 0.0f; //Resets run time
				asteroids[nAsteroid]->colliding = false;
			}
		}

		/*
			Create vector3 variable which will be set to the translation coordinates of the asteroid model
			Map the vector3 variables x component to the screen percentage
			Make x component go from left to right or right to left depending on the asteroids direction boolean
			Set y component to randomY, which is the the asteroids y value, either already set or reset due to prior collision
		*/
		vector3 v3Lerp;
		if(asteroids[nAsteroid]->go_right){
			v3Lerp.x = MapValue(asteroids[nAsteroid]->screen_percentage, 0.0f, 1.0f, -half_width, half_width);
		} else{
			v3Lerp.x = MapValue(asteroids[nAsteroid]->screen_percentage, 0.0f, 1.0f, half_width, -half_width);
		}
		v3Lerp.y = asteroids[nAsteroid]->rand_Y;
		v3Lerp.z = 0.0f;

		m_pMeshMngr->SetModelMatrix(glm::translate(v3Lerp), tempName);
	}

	//For each near asteroid
	for(int nAsteroid = 0; nAsteroid < nearAsteroids.size(); nAsteroid++){
		/*
			Initialization for this loops variables
		*/
		String tempName = nearAsteroids[nAsteroid]->name;
		BoundingObjectClass* tempBO = nearAsteroids[nAsteroid]->aBO;
		
		/*
			Logic to handle ship collision with asteroid
			Checks for collision with current asteroid
			Updates color based on ship's health

			Also updates life time and direction to make asteroid go in opposite direction
		*/
		if(shieldObject->IsColliding(*tempBO) && !nearAsteroids[nAsteroid]->colliding){
			nearAsteroids[nAsteroid]->colliding = true;
			nearAsteroids[nAsteroid]->isSlowed  = true;
			nearAsteroids[nAsteroid]->life_time = nearAsteroids[nAsteroid]->speed - nearAsteroids[nAsteroid]->life_time; 

			if(nearAsteroids[nAsteroid]->go_right){
				nearAsteroids[nAsteroid]->go_right = false;
			} else{
				nearAsteroids[nAsteroid]->go_right = true;
			}
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
			Resets nearAsteroids values to "create a new asteroid"
		*/
		if(shipObject->IsColliding(*tempBO) && !nearAsteroids[nAsteroid]->colliding)
		{
			color = MEBLACK;
			shipHealth--;
			nearAsteroids[nAsteroid]->life_time = 0.0f;
			nearAsteroids[nAsteroid]->screen_percentage = 0.0f;
			
			float speed = rand() % 5 + 3;
			float direction = rand() % 2;
			nearAsteroids[nAsteroid]->speed = speed;
			nearAsteroids[nAsteroid]->go_right = direction;
			nearAsteroids[nAsteroid]->rand_Y = rand() % (int)(height) + (int)(-half_height);
		}

		// Make asteroid blink right after colliding
		// TODO: Rework
		if(nearAsteroids[nAsteroid]->colliding){
			if((int)nearAsteroids[nAsteroid]->life_time % 2 == 0){
				m_pMeshMngr->SetShaderProgramByName(tempName, "MonoChrome", MEBLACK);
				
			}
			else{
				m_pMeshMngr->SetShaderProgramByName(tempName);
			}
		}
		else{
			m_pMeshMngr->SetShaderProgramByName(tempName);
		}
	}

	// Jared's shit
	OctDectection();

	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second	
}

void ApplicationClass::OctDectection(/*BoundingObjectClass* dude*/)
{		
	// Setting up vector3 variables for octree box
	vector3 m_v3Centroid = vector3(0,0,0);
	vector3 v3Max = vector3(0,0,0);
	vector3 v3Min = vector3(0,0,0);
	vector3 boxSize = vector3(0,0,0);

	// First time boolean
	bool firstTime = true;

	int numModels = m_pMeshMngr->GetNumberOfInstances();
	// For each bounding object check to see if its min or max
	for(int model = 0; model < numModels; model++){
		String bs = m_pMeshMngr->GetNameOfInstanceByIndex(model);
		BoundingObjectClass* bo = m_pMeshMngr->GetBoundingObject(bs);

		if(v3Min.x > bo->GetCentroidGlobal().x - bo->GetHalfWidth().x)
			v3Min.x = bo->GetCentroidGlobal().x - bo->GetHalfWidth().x;
		else if(v3Max.x < bo->GetCentroidGlobal().x + bo->GetHalfWidth().x)
			v3Max.x = bo->GetCentroidGlobal().x + bo->GetHalfWidth().x;
			
		if(v3Min.y > bo->GetCentroidGlobal().y - bo->GetHalfWidth().y)
			v3Min.y = bo->GetCentroidGlobal().y - bo->GetHalfWidth().y;
		else if(v3Max.y < bo->GetCentroidGlobal().y + bo->GetHalfWidth().y)
			v3Max.y = bo->GetCentroidGlobal().y + bo->GetHalfWidth().y;

		if(v3Min.z > bo->GetCentroidGlobal().z - bo->GetHalfWidth().z)
			v3Min.z = bo->GetCentroidGlobal().z - bo->GetHalfWidth().z;
		else if(v3Max.z < bo->GetCentroidGlobal().z + bo->GetHalfWidth().z)
			v3Max.z = bo->GetCentroidGlobal().z + bo->GetHalfWidth().z;

		
	}
	// Get centroid
	m_v3Centroid = (v3Min + v3Max) / 2.0f;

	// The size of the box
	boxSize.x = glm::distance(vector3(v3Min.x, 0.0f, 0.0f), vector3(v3Max.x, 0.0f, 0.0f));
	boxSize.y = glm::distance(vector3(0.0f, v3Min.y, 0.0f), vector3(0.0f, v3Max.y, 0.0f));
	boxSize.z = glm::distance(vector3(0.0f, 0.0f, v3Min.z), vector3(0.0f, 0.0f, v3Max.z));

	//Creating first subdivision
	vector3 subdivision1;
	subdivision1.x = boxSize.x/4;
	subdivision1.y = boxSize.y/4;
	subdivision1.z = boxSize.z/4;

	//Creating first subdivision
	vector3 subdivision2;
	subdivision2.x = boxSize.x/8;
	subdivision2.y = boxSize.y/8;
	subdivision2.z = boxSize.z/8;

	//Top Right
	if(b1Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
		//Second subdivision
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + subdivision2.x, m_v3Centroid.y + subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + subdivision2.x, m_v3Centroid.y + (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y + (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y + subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
	}
	else if(!b1Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
	}


	//Top-Left corner
	if(b2Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
		
		//Second subdivision
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - subdivision2.x, m_v3Centroid.y + subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - subdivision2.x, m_v3Centroid.y + (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y + (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y + subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);

	}
	else if(!b2Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y + boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
	}


	//Bottom-Right corner
	if(b3Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
	
		//Second subdivision
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + subdivision2.x, m_v3Centroid.y - subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + subdivision2.x, m_v3Centroid.y - (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y - (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y - subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
	
	}
	else if(!b3Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x + boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		
	}


	//Bottom-Left corner
	if(b4Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MEBLACK, WIRE);
		
		//Second subdivision
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - subdivision2.x, m_v3Centroid.y - subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - subdivision2.x, m_v3Centroid.y - (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y - (subdivision2.y*4 - subdivision2.y), boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - (subdivision2.x*4 - subdivision2.x), m_v3Centroid.y - subdivision2.y, boxSize.z - subdivision2.z)) * glm::scale(vector3(boxSize.x/4, boxSize.y/4, boxSize.z/4)), MERED, WIRE);
	
	}
	else if(!b4Color)
	{
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z - (boxSize.z/4))) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);
		m_pMeshMngr->AddCubeToQueue(glm::translate(vector3(m_v3Centroid.x - boxSize.x/4, m_v3Centroid.y - boxSize.y/4, boxSize.z/4)) * glm::scale(vector3(boxSize.x/2, boxSize.y/2, boxSize.z/2)), MERED, WIRE);

	}

	//Getting ship object
	shipObject = m_pMeshMngr->GetBoundingObject("Ship");

	
	// Checking collision with subdivision1 and ship in top right corner
	if(glm::distance(m_v3Centroid.x + subdivision1.x, shipObject->CentroidGlobal.x) < (subdivision1.x + shipObject->HalfWidth.x)  &&
	   glm::distance(m_v3Centroid.y + subdivision1.y, shipObject->CentroidGlobal.y) < (subdivision1.y + shipObject->HalfWidth.y) &&
	   glm::distance(m_v3Centroid.z + subdivision1.z, shipObject->CentroidGlobal.z) < (subdivision1.z + shipObject->HalfWidth.z))
	{
		// For each asteroid, check to see if in subdivision
		for(int nAsteroid = 0; nAsteroid < asteroids.size(); nAsteroid++)
		{
			BoundingObjectClass* bo = asteroids[nAsteroid]->aBO;

			// If in subdivision and ship is there, create subdivision 2 and add appropriate asteroid to BoundingObject array
			if(glm::distance(m_v3Centroid.x + subdivision1.x, bo->CentroidGlobal.x) < (subdivision1.x + bo->HalfWidth.x)  &&
			   glm::distance(m_v3Centroid.y + subdivision1.y, bo->CentroidGlobal.y) < (subdivision1.y + bo->HalfWidth.y) &&
			   glm::distance(m_v3Centroid.z + subdivision1.z, bo->CentroidGlobal.z) < (subdivision1.z + bo->HalfWidth.z))
			{
				b1Color = true;
				b2Color = false;
				b3Color = false;
				b4Color = false;
				asteroids[nAsteroid]->isNearShip = true;

				//If its the first time checking this subsection clear asteroid vector
				if(firstTime)
				{
					nearAsteroids.clear();
					firstTime = false;
				}
		
				nearAsteroids.push_back(asteroids[nAsteroid]);
			}
			else{
				asteroids[nAsteroid]->isNearShip = false;
			}
		}
	}
	// Checking collision with subdivision1 and ship in top left corner
	else if(glm::distance(m_v3Centroid.x - subdivision1.x, shipObject->CentroidGlobal.x) < (subdivision1.x + shipObject->HalfWidth.x)  &&
			glm::distance(m_v3Centroid.y + subdivision1.y, shipObject->CentroidGlobal.y) < (subdivision1.y + shipObject->HalfWidth.y) &&
			glm::distance(m_v3Centroid.z + subdivision1.z, shipObject->CentroidGlobal.z) < (subdivision1.z + shipObject->HalfWidth.z))
	{
		// For each asteroid, check to see if in subdivision
		for(int nAsteroid = 0; nAsteroid < asteroids.size(); nAsteroid++)
		{
			BoundingObjectClass* bo = asteroids[nAsteroid]->aBO;

			// If in subdivision and ship is there, create subdivision 2 and add appropriate asteroid to BoundingObject array
			if(glm::distance(m_v3Centroid.x - subdivision1.x, bo->CentroidGlobal.x) < (subdivision1.x + bo->HalfWidth.x)  &&
			   glm::distance(m_v3Centroid.y + subdivision1.y, bo->CentroidGlobal.y) < (subdivision1.y + bo->HalfWidth.y) &&
			   glm::distance(m_v3Centroid.z + subdivision1.z, bo->CentroidGlobal.z) < (subdivision1.z + bo->HalfWidth.z))
			{
				b1Color = false;
				b2Color = true;
				b3Color = false;
				b4Color = false;				
				asteroids[nAsteroid]->isNearShip = true;
				

				if(firstTime)
				{
					nearAsteroids.clear();
					firstTime = false;
				}

				nearAsteroids.push_back(asteroids[nAsteroid]);
			}
			else{
				asteroids[nAsteroid]->isNearShip = false;
			}
		}
	}
	// Checking collision with subdivision1 and ship in bottom right corner
	else if(glm::distance(m_v3Centroid.x + subdivision1.x, shipObject->CentroidGlobal.x) < (subdivision1.x + shipObject->HalfWidth.x)  &&
			glm::distance(m_v3Centroid.y - subdivision1.y, shipObject->CentroidGlobal.y) < (subdivision1.y + shipObject->HalfWidth.y) &&
			glm::distance(m_v3Centroid.z + subdivision1.z, shipObject->CentroidGlobal.z) < (subdivision1.z + shipObject->HalfWidth.z))
	{
		// For each asteroid, check to see if in subdivision
		for(int nAsteroid = 0; nAsteroid < asteroids.size(); nAsteroid++)
		{
			BoundingObjectClass* bo = asteroids[nAsteroid]->aBO;
			
			// If in subdivision and ship is there, create subdivision 2 and add appropriate asteroid to BoundingObject array
			if(glm::distance(m_v3Centroid.x + subdivision1.x, bo->CentroidGlobal.x) < (subdivision1.x + bo->HalfWidth.x)  &&
			   glm::distance(m_v3Centroid.y - subdivision1.y, bo->CentroidGlobal.y) < (subdivision1.y + bo->HalfWidth.y) &&
			   glm::distance(m_v3Centroid.z + subdivision1.z, bo->CentroidGlobal.z) < (subdivision1.z + bo->HalfWidth.z))
			{
				b1Color = false;
				b2Color = false;
				b3Color = true;
				b4Color = false;
				asteroids[nAsteroid]->isNearShip = true;

				if(firstTime)
				{
					nearAsteroids.clear();
					firstTime = false;
				}

				nearAsteroids.push_back(asteroids[nAsteroid]);
			}
			else{
				asteroids[nAsteroid]->isNearShip = false;
			}
		}
	}
	// Checking collision with subdivision1 and ship in bottom left corner
	else if(glm::distance(m_v3Centroid.x - subdivision1.x, shipObject->CentroidGlobal.x) < (subdivision1.x + shipObject->HalfWidth.x)  &&
			glm::distance(m_v3Centroid.y - subdivision1.y, shipObject->CentroidGlobal.y) < (subdivision1.y + shipObject->HalfWidth.y) &&
			glm::distance(m_v3Centroid.z + subdivision1.z, shipObject->CentroidGlobal.z) < (subdivision1.z + shipObject->HalfWidth.z))
	{
		// For each asteroid, check to see if in subdivision
		for(int nAsteroid = 0; nAsteroid < asteroids.size(); nAsteroid++)
		{
			BoundingObjectClass* bo = asteroids[nAsteroid]->aBO;
			
			// If in subdivision and ship is there, create subdivision 2 and add appropriate asteroid to BoundingObject array
			if(glm::distance(m_v3Centroid.x - subdivision1.x, bo->CentroidGlobal.x) < (subdivision1.x + bo->HalfWidth.x)  &&
			   glm::distance(m_v3Centroid.y - subdivision1.y, bo->CentroidGlobal.y) < (subdivision1.y + bo->HalfWidth.y) &&
			   glm::distance(m_v3Centroid.z + subdivision1.z, bo->CentroidGlobal.z) < (subdivision1.z + bo->HalfWidth.z))
			{
				b1Color = false;
				b2Color = false;
				b3Color = false;
				b4Color = true;
				asteroids[nAsteroid]->isNearShip = true;

				if(firstTime)
				{
					nearAsteroids.clear();
					firstTime = false;
				}

				nearAsteroids.push_back(asteroids[nAsteroid]);
			}
			else{
				asteroids[nAsteroid]->isNearShip = false;
			}
		}
	}

	//Count for checking if ship has left subdivision
	int count = 0;
	//Loop to check all asteroids to see if any are near the ship
	for(int i = 0; i < asteroids.size(); i++)
	{
		//If the asteroid is not near the ship add to count
		if(!asteroids[i]->isNearShip)
		{
			count++;
		}
		//If the count is the same as the amount of asteroids on the screen then reset because none are near
		if(count == asteroids.size())
		{
			b1Color = false;
			b2Color = false;
			b3Color = false;
			b4Color = false;

			nearAsteroids.clear();
		}
	}

	std::cout << nearAsteroids.size() << std::endl;
}

