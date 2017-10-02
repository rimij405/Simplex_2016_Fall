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
}

// This constructor takes a color.
SpaceInvader::SpaceInvader(Simplex::vector3 p_v3color) {
	m_binit = false; // Not intialized on construction.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = p_v3color;
	m_MeshCubes = nullptr;
} // SpaceInvader constructor.

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

	// Reset data member values.
	m_m4local = Simplex::matrix4(1.0f);
	m_v3color = Simplex::vector3(1.0f);
	m_binit = false;

}

// GoTo sets the position of the local matrix.
void SpaceInvader::GoTo(Simplex::vector3 p_v3position) {

}

///////////////////////////
// Helper methods.
//-------------------------
// Initialize the data structures of SpaceInvader.
void SpaceInvader::Init(void) {

}

// Initialize the meshes.
void SpaceInvader::InitMeshes(void) {

}

// Add a mesh to the array.
void SpaceInvader::AddMesh(MyMesh* &p_Mesh) {

}

// Add a mesh to the array while specifying a position.
void SpaceInvader::AddMesh(MyMesh* &p_Mesh, Simplex::vector3 p_v3position) {

}

// Add a mesh to the array while specifying a position and a scale.
void SpaceInvader::AddMesh(MyMesh* &p_Mesh, Simplex::vector3 p_v3position, float p_fscale) {

}