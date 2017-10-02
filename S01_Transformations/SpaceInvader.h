/*
	Ian Effendi
	SpaceInvader.h
	9.28.2017
	Contains definitions for the SpaceInvader class.
*/

#ifndef __SPACEINVADER_H_
#define __SPACEINVADER_H_

#include <vector>
#include "MyMesh.h"

class SpaceInvader {
private:	

	///////////////////////////
	// Private members for class.
	//-------------------------

	// Cubes in the space invader shape.
	MyMesh* m_MeshCubes = nullptr; // Set to nullptr. // 46 cubes in the space invader shape.	

	// Matrix models for each of the cubes.
	matrix4* m_m4models = nullptr; // Set to nullptr.

	// Color the boxes in the mesh should be.
	vector3 m_v3color = Simplex::vector3(1.0f); // Color vector.
	
	// Local matrix, modified to displace entire shape.
	matrix4 m_m4local = Simplex::matrix4(1.0f); // Center of the world matrix. (Gets modified to move all cubes).

	// Initialization flag.
	bool m_binit = false;

	// Number of meshes.
	unsigned int m_icount; // Number of meshes added to the space invader.

	///////////////////////////
	// Helper methods.
	//-------------------------
	// Initialize the data structures of SpaceInvader.
	void Init(void);

	// Initialize the meshes.
	void InitMeshes(void);

	// Add a mesh to the array.
	void AddMesh(MyMesh* &p_Mesh);

	// Add a mesh to the array while specifying a position.
	void AddMesh(MyMesh* &p_Mesh, Simplex::vector3 p_v3position);

	// Add a mesh to the array while specifying a position and a scale.
	void AddMesh(MyMesh* &p_Mesh, Simplex::vector3 p_v3position, float p_fscale);

public:
	
	///////////////////////////
	// Constructors for SpaceInvader.
	//-------------------------	
	// Empty constructor for SpaceInvader.
	SpaceInvader(void); // Simplex::vector3(1.0f, 1.0f, 1.0f)

	// This constructor takes a color.
	SpaceInvader(Simplex::vector3 p_v3color); // SpaceInvader constructor.

	// Destructor for SpaceInvader.
	~SpaceInvader(void);

	///////////////////////////
	// Rule of Three.
	//-------------------------	
	// Copy constructor.
	// SpaceInvader(SpaceInvader const& input);
	
	// Copy assignment operator.
	// SpaceInvader& operator=(SpaceInvader const& input);

	
	///////////////////////////
	// Service methods.
	//-------------------------
	// Check initialization flag.
	bool IsInitialized(void);

	// Update the SpaceInvader's properties.
	void Update(void);

	// Add the SpaceInvader's meshes to the render list.
	void Render(matrix4 p_m4proj, matrix4 p_m4view, matrix4 p_m4model);

	// Safely release data.
	void Release(void);

	// GoTo sets the position of the local matrix.
	void GoTo(Simplex::vector3 p_v3position);

};


#endif //__SPACEINVADER_H_


