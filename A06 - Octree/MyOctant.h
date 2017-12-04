/*
	MyOctant.h
	Definition for an Octree node.
	Ian Effendi (iae2784@g.rit.edu)
*/

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/

#ifndef __MY_OCTANT_H_
#define __MY_OCTANT_H_

#include "Definitions.h"
#include "MyEntityManager.h"

// Add to the Simplex namespace.
namespace Simplex {
	
	// An Octant is a node in an Octree.
	// Since we're tying this to spacial optimization, we should also be keeping track of the octant's bounds.
	class MyOctant {
		
#pragma region Static Members

		// Stores the number of octants that have been instantiated.
		static uint m_uOctantCount;

		// Stores the maximum level an octant is allowed to go.
		static uint m_uDepthThreshold;

		// Stores the ideal entity count.
		static uint m_uEntityThreshold;
	
#pragma endregion

	private:

#pragma region Fields

		uint m_uID = 0;	// This octant's current ID.
		uint m_uDepth = 0;	// The current subdivision level 
		uint m_uChildCount = 0;	// Number of children in the octant.
				
		vector3 m_v3HalfWidths = vector3(0.0f);	// Contains the half-widths for each of the three axes.
		vector3 m_v3Center = vector3(0.0f); // Stores the center point of the octant.
		vector3 m_v3Minimum = vector3(0.0f); // Stores the minimum vector of the octant.
		vector3 m_v3Maximum = vector3(0.0f); // Stores the maximum vector of the octant.
		
#pragma endregion

#pragma region Memory Managed

		MeshManager* m_pMeshManager = nullptr; // Mesh manager reference.
		MyEntityManager* m_pEntityManager = nullptr; // Entity manager reference.		
	
		MyOctant* m_pParent = nullptr; // Parent octant.		
		MyOctant* m_pChild[8]; // Will store the chilren of the current octant.		
		MyOctant* m_pRoot = nullptr; // Reference to the root octant.
		std::vector<MyOctant*> m_lChild; // Reference to list of nodes that will contain octants.

#pragma endregion

#pragma region Helper Methods

		/*
		USAGE: Deallocates the member fields in need of memory management.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);

		/*
		USAGE: Allocates member field data and initializes the octant.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);

		/*
		USAGE: Creates the list of all leaves that contain objects.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ConstructList(void);

#pragma endregion

	public:

#pragma region Constructor(s)

		/*
		USAGE: Constructor will create an octant containing all the Entities that the MeshManager currently contains.
		ARGUMENTS:
		- uint a_uSubdivisionLimit = 2 -> Sets the maximum level of subdivisions.
		- uint a_uIdealEntityCount = 5 -> Sets the ideal level of objects per octant.
		OUTPUT: class object.
		*/
		MyOctant(uint a_uSubdivisionLimit = 2, uint a_uIdealEntityCount = 5);

		/*
		USAGE: Constructs an octant at a center point using uniform sides.
		ARGUMENTS: 
		- vector3 a_v3Center -> Center of the octant in global space.
		- float a_fSize -> Size of each of the sides of the octant volume.
		OUTPUT: class object.
		*/
		MyOctant(vector3 a_v3Center, float a_fSize);

		/*
		USAGE: Constructs an octant at a center point using varying sides.
		ARGUMENTS:
		- vector3 a_v3Center -> Center of the octant in global space.
		- float a_v3Size -> Size of each of the sides of the octant volume.
		OUTPUT: class object.
		*/
		MyOctant(vector3 a_v3Center, vector3 a_v3Size);

#pragma endregion

#pragma region Rule of Three
				
		/*
		USAGE: Copy constructor.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: class object instance.
		*/
		MyOctant(MyOctant const& other);

		/*
		USAGE: Copy assignment operator.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: ---
		*/
		MyOctant& operator=(MyOctant const& other);

		/*
		USAGE: Destructor.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~MyOctant(void);

#pragma endregion


	};
} // namespace Simplex

#endif //__MY_OCTANT_H_
