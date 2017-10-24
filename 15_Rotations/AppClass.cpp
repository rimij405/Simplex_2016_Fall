#include "AppClass.h"
void Application::InitVariables(void)
{

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);
	m_qOrientation = IDENTITY_QUAT;
	m_v3Rotation = vector3(0.0f);
	m_v3Heading = vector3(0.0f);
	m_fRotationSpeed = 1;

	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCone(0.5f, 1.0f, 7, C_RED);
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
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	
#pragma region System Clock for Delta Time

	// Clock.
	static uint uClock = m_pSystem->GenClock();
	float delta = m_pSystem->GetDeltaTime(uClock) * m_fRotationSpeed;

#pragma endregion

#pragma region Rotation Matrix Setup

	static matrix4 rotMatrix = IDENTITY_M4;

#pragma endregion

#pragma region Euler Angles to Quaternions
	
	// This causes gimbal lock.

	/*
	// Create a quaternion for each of the axes.
	quaternion qX = glm::angleAxis(glm::radians(m_v3Rotation.x) * 100.0f, AXIS_X);
	quaternion qY = glm::angleAxis(glm::radians(m_v3Rotation.y) * 100.0f, AXIS_Y);
	quaternion qZ = glm::angleAxis(glm::radians(m_v3Rotation.z) * 100.0f, AXIS_Z);

	m_qOrientation = IDENTITY_QUAT;
	m_qOrientation = m_qOrientation * qZ;
	m_qOrientation = m_qOrientation * qY;
	m_qOrientation = m_qOrientation * qX;

	rotMatrix = ToMatrix4(m_qOrientation);
	*/

#pragma endregion
	
#pragma region Rotation matrix for each axis.
	
	// This also causes gimbal lock.

	/*
	// Create a quaternion for each of the axes.
	quaternion qX = glm::angleAxis(glm::radians(m_v3Rotation.x) * 100.0f, AXIS_X);
	quaternion qY = glm::angleAxis(glm::radians(m_v3Rotation.y) * 100.0f, AXIS_Y);
	quaternion qZ = glm::angleAxis(glm::radians(m_v3Rotation.z) * 100.0f, AXIS_Z);

	rotMatrix = ToMatrix4(m_qOrientation) * ToMatrix4(qX) * ToMatrix4(qZ) * ToMatrix4(qY);
	*/

#pragma endregion

#pragma region Implementation 2 - Operating directly on the Orientation 

	
	// Solves gimball lock and is animatable to occur at various speeds.

	// Operate on the quaternion using the heading vector.
	quaternion qRotation = quaternion(m_v3Heading * delta);
	m_qOrientation = m_qOrientation * qRotation;

	rotMatrix = ToMatrix4(m_qOrientation);
	

#pragma endregion
			
	matrix4 m4Model = glm::translate(vector3(1.0f, 0.0f, 0.0f)) * rotMatrix;
	
	// render the object
	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//draw the center of the world
	m_pMeshMngr->AddSphereToRenderList(glm::scale(vector3(0.1f)), C_RED, RENDER_WIRE);

	//print the time on the screen
#pragma region Debugging Information
	//Print info on the screen
	uint nEmptyLines = 21;
	for (uint i = 0; i < nEmptyLines; ++i)
		m_pMeshMngr->PrintLine("");//Add a line on top
								   //m_pMeshMngr->Print("						");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), C_YELLOW);

	//m_pMeshMngr->Print("						");
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->PrintLine(std::to_string(m_pSystem->GetFPS()), C_RED);
#pragma endregion
		
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
	//release the mesh
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
