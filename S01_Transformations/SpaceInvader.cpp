// Include headers.
#include "SpaceInvader.h"

#include <iostream>
#include <fstream>

sf::Clock SpaceInvader::k_Clock;

///////////////////////////
// Constructors for SpaceInvader.
//-------------------------	
// Empty constructor for SpaceInvader.
SpaceInvader::SpaceInvader(void) {
	m_binit = false; // Not intialized on construction.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = Simplex::vector3(1.0f);
	m_MeshCubes = nullptr;
	m_m4models = nullptr;
	m_uicapacity = 5;
	m_uicount = 0;
}

// SpaceInvader constructor that takes a color.
SpaceInvader::SpaceInvader(Simplex::vector3 p_v3color) {
	m_binit = false; // Not intialized on construction.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = vector3(p_v3color);
	m_MeshCubes = nullptr;
	m_m4models = nullptr;
	m_uicapacity = 5;
	m_uicount = 0;
}

// SpaceInvader constructor that takes a color and a capacity.
SpaceInvader::SpaceInvader(Simplex::vector3 p_v3color, unsigned int p_uicapacity) {
	m_binit = false; // Not intialized on construction.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = vector3(p_v3color);
	m_MeshCubes = nullptr;
	m_m4models = nullptr;
	m_uicapacity = p_uicapacity;
	m_uicount = 0;
}

// Destructor for SpaceInvader.
SpaceInvader::~SpaceInvader(void) {
	Release(); // Call the release method.
}

///////////////////////////
// Service methods.
//-------------------------
// Check initialization flag.
bool SpaceInvader::IsInitialized(void) {
	return (m_binit == true);
}

// By calling update, 
void SpaceInvader::Update(void) {
	
	// Set current.
	static sf::Time dt = k_Clock.restart();
	float delta = dt.asSeconds();

	// Movement functionality will take place here.
	vector3 f_v3uniform(1.0f, 1.0f, 1.0f); // Base vector.
	vector3 f_v3speed(0.01f, 0.0f, 0.0f); // Speed in a particular direction, for the frame.
	vector3 f_v3translation(
		delta * f_v3uniform.x * f_v3speed.x,
		delta * f_v3uniform.y * f_v3speed.y,
		delta * f_v3uniform.z * f_v3speed.z
	);

	// Edit the translation matrix for the space invaders.
	m_m4local = glm::translate(m_m4local, f_v3translation);

	std::cout << "Debug: [" << delta << "]: <" << glm::to_string(m_m4local) << ">.\n";

}

// Add the SpaceInvader's meshes to the render list.
void SpaceInvader::Render(matrix4 p_m4proj, matrix4 p_m4view, matrix4 p_m4model) {

	// Calculate translation matrices.
	matrix4 f_m4temp = p_m4model * m_m4local;

	// Render each of the cubes.
	for (unsigned int i = 0; i < m_uicount; i++) {
		m_MeshCubes[i].Render(p_m4proj, p_m4view, glm::translate(m_m4models[i] * f_m4temp, vector3(0.0f, 0.0f, 0.0f)));		
	}

}

// Safely release data.
void SpaceInvader::Release(void) {
	
	// Delete all the cubes inside Mesh Cubes.
	if (m_MeshCubes != nullptr) {
		delete[] m_MeshCubes;
		m_MeshCubes = nullptr;
	}

	// Delete all the data inside Models.
	if (m_m4models != nullptr) {
		delete m_m4models;
		m_m4models = nullptr;
	}

	// Reset data member values.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = Simplex::vector3(1.0f);
	m_binit = false;
	m_uicount = 0;

}

///////////////////////////
// Helper methods.
//-------------------------
// Initialize the data structures of SpaceInvader.
void SpaceInvader::Init(void) {
	
	// Check if already initialized.
	if (!IsInitialized()) {
	
		// Initialize the data members.
		m_m4local = glm::translate(Simplex::matrix4(1.0f), vector3(-5.0f, 0.0f, 0.0f));

		// Initialze the meshes.
		InitMeshes();

		// Set initialization flag to true.
		m_binit = true;
	}

}

// Initialize the meshes.
void SpaceInvader::InitMeshes(void) {

	// Check if already initialized.
	if (!IsInitialized()) {
		
		// Create the mesh data structures.
		m_MeshCubes = new MyMesh[m_uicapacity]; // Create the array with 46 cubes.
		m_m4models = new matrix4[m_uicapacity]; // Create the array with 46 model matrices.

		// Now the arrays are initialized.
		m_binit = true;

		// Add the meshes in the positions they need to be for the space invader shape.
		
		// ROW 8
		AddMesh(vector3(3.0f, 8.0f, 0.0f), 1.0f);
		AddMesh(vector3(9.0f, 8.0f, 0.0f), 1.0f);

		// ROW 7
		AddMesh(vector3(4.0f, 7.0f, 0.0f), 1.0f);
		AddMesh(vector3(8.0f, 7.0f, 0.0f), 1.0f);

		// ROW 6
		AddMesh(vector3(3.0f, 6.0f, 0.0f), 1.0f);
		AddMesh(vector3(4.0f, 6.0f, 0.0f), 1.0f);
		AddMesh(vector3(5.0f, 6.0f, 0.0f), 1.0f);
		AddMesh(vector3(6.0f, 6.0f, 0.0f), 1.0f);
		AddMesh(vector3(7.0f, 6.0f, 0.0f), 1.0f);
		AddMesh(vector3(8.0f, 6.0f, 0.0f), 1.0f);
		AddMesh(vector3(9.0f, 6.0f, 0.0f), 1.0f);

		// ROW 5
		AddMesh(vector3(2.0f, 5.0f, 0.0f), 1.0f);
		AddMesh(vector3(3.0f, 5.0f, 0.0f), 1.0f);
		AddMesh(vector3(5.0f, 5.0f, 0.0f), 1.0f);
		AddMesh(vector3(6.0f, 5.0f, 0.0f), 1.0f);
		AddMesh(vector3(7.0f, 5.0f, 0.0f), 1.0f);
		AddMesh(vector3(9.0f, 5.0f, 0.0f), 1.0f);
		AddMesh(vector3(10.0f, 5.0f, 0.0f), 1.0f);

		// ROW 4
		AddMesh(vector3(1.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(2.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(3.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(4.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(5.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(6.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(7.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(8.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(9.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(10.0f, 4.0f, 0.0f), 1.0f);
		AddMesh(vector3(11.0f, 4.0f, 0.0f), 1.0f);

		// ROW 3
		AddMesh(vector3(1.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(3.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(4.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(5.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(6.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(7.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(8.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(9.0f, 3.0f, 0.0f), 1.0f);
		AddMesh(vector3(11.0f, 3.0f, 0.0f), 1.0f);

		// ROW 2
		AddMesh(vector3(1.0f, 2.0f, 0.0f), 1.0f);
		AddMesh(vector3(3.0f, 2.0f, 0.0f), 1.0f);
		AddMesh(vector3(9.0f, 2.0f, 0.0f), 1.0f);
		AddMesh(vector3(11.0f, 2.0f, 0.0f), 1.0f);

		// ROW 1
		AddMesh(vector3(4.0f, 1.0f, 0.0f), 1.0f);
		AddMesh(vector3(5.0f, 1.0f, 0.0f), 1.0f);
		AddMesh(vector3(7.0f, 1.0f, 0.0f), 1.0f);
		AddMesh(vector3(8.0f, 1.0f, 0.0f), 1.0f);

	}
}

// Adds mesh to the data structure with specified scale and position values.
void SpaceInvader::AddMesh(Simplex::vector3 p_v3position, float p_fscale) {

	if (IsInitialized()) {

		if (m_uicount < m_uicapacity) {

			// Set the initialized position for the mesh.
			matrix4 f_m4temp = glm::translate(p_v3position);

			// Create a reference to a mesh object on the heap.
			MyMesh* f_MeshTemp = new MyMesh();
			f_MeshTemp->GenerateCube(p_fscale, m_v3color);

			// Generate the shape for the added mesh, using specified color.
			m_MeshCubes[m_uicount] = *f_MeshTemp;

			// Place the mesh onto the array.
			m_m4models[m_uicount] = f_m4temp;

			// Increment the count.
			m_uicount = m_uicount + 1;

			// Debug message.
			// std::cout << "[" << m_uicount << "/" << m_uicapacity << "] " << glm::to_string(p_v3position) << '\n';

			// Remove the temp reference on the pointer.
			f_MeshTemp = nullptr; // Do NOT delete the data referenced by the pointer.

		}

	}
}

// Check if a file exists.
bool SpaceInvader::IsFile(const std::string& p_rsfilename) {
	std::ifstream inputFile(p_rsfilename);
	return inputFile.good();
}