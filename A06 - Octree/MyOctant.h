/*
	MyOctant.h
	Definition for an Octree node.
	Ian Effendi (iae2784@g.rit.edu)
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
		
#pragma region //	Constants

		// Default maximum subdivision limit.
		const static uint DMAX_DEPTH = 2;

		// Default maximum capacity value.
		const static uint DMAX_CAPACITY = 5;

		// Default minimum size.
		const static uint DMIN_SIZE = 5;

		// Default max processes.
		const static uint DMAX_PROCESSES = 50;

#pragma endregion

#pragma region // 	Static Members

		// Stores the number of octants that have been instantiated.
		static uint sOctantCount;

		// Stores the maximum level an octant is allowed to go.
		static uint sDepthThreshold;

		// Stores the ideal entity count.
		static uint sEntityThreshold;

		// Minimum octant dimension size.
		static uint sMinimumSize;

		// Maximum queue processes.
		static uint sMaximumProcesses;

		/*
		Usage: Sets the minimum size of all octants.
		Arguments: uint a_uMinimumSize -> Input value to set.
		Output: ---
		*/
		static void SetMinimumSize(uint a_uMinimumSize);

		/*
		Usage: Sets the maximum subdivision level an octant can have.
		Arguments: uint a_uDepthThreshold -> Input value to set.
		Output: ---
		*/
		static void SetMaximumDepth(uint a_uDepthThreshold);

		/*
		Usage: Sets the ideal entity count.
		Arguments: uint m_uEntityThreshold -> Input value to set.
		Output: ---
		*/
		static void SetMaximumCapacity(uint a_uEntityThreshold);

		/*
		Usage: Sets the maximum entity queue process count.
		Arguments: uint a_uMaximumProcesses -> Input value to set.
		Output: ---
		*/
		static void SetMaximumProcesses(uint a_uMaximumProcesses);

#pragma endregion

	private:

#pragma region // 	Fields

		uint m_uID = 0;	// This octant's current ID.
		uint m_uDepth = 0;	// The current subdivision level 
		uint m_uChildCount = 0;	// Number of children in the octant.
				
		vector3 m_v3Active = C_YELLOW; // Active color (when the octant or its children contain entities).
		vector3 m_v3Inactive = C_WHITE; // Inactive color (when the octant has no children and is empty).

		vector3 m_v3HalfWidths = ZERO_V3;	// Contains the half-widths for each of the three axes.
		vector3 m_v3Center = ZERO_V3; // Stores the center point of the octant. (Global)
		vector3 m_v3Minimum = ZERO_V3; // Stores the minimum vector of the octant. (Global)
		vector3 m_v3Maximum = ZERO_V3; // Stores the maximum vector of the octant. (Global)
		
		// Queue of entities to add on the next update.
		std::deque<uint> m_lEntityQueue; 

		// We use a set to take advantage of it's automatic handling of 
		// List of entities contained within the octant.
		std::set<uint> m_lEntities; 

		// Reference to list of nodes that will contain objects.
		std::vector<MyOctant*> m_lActiveChildren; 

		bool m_bEnabled = true; // Octant enabled. (Read on the root octant).
		bool m_bVisibleOBB = false; // Visibility of the octant bounding box.

#pragma endregion

#pragma region // 	Memory Managed

		MeshManager* m_pMeshManager = nullptr; // Mesh manager reference.
		MyEntityManager* m_pEntityManager = nullptr; // Entity manager reference.		
	
		MyOctant* m_pParent = nullptr; // Parent octant.			
		MyOctant* m_pRoot = nullptr; // Reference to the root octant.
		MyOctant* m_pChild[8]; // Will store the children of the current octant.	

#pragma endregion

#pragma region // 	Helper Methods

		/*
		USAGE: Deallocates the member fields in need of memory management.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);

		/*
		USAGE: Initializes the octant.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);
		
		/*
		USAGE: Allocates member field data and initializes the octant.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void InitVariables(void);

		/*
		USAGE: Creates the list of all leaves that contain objects.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ConstructList(void);

#pragma endregion

	public:

#pragma region // 	Constructor(s)

		/*
		USAGE: Constructor will create an octant containing all the Entities that the EntityManager currently contains.
		ARGUMENTS:
		- uint a_uSubdivisionLimit = 2 -> Sets the maximum level of subdivisions.
		- uint a_uIdealEntityCount = 5 -> Sets the ideal level of objects per octant.
		- uint a_uMinimumSize = 5 -> Sets the minimum size threshold an octant must have if it is to be subdivided.
		OUTPUT: class object.
		*/
		MyOctant(uint a_uSubdivisionLimit = DMAX_DEPTH, uint a_uIdealEntityCount = DMAX_CAPACITY, uint a_uMinimumSize = DMIN_SIZE);

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
		- float a_v3HalfWidths -> Half size of each of the sides of the octant volume.
		OUTPUT: class object.
		*/
		MyOctant(vector3 a_v3Center, vector3 a_v3HalfWidths);

#pragma endregion

#pragma region // 	Rule of Three
				
		/*
		USAGE: Performs a deep copy of an existing octant. Keeps the ID.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: class object instance.
		*/
		MyOctant(MyOctant const& other);

		/*
		USAGE: Performs a deep copy of an existing octant. Keeps the ID.
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

#pragma region // 	Rule of Five

		/*
		USAGE: Move constructor.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: class object instance.
		*/
		MyOctant(MyOctant&& other);

		/*
		USAGE: Move assignment operator.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: ---
		*/
		MyOctant& operator=(MyOctant&& other);
		
		/*
		USAGE: Changes the object's contents for the input object.
		ARGUMENTS: MyOctant& other -> The other object to swap contents with.
		OUTPUT: ---
		*/
		void Swap(MyOctant& other);

#pragma endregion

#pragma region // 	Flag Methods
		
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
		bool IsRoot(void) const;

		/*
		USAGE: Asks if this octant has no children.
		ARGUMENTS: ---
		OUTPUT: Returns check.
		*/
		bool IsLeaf(void) const;
		
		/*
		USAGE: Asks if this octant has no entities.
		ARGUMENTS: ---
		OUTPUT: Returns check.
		*/
		bool IsEmpty(void) const;

		/*
		USAGE: Checks to see if this octant has more than the input number of entities available.
		ARGUMENTS: uint a_uEntities -> Number of entities to check for.
		OUTPUT: Returns check.
		*/
		bool ContainsMoreThan(uint a_uEntities);

		/*
		Usage: Gets visibility of oriented bounding box.
		Arguments: ---
		Output: Returns check.
		*/
		bool IsOBBVisible(void) const;

		/*
		Usage: Gets enabled flag of octant.
		Arguments: ---
		Output: Returns check.
		*/
		bool IsEnabled(void) const;

#pragma endregion

#pragma region // 	Mutator Methods

		/*
		Usage: Sets enabled flag of octant.
		Arguments: bool a_bVisibleOBB -> Input value to set OBB visibility flag to.
		Output: ---
		*/
		void SetOBBVisbile(bool a_bVisibleOBB);

		/*
		Usage: Sets enabled flag of octant.
		Arguments: bool a_bEnabled -> Input value to set enable flag to.
		Output: ---
		*/
		void SetEnabled(bool a_bEnabled);
		
		/*
		USAGE: Sets this octant's size. (via half-width property).
		ARGUMENTS: a_fSize -> Length of sides of the octant.
		OUTPUT: ---
		*/
		void SetSize(float a_fSize);

		/*
		USAGE: Sets this octant's half widths.
		ARGUMENTS: a_v3HalfWidths -> Value to set half widths to.
		OUTPUT: ---
		*/
		void SetHalfWidths(vector3 a_v3HalfWidths);

		/*
		USAGE: Set the center of the octant in global space.
		ARGUMENTS: a_v3Center -> Value to set center to.
		OUTPUT: ---
		*/
		void GetCenterGlobal(vector3 a_v3Center);

		/*
		USAGE: Set the maximum vector in global space.
		ARGUMENTS: a_v3Maximum -> Value to set maximum vector to.
		OUTPUT: ---
		*/
		void SetMaxGlobal(vector3 a_v3Maximum);

		/*
		USAGE: Set the minimum vector in global space.
		ARGUMENTS: a_v3Minimum -> Value to set minimum vector to.
		OUTPUT: ---
		*/
		void SetMinGlobal(vector3 a_v3Minimum);

		/*
		USAGE: Sets the octant reference at the specified index to the input value (if possible).
		ARGUMENTS:
		- uint a_uChild -> index of the child [0, 7].
		- MyOctant* const a_pChild -> pointer child to add.
		OUTPUT: ---
		*/
		void SetChild(uint a_uChild, MyOctant* const a_pChild);

		/*
		USAGE: Set the active color of the octant.
		ARGUMENTS: vector3 a_v3Color -> Input color value to assign.
		OUTPUT: ---
		*/
		void SetActiveColor(vector3 a_v3Color);

		/*
		USAGE: Set the inactive color of the octant.
		ARGUMENTS: vector3 a_v3Color -> Input color value to assign.
		OUTPUT: ---
		*/
		void SetInactiveColor(vector3 a_v3Color);

#pragma endregion

#pragma region // 	Accessor Methods

		/*
		USAGE: Gets this octant's size.
		ARGUMENTS: ---
		OUTPUT: Returns the size of the octant as a float.
		*/
		float GetSize(void) const;
		
		/*
		USAGE: Gets this octant's half widths.
		ARGUMENTS: ---
		OUTPUT: Returns the half widths of the octant as a vector3.
		*/
		vector3 GetHalfWidths(void) const;

		/*
		USAGE: Get the center of the octant in global space.
		ARGUMENTS: ---
		OUTPUT: Returns a vector3.
		*/
		vector3 GetCenterGlobal(void) const;

		/*
		USAGE: Get the maximum vector in global space.
		ARGUMENTS: ---
		OUTPUT: Returns a vector3.
		*/
		vector3 GetMaxGlobal(void) const;

		/*
		USAGE: Get the minimum vector in global space.
		ARGUMENTS: ---
		OUTPUT: Returns a vector3.
		*/
		vector3 GetMinGlobal(void) const;
		
		/*
		USAGE: Returns the child at the specified index (if possible).
		ARGUMENTS: uint a_uChild -> index of the child [0, 7].
		OUTPUT: MyOctant object (child at index).
		*/
		MyOctant* GetChild(uint a_uChild) const;

		/*
		USAGE: Returns the parent of this octant.
		ARGUMENTS: ---
		OUTPUT: MyOctant object (parent of this octant).
		*/
		MyOctant* GetParent(void) const;
		
		/*
		USAGE: Return the total number of octants in the world.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		uint GetOctantCount(void) const;

		/*
		USAGE: Return the number of entities within the octant.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		uint GetEntityCount(void) const;

		/*
		USAGE: Get the active color of the octant.
		ARGUMENTS: ---
		OUTPUT: Returns the color.
		*/
		vector3 GetActiveColor(void) const;

		/*
		USAGE: Get the inactive color of the octant.
		ARGUMENTS: ---
		OUTPUT: Returns the color.
		*/
		vector3 GetInactiveColor(void) const;

#pragma endregion

#pragma region // 	Service Methods

		/*
		USAGE: Adds a collection of entities to the update queue.
		ARGUMENTS: uint a_lIndices -> Indices of the entities to be added.
		OUTPUT: ---
		*/
		void AddEntities(std::vector<uint> a_lIndices);

		/*
		USAGE: Adds a single entity to the update queue.
		ARGUMENTS: uint a_uIndex -> Index of the entity to be added.
		OUTPUT: ---
		*/
		void AddEntity(uint a_uIndex);

		/*
		USAGE: Adds (or updates) all entities present in the queue.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AddQueue(void);

		/*
		USAGE: Adds entities from queue, updates min and max vectors, updates center, updates halfwidths, and clears the queue.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void UpdateTree(void);
		
		/*
		USAGE: Display the MyOctant object, specified by index, including the objects underneath. (Used to display children).
		ARGUMENTS:
		- uint a_uIndex -> MyOctant to be displayed.
		- vector3 a_v3color = C_YELLOW -> Color to be displayed in.
		OUTPUT: ---
		*/
		void Display(uint a_uIndex, vector3 a_v3Color = C_YELLOW);
		
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
		USAGE: Deletes each child octant in the array and sets array back to an empty one.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearChildren(void);

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