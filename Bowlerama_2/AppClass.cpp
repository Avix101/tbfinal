#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "The Backpacks - DSA2 Final Project";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(	vector3(0.0f, 3.0f, 13.0f), //Position
											vector3(0.0f, 3.0f, 12.0f),	//Target
											AXIS_Y );					//Up

	//Set the position of the light
	m_pLightMngr->SetPosition(vector3(10.0f));

	bowlingBall = new Mesh();
	bowlingBall->GenerateSphere(1.0f, 4, C_BLACK);

	bowlingBallWire = new Mesh();
	bowlingBallWire->GenerateWireSphere(0.5f, C_GREEN);

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
		pinLocations[i] = vector3((-1.0f * row) + (2.0f * currentPin), 0.0f, -10.0f + (-1.0f * row));

		currentPin++;

		if (currentPin == row + 1)
		{
			row++;
			currentPin = 0;
		}
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
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

		//std::cout << "X: " << position.x << "  Y: " << position.y << "  Z: " << position.z << std::endl;
	}

	//Lessen the force over time due to friction
	force *= dampeningRate;

	//Render the bowling ball, ball wireframe and the plane
	bowlingBall->Render(projection, view, model);
	bowlingBallWire->Render(projection, view, model);
	plane->Render(projection, view, glm::translate(glm::rotate(IDENTITY_M4, -90.0f, AXIS_X), vector3(0.0f, 0.0f, -0.5f)));

	//Draw pins
	for (uint i = 0; i < 10; i++)
	{
		matrix4 pinModel = glm::translate(IDENTITY_M4, pinLocations[i]);
		pins[i].Render(projection, view, pinModel);
	}

	//Add grid to the scene
	//m_pMeshMngr->AddGridToRenderList();

	//Add skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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
	//Delete everything, and avoid memory leaks!
	SafeDelete(bowlingBall);
	SafeDelete(bowlingBallWire);
	SafeDelete(plane);
	delete[] pins;
	delete[] pinLocations;

	//release GUI
	ShutdownGUI();
}
