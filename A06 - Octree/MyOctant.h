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
		vector3 m_v3Center = vector3(0.0f); // Stores the center point of the octant. (Global)
		vector3 m_v3Minimum = vector3(0.0f); // Stores the minimum vector of the octant. (Global)
		vector3 m_v3Maximum = vector3(0.0f); // Stores the maximum vector of the octant. (Global)
		
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

#pragma region Flag Methods
		
		/*
		USAGE: Asking if there is a collision with an Entity as supplied by the input index value.
		ARGUMENTS: uint a_uRBIndex -> Index of the Entity in the EntityManager.
		OUTPUT: Check of the collision.
		*/
		bool IsColliding(uint a_uRBIndex);
		
		/*
		USAGE: Asks if this octant has no parents.
		ARGUMENTS: ---
		OUTPUT: Returns check.
		*/
		bool IsRoot(void);

		/*
		USAGE: Asks if this octant has no children.
		ARGUMENTS: ---
		OUTPUT: Returns check.
		*/
		bool IsLeaf(void);
		
		/*
		USAGE: Checks to see if this octant has more than the input number of entities available.
		ARGUMENTS: uint a_uEntities -> Number of entities to check for.
		OUTPUT: Returns check.
		*/
		bool ContainsMoreThan(uint a_uEntities);

#pragma endregion

#pragma region Accessor Methods

		/*
		USAGE: Gets this octant's size.
		ARGUMENTS: ---
		OUTPUT: Returns the size of the octant as a float.
		*/
		float GetSize(void);

		/*
		USAGE: Gets this octant's half widths.
		ARGUMENTS: ---
		OUTPUT: Returns the half widths of the octant as a vector3.
		*/
		vector3 GetHalfWidths(void);

		/*
		USAGE: Get the center of the octant in global space.
		ARGUMENTS: ---
		OUTPUT: Returns a vector3.
		*/
		vector3 GetCenterGlobal(void);

		/*
		USAGE: Get the maximum vector in global space.
		ARGUMENTS: ---
		OUTPUT: Returns a vector3.
		*/
		vector3 GetMaxGlobal(void);

		/*
		USAGE: Get the minimum vector in global space.
		ARGUMENTS: ---
		OUTPUT: Returns a vector3.
		*/
		vector3 GetMinGlobal(void);
		
		/*
		USAGE: Returns the child at the specified index (if possible).
		ARGUMENTS: uint a_uChild -> index of the child [0, 7].
		OUTPUT: MyOctant object (child at index).
		*/
		MyOctant* GetChild(uint a_uChild);

		/*
		USAGE: Returns the parent of this octant.
		ARGUMENTS: ---
		OUTPUT: MyOctant object (parent of this octant).
		*/
		MyOctant* GetParent(void);
		
		/*
		USAGE: Return the total number of octants in the world.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		uint GetOctantCount(void);

#pragma endregion

#pragma region Service Methods

		/*
		USAGE: Changes the object's contents for the input object.
		ARGUMENTS: MyOctant& other -> The other object to swap contents with.
		OUTPUT: ---
		*/
		void Swap(MyOctant& other);
		
		/*
		USAGE: Display the MyOctant object, specified by index, including the objects underneath. (Used to display children).
		ARGUMENTS:
		- uint a_uIndex -> MyOctant to be displayed.
		- vector3 a_v3color = C_YELLOW -> Color to be displayed in.
		OUTPUT: ---
		*/
		void Display(uint a_uIndex, vector3 a_v3color = C_YELLOW);
		
		/*
		USAGE: Displays the MyOctant volume in the color specified.
		ARGUMENTS: vector3 a_v3Color = C_YELLOW -> Color to be displayed in.
		OUTPUT: ---
		*/
		void Display(vector3 a_v3Color = C_YELLOW);
		
		/*
		USAGE: Displays the non-empty leaf nodes in the octree.
		ARGUMENTS:
		- vector3 a_v3Color = C_YELLOW -> Color to display leaf octant in.
		OUTPUT: ---
		*/
		void DisplayLeafs(vector3 a_v3Color = C_YELLOW);
		
		/*
		USAGE: Clears the entity list for each node.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearEntityList(void);

		/*
		USAGE: Allocates 8 smaller octants in the child pointers.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Subdivide(void);
		
		/*
		USAGE: Recursively deletes all children (and children of children).
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void KillBranches(void);
		
		/*
		USAGE: Creates a tree using subdivisions, the max number of objects and levels.
		ARGUMENTS: uint a_uSubdivisionThreshold = 3 -> Sets the maximum level of the tree it is constructing.
		OUTPUT: ---
		*/
		void ConstructTree(uint a_uSubdivisionThreshold = 3);
		
		/*
		USAGE: Traverse the tree to the leafs and sets the objects in them to the index.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AssignID(void);

#pragma endregion


	};
} // namespace Simplex

#endif //__MY_OCTANT_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */