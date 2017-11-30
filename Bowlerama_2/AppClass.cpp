#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "The Backpacks - DSA2 Final Project";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(vector3(0.0f, 3.0f, 13.0f), //Position
		vector3(0.0f, 3.0f, 12.0f),	//Target
		AXIS_Y);					//Up

									//Set the position of the light
	m_pLightMngr->SetPosition(vector3(10.0f));

	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();



	plane = new Mesh();
	plane->GeneratePlane(30.0f, C_GRAY);

	pins = new Mesh[10];
	pinLocations = new vector3[10];


	uint row = 0;
	uint currentPin = 0;
	for (uint i = 0; i < 10; i++)
	{
		pins[i] = Mesh();
		pins[i].GenerateCylinder(0.3f, 1.5f, 8, C_RED);
		pinLocations[i] = vector3((-1.5f * row) + (3.0f * currentPin), -0.5f/*0.25f*/, -10.0f + (-1.5f * row));

		m_pEntityMngr->AddEntity("Bowlerama\\BowlingPin.obj", "Pin" + std::to_string(i));
		m_pEntityMngr->SetAxisVisibility(true, "Pin" + std::to_string(i)); //set visibility of the entity's axis

		

		currentPin++;

		if (currentPin == row + 1)
		{
			row++;
			currentPin = 0;
		}
	}
	m_pEntityMngr->AddEntity("Bowlerama\\BowlingBall.obj", "Bowling");
	m_pEntityMngr->SetAxisVisibility(true, "Bowling"); //set visibility of the entity's axis

	//bowlingBall = new Mesh();
	//bowlingBall->GenerateSphere(1.75f, 4, C_BLACK);

	bowlingBallWire = new Mesh();
	bowlingBallWire->GenerateWireSphere(0.9f, C_GREEN);
	
	m_pEntityMngr->UsePhysicsSolver(true, "Bowling");
	m_pEntityMngr->SetMass(5.0f, "Bowling");

	matrix4 pinModel1 = glm::translate(IDENTITY_M4, pinLocations[0]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel1, "Pin0");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin0");

	matrix4 pinModel2 = glm::translate(IDENTITY_M4, pinLocations[1]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel2, "Pin1");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin1");

	matrix4 pinModel3 = glm::translate(IDENTITY_M4, pinLocations[2]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel3, "Pin2");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin2");

	matrix4 pinModel4 = glm::translate(IDENTITY_M4, pinLocations[3]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel4, "Pin3");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin3");

	matrix4 pinModel5 = glm::translate(IDENTITY_M4, pinLocations[4]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel5, "Pin4");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin4");

	matrix4 pinModel6 = glm::translate(IDENTITY_M4, pinLocations[5]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel6, "Pin5");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin5");

	matrix4 pinModel7 = glm::translate(IDENTITY_M4, pinLocations[6]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel7, "Pin6");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin6");

	matrix4 pinModel8 = glm::translate(IDENTITY_M4, pinLocations[7]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel8, "Pin7");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin7");

	matrix4 pinModel9 = glm::translate(IDENTITY_M4, pinLocations[8]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel9, "Pin8");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin8");

	matrix4 pinModel10 = glm::translate(IDENTITY_M4, pinLocations[9]) * glm::scale(vector3(0.75f, 0.75f, 0.75f));
	m_pEntityMngr->SetModelMatrix(pinModel10, "Pin9");
	m_pEntityMngr->UsePhysicsSolver(true, "Pin9");


	//Load audio
	sf::Music* bgMusic = new sf::Music();
	if (!bgMusic->openFromFile("Data/Audio/BowleramaSounds/ChillWave.ogg"))
	{
		//Song failed to load
		std::cout << "BG Audio Failed to load!";
	}
	else
	{
		std::cout << "BG Audio loaded!";
		music.push_back(bgMusic);
	}

	sf::Music* ambience = new sf::Music();
	if (!ambience->openFromFile("Data/Audio/BowleramaSounds/AlleyAmbience.ogg"))
	{
		std::cout << "Ambience Audio Failed to load!";
	}
	else
	{
		std::cout << "Ambience Audio loaded!";
		music.push_back(ambience);
	}

	sf::Sound* rollingBall = new sf::Sound();
	sf::SoundBuffer* rollingBallBuffer = new sf::SoundBuffer();
	if (!rollingBallBuffer->loadFromFile("Data/Audio/BowleramaSounds/RollingBall.ogg"))
	{
		std::cout << "Rolling Ball Audio Failed to load!";
	}
	else
	{
		std::cout << "Rolling Ball Audio loaded!";
		rollingBall->setBuffer(*rollingBallBuffer);
		rollingBall->setLoop(true);
		rollingBall->setVolume(globalSoundVolume);
		soundBuffers.push_back(rollingBallBuffer);
		sounds.push_back(rollingBall);
	}

	for (uint i = 0; i < music.size(); i++)
	{
		music[i]->play();
		//I don't know why, but we need to loop manually to avoid that stupid thread breaking behavior
		music[i]->setLoop(true);
		music[i]->setVolume(globalMusicVolume);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//check for mouse press and get position to calculate force
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		pressed = true;
		GetCursorPos(&pt);
		startMouseX = pt.x;
		startMouseY = pt.y;
		force *= 0.8;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false)
	{
		if (pressed == true)
		{
			GetCursorPos(&pt);
			endMouseX = pt.x;
			endMouseY = pt.y;

			pushforce = vector3(endMouseX - startMouseX, 0.0f, endMouseY - startMouseY);

			force -= pushforce / 2.0f;

			pressed = false;
		}

	}

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//Get the projection and view matrices from the camera
	matrix4 projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 view = m_pCameraMngr->GetViewMatrix();

	//If the force vector is greater than nothing
	if (abs(force.x) > 0.0f || abs(force.y) > 0.0f || abs(force.z) > 0.0f)
	{
		//Find the magnitude of the force (equivalent to the rotation in degrees)
		float speed = glm::length(force);

		//Get the sphere's forward vector
		vector3 forward = glm::normalize(force);

		//Normalized forward vector * (degrees of rotation / 360 total degrees) * (Circumfrence of the sphere, 2PIr (radius is 1.0f))
		//Keep in case we need: position -= (force / ((2.0f * (float) PI) / ((speed * (float)PI) / 180.0f) / 2.0f));
		position -= forward * (speed / 360.0f) * (2.0f * (float)PI);

		//The ball's axis of rotation can be found by crossing the forward and up vectors
		vector3 rotationAxis = glm::cross(forward, AXIS_Y);

		//Create a new quaternion out of the rotation axis and the number of degrees to rotate
		quaternion forceRotation = glm::rotate(quaternion(), speed, rotationAxis);

		//Multiply the current orientation by the new rotation caused by the force
		currentOrientation = forceRotation * currentOrientation;

		//Translate the model, and then rotate it to the current orientation
		model = glm::translate(IDENTITY_M4, position);
		model = model * ToMatrix4(currentOrientation);

		//If the speed of the ball is less than a small value, just set it to (0, 0, 0)
		if (speed < 0.001f)
		{
			force = vector3();
		}

		//std::cout << sounds[0]->getStatus() << std::endl;
		//Play a ball rolling sound effect
		if (sounds[0]->getStatus() == sf::Sound::Status::Playing)
		{
			sounds[0]->setPitch(1.0f + speed / 10.0f);
		}
		else
		{
			sounds[0]->play();
		}

		//std::cout << "X: " << position.x << "  Y: " << position.y << "  Z: " << position.z << std::endl;
	}
	else
	{
		if (sounds[0]->getStatus() == sf::Sound::Status::Playing)
		{
			sounds[0]->stop();
		}
	}

	//Lessen the force over time due to friction
	force *= dampeningRate;

	//Render the bowling ball, ball wireframe and the plane
	//bowlingBall->Render(projection, view, model);

	m_pEntityMngr->SetModelMatrix(model, "Bowling");

	bowlingBallWire->Render(projection, view, glm::scale(model, vector3(1.2f)));
	plane->Render(projection, view, glm::translate(glm::rotate(IDENTITY_M4, -90.0f, AXIS_X), vector3(0.0f, 0.0f, -0.5f)) * glm::scale(vector3(0.35f, 1.0f, 1.0f)));

	//Render the pins
	/**/


	//Add grid to the scene
	//m_pMeshMngr->AddGridToRenderList();

	//Add skybox
	m_pMeshMngr->AddSkyboxToRenderList("Skybox.png");

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Delete all audio!
	for (uint i = 0; i < music.size(); i++)
	{
		SafeDelete(music[i]);
	}
	music.clear();

	for (uint i = 0; i < sounds.size(); i++)
	{
		SafeDelete(soundBuffers[i]);
		SafeDelete(sounds[i]);
	}
	sounds.clear();

	//Delete everything, and avoid memory leaks!
	SafeDelete(bowlingBall);
	SafeDelete(bowlingBallWire);
	SafeDelete(plane);
	delete[] pins;
	delete[] pinLocations;

	//release the entity manager
	m_pEntityMngr->ReleaseInstance();

	//release GUI
	ShutdownGUI();
}
