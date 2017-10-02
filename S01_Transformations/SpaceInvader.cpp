// Include headers.
#include "SpaceInvader.h"

///////////////////////////
// Data members of class.
//-------------------------
// Cubes in the space invader shape.
// MyMesh* m_MeshCubes = nullptr; // Don't set = new MyMesh[46]! Should be done in the init method.

// Color the boxes in the mesh should be.
// vector3 m_v3color; // Color vector.

// Local matrix, modified to displace entire shape.
// matrix4 m_m4local; // Center of the world matrix. (Gets modified to move all cubes).'

// Initialization flag.
// bool m_binit;

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
	m_icount = 0;
}

// SpaceInvader constructor that takes a color.
SpaceInvader::SpaceInvader(Simplex::vector3 p_v3color) {
	m_binit = false; // Not intialized on construction.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = p_v3color;
	m_MeshCubes = nullptr;
	m_m4models = nullptr;
	m_icount = 0;
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
	// Movement functionality will take place here.
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
	m_icount = 0;

}

// GoTo sets the position of the local matrix.
void SpaceInvader::GoTo(Simplex::vector3 p_v3position) {
	 
}

///////////////////////////
// Helper methods.
//-------------------------
// Initialize the data structures of SpaceInvader.
void SpaceInvader::Init(void) {
	
	// Check if already initialized.
	if (!IsInitialized()) {
	
		// Initialize the data members.
		m_m4local = Simplex::matrix4(1.0f);
		m_v3color = Simplex::vector3(1.0f);

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

		// Create the meshes.
		m_MeshCubes = new MyMesh[46]; // Create the array with 46 cubes.
		m_m4models = new matrix4[46]; // Create the array with 46 model matrices.
		m_icount = 0; // Set count of meshes back to zero.
	}
}

// Add a mesh to the data structure.
void SpaceInvader::AddMesh(MyMesh* &p_Mesh) {
	
	// Calls the add mesh with a vector3 at <0, 0, 0> and a scale of 1.0f.
	AddMesh(p_Mesh, vector3(0.0f), 1.0f);

}

// Add a mesh to the data structure while specifying a position.
void SpaceInvader::AddMesh(MyMesh* &p_Mesh, Simplex::vector3 p_v3position) {

	// Calls the add mesh with the parameter vector and a scale of 1.0f.
	AddMesh(p_Mesh, p_v3position, 1.0f);

}

// Adds mesh to the data structure with specified scale and position values.
void SpaceInvader::AddMesh(MyMesh* &p_Mesh, Simplex::vector3 p_v3position, float p_fscale) {

	// Create a reference to a mesh object on the heap.
	MyMesh* f_MeshTemp = new MyMesh();

	// Set the initialized position for the mesh.
	matrix4 f_m4temp = glm::translate(p_v3position);

	// Generate the shape for the added mesh, using specified color.
	f_MeshTemp->GenerateCube(p_fscale, m_v3color);

	// Place the mesh onto the array.
	m_MeshCubes[m_icount] = *f_MeshTemp;
	m_m4models[m_icount] = f_m4temp;

	// Increment the count.
	m_icount = m_icount + 1;

	// Remove the temp reference on the pointer.
	f_MeshTemp = nullptr; // Do NOT delete the data referenced by the pointer.

}