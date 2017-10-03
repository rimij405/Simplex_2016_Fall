#include "AppClass.h"
void Application::InitVariables(void)
{
	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	// Make SpaceInvader object.
	m_pSpaceInvader = new SpaceInvader(C_PURPLE, 46);
	
	// Craft the boxes in the SpaceInvader.
	m_pSpaceInvader->Init();

}

void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	// Update custom entities here.
	m_pSpaceInvader->Update();

}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Render meshes here:
	m_pSpaceInvader->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f)));

	// draw a skybox
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

	if (m_pSpaceInvader) {
		delete m_pSpaceInvader;
		m_pSpaceInvader = nullptr;
	}
	
	//release GUI
	ShutdownGUI();
}