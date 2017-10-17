#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Ian Effendi - iae2784@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3) {
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));
	}

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER) {
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if (m_uOrbits < 1) {
		m_uOrbits = 7;
	}

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red

	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)

	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/

	// // // // // // // // // // // // // // // // // // // // // // //
	// Code dealing with creation of the routes, dynamically at runtime.
	// // // // // // // // // // // // // // // // // // // // // // //

	// Calculate the current targets for each of the orbits.
	m_orbits = std::vector<std::vector<vector3>>();
	m_cycleIDs = std::vector<uint>();
	
	// Add current indices.
	for (uint j = 0; j < m_uOrbits; j++) {
		m_cycleIDs.push_back(0); // every cycle should start at zero! push a zero index for every orbit.
	}
		
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		
		// Make new target vector to hold vertices.
		std::vector<vector3> targets = std::vector<vector3>();

		// Generate the points on a shape.
		for (uint vertex = 0; vertex < i; vertex++) {
						
			// Calculate the current angle.
			float theta = 2.0f * PI * ((float)vertex / (float)i);

			// Use polar coordinates to get x and y.
			float x = fSize * cosf(theta);
			float y = fSize * sinf(theta);

			// Push new vertex back onto the collection.
			targets.push_back(vector3(x, y, (float)(i * 0.5f)));
		}

		// Push this targets vector to the bigger collection.
		m_orbits.push_back(targets);

		// Original code building the custom tori.
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
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
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system

	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/

	m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	// Render spheres at each of the vertices.
	for (uint ai = 0; ai < m_uOrbits; ai++) {
		uint size = m_orbits[ai].size();
		for (uint aj = 0; aj < size; aj++) {
			
			vector3 pos = m_orbits[ai][aj];
			matrix4 m4 = glm::translate(m4Offset, pos);
			m_pMeshMngr->AddSphereToRenderList(m4 * glm::scale(vector3(0.05f)), C_GREEN);

		}
	}


	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
	// We want to increment our time on each call to the method; not inside a loop within this function.
	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

	// Get a timer.
	static float fTimer = 0; // Store the new timer.
	static uint uClock = m_pSystem->GenClock(); // Generate a new clock for that timer.
	fTimer += m_pSystem->GetDeltaTime(uClock); // Get the delta time for that timer.
	
	// Percentage.
	float fAnimationLength = 1.0f; // seconds.
	float fPercent = MapValue(fTimer, 0.0f, fAnimationLength, 0.0f, 1.0f);
	
	// On completion of a segment. (we say 0.999f, to round up those final 0.9999f values).
	if (fPercent > 0.999f) {

		// When percentage is 1.0f
		// Reset the timer and
		// Move the references over by 1.

		// Resets the timer for the next segment. //
		fTimer = 0;
		fPercent = 0;
		uClock = m_pSystem->GenClock();

		// Increment the targets for each of the orbits. //
		for(uint j = 0; j < m_uOrbits; j++) {
			
			// j represents the orbit we're talking about. (1 to 8, but zero based, so 0 to 7).
			// CycleIDs represents the current stop index for each of the orbits.

			// Get the current index value.
			uint currentIndexValue = m_cycleIDs[j];

			// Increment the value by one.
			currentIndexValue++;

			// If the index is greater than the number of sides in the orbit...
			if (currentIndexValue == m_orbits[j].size()) {
				currentIndexValue = 0; // ...set back to zero.
			}

			// Assign the calculated value back to its position in m_cycleIDs.
			m_cycleIDs[j] = currentIndexValue;
			
			// Example.		4 sides		3 sides
			//				Cycle 0		Cycle 0
			//				Cycle 1		Cycle 1
			//				Cycle 2		Cycle 2
			//				Cycle 3		Cycle 0
			//				Cycle 0		Cycle 1
			//				Cycle 1		Cycle 2
			//				Cycle 2		Cycle 0
			//				Cycle 3		Cycle 1

		}
	}

	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //	
	// Loop through all of the orbits (eg. cycles) and draw the shapes (as well as tracking spheres).
	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
	
	for (uint i = 0; i < m_uOrbits; i++) {
		
#pragma region 
						
		// std::cout << "\n\n\nDrawing shapes for orbit #" << std::to_string(i + 1) << ".";
		
		// Get the targets for this current cycle.
		std::vector<vector3> targets = m_orbits[i];
		
		// Get the number of sides from the current orbit tracked by this loop.
		// uint orbitSides = m_orbits[i].size(); // This is equivalent with the statement below.
		int orbitSides = targets.size();

		// Don't run the code if orbitSides is less than 3.
		if (orbitSides > 2) {

#pragma region

			// Draw the tori needed for one orbit (designated by i).
			float depth = 0.5f * (float)(i + 3);
			m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::translate(vector3(0.0f, 0.0f, depth)) * glm::rotate(m4Offset, 90.0f, AXIS_X));

#pragma endregion // Draw the orbit tori.

			// std::cout << "\nNumber of vertices for this orbit: " << std::to_string(m_orbits[i].size()) << " verts.";

			// // // // //
			// Get the current start and target indices for the current orbit.

			// Get current start position index.
			uint currentStartIndex = m_cycleIDs[i];

			// Clamp the index value if it's too big.
			if (currentStartIndex == orbitSides) {
				currentStartIndex = 0;
			}

			// Get current target position index.
			uint currentTargetIndex = (currentStartIndex + 1);

			// Clamp the target value if it's too big.
			if (currentTargetIndex == orbitSides) {
				currentTargetIndex = 0;
			}

			// // // // //
			// Get the current start and target vector positions for the current orbit.
			vector3 currentStart = targets[currentStartIndex]; // Get the current start position.
			vector3 currentTarget = targets[currentTargetIndex]; // Get the current target position.

			// Get the current position based on linear interpolation between the start and target points.
			vector3 currentPosition = glm::lerp(currentStart, currentTarget, fPercent);

			// Translate the model based off of the calculated linear interpolation position.
			matrix4 m4Model = glm::translate(m4Offset, currentPosition);

#pragma region

			// Draw the spheres.
			m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1f)), C_WHITE);

#pragma endregion // Draw the orbit tracking spheres.

		}		

#pragma endregion // Orbit LERP tracking.

	}

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
	//release GUI
	ShutdownGUI();
}