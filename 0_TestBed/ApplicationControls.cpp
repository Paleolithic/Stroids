#include "ApplicationClass.h"
void ApplicationClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.1f;

	m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
	m_m4ShieldObject = m_pMeshMngr->GetModelMatrix(m_sShieldObject);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//To correctly terminate the application releasing memory
		PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL); 
		//exit(0); //will not terminate cleanly at this point
	}

	//Modifiers
#pragma region Modifiers

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		fSpeed += 1.0f;
		bModifier = true;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
	{
		fSpeed += 1.0f;
		bModifier = true;
	}
	
#pragma endregion

	static bool bPressedF5 = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
		bPressedF5 = true;
	else 
	{
		if(bPressedF5 == true)
		{
			m_pMeshMngr->m_pModelMngr->GenerateOctree();
		}
		bPressedF5 = false;
	}

	static bool bPressedF6 = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
		bPressedF6 = true;
	else 
	{
		if(bPressedF6 == true)
		{
			static bool bVisible = true;
			m_pMeshMngr->m_pModelMngr->SetVisibleOctree(bVisible);
			bVisible = !bVisible;
		}
		bPressedF6 = false;
	}

	

	//Model Positioning
#pragma region Model Positioning
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{		
		degreeDir = 0;
		if(bModifier)
			m_m4ShipObject = glm::rotate(m_m4ShipObject, 1.0f, vector3(0.0f, -1.0f, 0.0f));
		else{
			m_m4ShipObject = glm::translate(matrix4(), vector3(-fSpeed, 0.0f, 0.0f)) * m_m4ShipObject;
			is_left = true;
			 if(is_up){
				degreeDir = 270;
				is_up = false;
			}
			else if(is_right){
				degreeDir = 180;
				is_right = false;
			}
			else if(is_down){
				degreeDir = 90;
				is_down = false;
			}
			m_m4ShipObject = glm::rotate(m_m4ShipObject, degreeDir, vector3(0.0f, 1.0f, 0.0f));
		}	
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		degreeDir = 0.0f;
		if(bModifier)
			m_m4ShipObject = glm::rotate(m_m4ShipObject, 1.0f, vector3(0.0f, 1.0f,0.0f));
		else{
			m_m4ShipObject = glm::translate(matrix4(), vector3( fSpeed, 0.0f, 0.0f)) * m_m4ShipObject;
			is_right = true;
			if(is_down){
				degreeDir = 270;
				is_down = false;
			}
			else if(is_left){
				degreeDir = 180;
				is_left = false;
			}
			else if(is_up){
				degreeDir = 90;
				is_up = false;
			}
			m_m4ShipObject = glm::rotate(m_m4ShipObject, degreeDir, vector3(0.0f, 1.0f, 0.0f));
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		degreeDir = 0.0f;
		if(bModifier)
			m_m4ShipObject = glm::translate(matrix4(), vector3(0.0f, 0.0f, fSpeed - 1.0f)) * m_m4ShipObject;
		else{
			m_m4ShipObject = glm::translate(matrix4(), vector3(0.0f, fSpeed, 0.0f)) * m_m4ShipObject;
			is_up = true;
			if(is_right){
				degreeDir = 270;
				is_right = false;
			}
			else if(is_down){
				degreeDir = 180;
				is_down = false;
			}
			else if(is_left){
				degreeDir = 90;
				is_left = false;
			}
			m_m4ShipObject = glm::rotate(m_m4ShipObject, degreeDir, vector3(0.0f, 1.0f, 0.0f));
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		degreeDir = 0.0f;
		if(bModifier)
			m_m4ShipObject = glm::translate(matrix4(), vector3(0.0f, 0.0f,-fSpeed + 1.0f)) * m_m4ShipObject;
			
		else{
			m_m4ShipObject = glm::translate(matrix4(), vector3(0.0f,-fSpeed, 0.0f)) * m_m4ShipObject;
			is_down = true;
			 if(is_left){
				degreeDir = 270;
				is_left = false;
			}
			else if(is_up){
				degreeDir = 180;
				is_up = false;
			}
			else if(is_right){
				degreeDir = 90;
				is_right = false;
			}
			m_m4ShipObject = glm::rotate(m_m4ShipObject, degreeDir, vector3(0.0f, 1.0f, 0.0f));
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
		//Go Counter-clockwise
		degreeSpin++;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
		//Go Clockwise
		degreeSpin--;
	}

#pragma endregion
	////ModelSelection
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	//{
	//	m_sShipObject = "Cow";
	//	m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
	//}

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	//{
	//	m_sShipObject = "Zombie";
	//	m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
	//}
	//
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
	//{
	//	m_sShipObject = "Creeper";
	//	m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
	//}

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
	//{
	//	m_sShipObject = "Steve";
	//	m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
	//}

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	//{
	//	m_sShipObject = "Pig";
	//	m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
	//}

	//Camera
#pragma region Camera
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_pCamera->MoveForward(fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_pCamera->MoveForward(-fSpeed);
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_pCamera->MoveSideways(-fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_pCamera->MoveSideways(fSpeed);
	m_pCamera->CalculateView();
#pragma endregion
}
void ApplicationClass::ProcessMouse(void)
{
	m_bArcBall = false;
	m_bFPC = false;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		m_bArcBall = true;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_bFPC = true;

	/*static bool bLeft_Released = false;
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		bLeft_Released = true;
	}
	else
	{
		if(bLeft_Released == true)
		{
			m_pMeshMngr->SetVisibleBoundingObject(false);
			vector2 v2Object = m_pMeshMngr->m_pModelMngr->ShootRay(
														sf::Mouse::getPosition().x,
														sf::Mouse::getPosition().y,
														m_pCamera,
														false
													);
			String sInstance = m_pMeshMngr->GetNameOfInstanceByIndex(static_cast<int>(v2Object.x));
			if(sInstance != "")
			{
				m_pMeshMngr->SetVisibleBoundingObject(true, sInstance);
				m_sShipObject = sInstance;
				m_m4ShipObject = m_pMeshMngr->GetModelMatrix(m_sShipObject);
				m_pMeshMngr->SetVisibleBoundingObjectHierarchy(true, m_sShipObject);
			}
		}
		bLeft_Released = false;
	}*/
}
void ApplicationClass::ProcessJoystick(void)
{
	sf::Joystick::update();

	int nPad = 0;
	if(sf::Joystick::isConnected(nPad))
	{
		if(sf::Joystick::isButtonPressed(nPad, 4) && sf::Joystick::isButtonPressed(nPad, 5))
			exit(0);

		static float fAngleX = 0.0f;
		static float fAngleY = 0.0f;

		if(sf::Joystick::isButtonPressed(nPad, 8) && sf::Joystick::isButtonPressed(nPad, 9))	
		{
			fAngleX = 0.0f;
			fAngleY = 0.0f;
			m_pCamera->SetPosition(glm::vec3( 0.0f, 0.0f, 10.0f));
		}

		float fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::Y);
		if(fDelta > 20 || fDelta < -20)
		{
			fDelta /= 1000.0f;
			m_pCamera->MoveForward(-fDelta);
		}
		
		fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::X);
		if(fDelta > 20 || fDelta < -20)
		{
			fDelta /= 1000.0f;
			m_pCamera->MoveSideways(fDelta);
		}

		fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::Z);
		if(fDelta > 20 || fDelta < -20)
		{
			fDelta /= 1000.0f;
			m_pCamera->MoveVertical(fDelta);
		}

		fDelta = -sf::Joystick::getAxisPosition(nPad, sf::Joystick::R);
		if(fDelta > 25 || fDelta < -25)
		{
			fAngleX -= fDelta/10000.0f;
		}
		
		fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::U);
		if(fDelta > 25 || fDelta < -25)
		{
			fAngleY -= fDelta/10000.0f;
		}

		//m_pCamera->Rotate(fAngleX, fAngleY);
		m_pCamera->CalculateView();
		
	}
}
void ApplicationClass::CameraRotation(float a_fSpeed)
{
	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

	CenterX = m_pSystem->WindowX + m_pSystem->WindowWidth / 2;
	CenterY = m_pSystem->WindowY + m_pSystem->WindowHeight / 2;
	
	float DeltaMouse;
	POINT pt;

	GetCursorPos(&pt);
	
	MouseX = pt.x;
	MouseY = pt.y;

	SetCursorPos(CenterX, CenterY);

	static float fAngleX = 0.0f;
	static float fAngleY = 0.0f;

	if(MouseX < CenterX)
	{
		DeltaMouse = static_cast<float>(CenterX - MouseX);
		fAngleY += DeltaMouse * a_fSpeed;
	}
	else if(MouseX > CenterX)
	{
		DeltaMouse = static_cast<float>(MouseX - CenterX);
		fAngleY -= DeltaMouse * a_fSpeed;
	}

	if(MouseY < CenterY)
	{
		DeltaMouse = static_cast<float>(CenterY - MouseY);
		fAngleX -= DeltaMouse * a_fSpeed;
	}
	else if(MouseY > CenterY)
	{
		DeltaMouse = static_cast<float>(MouseY - CenterY);
		fAngleX += DeltaMouse * a_fSpeed;
	}
	//m_pCamera->Rotate(fAngleX, fAngleY);
	m_pCamera->ChangeHeading(fAngleY * 3.0f);//fAngleY and fAngleX are no longer static, the value is saved inside of the camera object
	m_pCamera->ChangePitch(-fAngleX * 3.0f);
}
void ApplicationClass::ArcBall(float a_fSensitivity)
{
	static matrix4 arcball;
	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

	CenterX = m_pSystem->WindowX + m_pSystem->WindowWidth / 2;
	CenterY = m_pSystem->WindowY + m_pSystem->WindowHeight / 2;
	
	float DeltaMouse;
	POINT pt;

	GetCursorPos(&pt);
	
	MouseX = pt.x;
	MouseY = pt.y;

	SetCursorPos(CenterX, CenterY);

	static float fVerticalAngle = 0.0f;
	static float fHorizontalAngle = 0.0f;

	float fSpeed = 0.001f;

	if(MouseX < CenterX)
	{
		DeltaMouse = static_cast<float>(CenterX - MouseX);
		arcball = glm::rotate(arcball, a_fSensitivity * DeltaMouse, vector3(0.0f, 1.0f, 0.0f));
	}
	else if(MouseX > CenterX)
	{
		DeltaMouse = static_cast<float>(MouseX - CenterX);
		arcball = glm::rotate(arcball, -a_fSensitivity * DeltaMouse, vector3(0.0f, 1.0f, 0.0f));
	}

	if(MouseY < CenterY)
	{
		DeltaMouse = static_cast<float>(CenterY - MouseY);
		arcball = glm::rotate(arcball, a_fSensitivity * DeltaMouse, vector3(1.0f, 0.0f, 0.0f));
	}
	else if(MouseY > CenterY)
	{
		DeltaMouse = static_cast<float>(MouseY - CenterY);
		arcball = glm::rotate(arcball, -a_fSensitivity * DeltaMouse, vector3(1.0f, 0.0f, 0.0f));
	}
	m_m4ShipObject = arcball;
}