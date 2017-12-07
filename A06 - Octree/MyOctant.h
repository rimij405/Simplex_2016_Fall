/*
	MyOctant.h
	Definition for an Octree node.
	Ian Effendi (iae2784@g.rit.edu)
*/

#ifndef __MY_OCTANT_H_
#define __MY_OCTANT_H_

#include "Definitions.h"

// Add to the Simplex namespace.
namespace Simplex 
{	

#pragma region //	Forward Declarations

	// Forward declaration of our MyEntityManager class.
	class MyEntity;

	// Forward declaration of our MyEntityManager class.
	class MyEntityManager;

	// Forward declaration of our Octant class. (Remember to #include "MyOctree.h" in .cpp file).
	class MyOctree;

#pragma endregion

	// An Octant is a node in an Octree.
	class MyOctant {
		
#pragma region // 	Static Members
		
		/*
		Usage: Sets the minimum size of all octants.
		Arguments: uint a_uMinimumSize -> Input value to set.
		Output: ---
		*/
		// static void SetMinimumSize(uint a_uMinimumSize);

		/*
		Usage: Sets the maximum subdivision level an octant can have.
		Arguments: uint a_uDepthThreshold -> Input value to set.
		Output: ---
		*/
		// static void SetMaximumDepth(uint a_uDepthThreshold);

		/*
		Usage: Sets the ideal entity count.
		Arguments: uint m_uEntityThreshold -> Input value to set.
		Output: ---
		*/
		// static void SetMaximumCapacity(uint a_uEntityThreshold);

		/*
		Usage: Sets the maximum entity queue process count.
		Arguments: uint a_uMaximumProcesses -> Input value to set.
		Output: ---
		*/
		// static void SetMaximumProcesses(uint a_uMaximumProcesses);

#pragma endregion

	private:

#pragma region // 	Fields

		// Values.
		uint m_uID = -1; // Octant ID.
		uint m_pOctreeID = -1; // Reference to the parent octree. (And therefore, the root).
		float m_fSize = DMIN_SIZE; // Minimum size.

		// Flags.
		bool m_bEnabled = false; // Octant enabled. (Read on the root octant).
		bool m_bVisible = false; // Visibility of the octant bounding box.

		// Vectors.
		matrix4 m_m4Translation = IDENTITY_M4; // The translation matrix. In most cases, this will not change.
		vector3 m_v3Center = ZERO_V3; // The center translation vector, in global space.
		vector3 m_v3Maximum = vector3(DMIN_SIZE / 2.0f); // The minimum extent vector, in global space.
		vector3 m_v3Minimum = vector3(-DMIN_SIZE / 2.0f); // The minimum extent vector, in global space.
		
		// Collection of child octants.
		// std::set<uint> m_lOctantIDs; // TODO: Phase out so that we can just use the managed vector list directly.

		// Collection of active child octants.
		// std::set<uint> m_lActiveOctantIDs; // TODO: Turn this into a method.

		// Collection of entity IDs, paired with this Octant.
		std::set<uint> m_lEntityIDs;
		
#pragma endregion

#pragma region // 	Memory Managed

		// Mesh manager reference.
		MeshManager* m_pMeshMngr = nullptr;

		// Entity manager reference.	
		MyEntityManager* m_pEntityMngr = nullptr; 	

		// Reference to children.
		std::vector<MyOctant*> m_vChildren;

		// Reference to parent.
		MyOctant* m_pParent = nullptr;

		// Reference to the root.
		MyOctant* m_pRoot = nullptr;

		// Reference to next sibling. (Useful for traversing through a collection of children).
		MyOctant* m_pNextSibling = nullptr;

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

#pragma region // 	Private Mutator Methods

		/*
		Usage: Sets the octant ID.
		Arguments: uint a_uID -> Octant ID to assign. If -1, gets ID based off of the octant count.
		Output: ---
		*/
		void SetID(uint a_uID);

		/*
		Usage: Sets the octant depth level.
		Arguments: uint a_uDepth -> Depth level to give the octant.
		Output: ---
		*/
		void SetDepth(uint a_uDepth);

		/*
		USAGE: Sets this octant's size. (via half-width property).
		ARGUMENTS: a_fSize -> Length of sides of the octant.
		OUTPUT: ---
		*/
		void SetSize(float a_fSize);

		/*
		USAGE: Set the center of the octant in global space.
		ARGUMENTS: a_v3Center -> Value to set center to.
		OUTPUT: ---
		*/
		void SetCenterGlobal(vector3 a_v3Center);

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
		USAGE: Sets the octant's parent.
		ARGUMENTS: MyOctant* const a_pParent -> pointer parent to set.
		OUTPUT: ---
		*/
		void SetParent(MyOctant* const a_pParent);
		
#pragma endregion

#pragma region // 	Private Accessor Methods

		/*
		USAGE: Gets this octant's size.
		ARGUMENTS: ---
		OUTPUT: Returns the size of the octant as a float.
		*/
		float GetSize(void) const;

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
		- MyOctant* a_pParent -> Parent octant pointer.
		- vector3 a_v3Center -> Center of the octant in global space.
		- float a_fSize -> Size of each of the sides of the octant volume.
		OUTPUT: class object.
		*/
		MyOctant(MyOctant* a_pParent, vector3 a_v3Center, float a_fSize);

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
		OUTPUT: class object reference.
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
		Usage: Sets visibility of octant OBB.
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
		USAGE: Gets the subdivision limit.
		ARGUMENTS: ---
		OUTPUT: Returns the subdivision limit.
		*/
		uint GetSubdivisionLimit(void);

		/*
		USAGE: Gets an octant by it's ID.
		ARGUMENTS: a_uID -> ID for octant.
		OUTPUT: Returns an octant pointer.
		*/
		void GetOctant(uint a_uID, MyOctant* o_pOctant);

		/*
		USAGE: Gets this octant's ID.
		ARGUMENTS: ---
		OUTPUT: Returns the ID of the octant.
		*/
		uint GetID(void) const;

		/*
		USAGE: Gets this octant's depth.
		ARGUMENTS: ---
		OUTPUT: Returns the depth level of the octant.
		*/
		uint MyOctant::GetDepth(void) const;

		/*
		USAGE: Returns the parent of this octant.
		ARGUMENTS: ---
		OUTPUT: MyOctant object (parent of this octant).
		*/
		MyOctant* GetParent(void) const;
		
		/*
		USAGE: Returns the root.
		ARGUMENTS: ---
		OUTPUT: MyOctant object (root of this octant's octree).
		*/
		MyOctant& GetRoot(void);
		
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
		
#pragma endregion

#pragma region // 	Service Methods
		
		/*
		USAGE: Send the octree to a string.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ToString(void);

		/*
		USAGE: Adds a collection of entities to the tree.
		ARGUMENTS: uint a_lIndices -> Indices of the entities to be added.
		OUTPUT: ---
		*/
		void AddEntities(std::set<uint> a_lIndices);

		/*
		USAGE: Adds a single entity to the tree.
		ARGUMENTS: uint a_uIndex -> Index of the entity to be added.
		OUTPUT: ---
		*/
		void AddEntity(uint a_uIndex);
				
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
		USAGE: Creates a tree using subdivisions, the max number of objects and levels.
		ARGUMENTS: uint a_uSubdivisionThreshold -> Sets the maximum level of the tree it is constructing.
		OUTPUT: ---
		*/
		void ConstructTree(uint a_uSubdivisionThreshold);

		/*
		USAGE: Creates a tree using global settings.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ConstructTree();
		
		/*
		USAGE: Traverse the tree to the leafs and sets the objects in them to the index.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AssignID(void);
		
		/*
		USAGE: Returns a vector that represents the center.
		ARGUMENTS: std::vector<vector3> points -> Contains a set of vector3 points.
		OUTPUT: Returns the center vector.
		*/
		vector3 FindCenter(std::vector<vector3> points);

		/*
		USAGE: Returns a vector that represents the maximum extent.
		ARGUMENTS: std::vector<vector3> points -> Contains a set of vector3 points.
		OUTPUT: Returns the maximum vector.
		*/
		vector3 FindMaximum(std::vector<vector3> points);

		/*
		USAGE: Returns a vector that represents the minimum extent.
		ARGUMENTS: std::vector<vector3> points -> Contains a set of vector3 points.
		OUTPUT: Returns the minimum vector.
		*/
		vector3 FindMinimum(std::vector<vector3> points);

#pragma endregion

	};
} // namespace Simplex

#endif //__MY_OCTANT_H_

#pragma region Octree Pseudocode.

/*
	Octree:
	-> Create the root octant.
		-> [Set the minimum size (half widths) for subdivision.]
			-> [Validate as a vector3 where every dimension is positive and non-zero.]
		-> [Set the maximum subdivision level.]
			-> [Validate as a positive, non-zero value (min: 1).]
		-> [Set the maximum/ideal entity count.]
			-> [Validate as a positive, non-zero value (min: 1).]

	-> When adding entities to the octree:
		-> [Check if entity is within the octant's bounds.]
		-> If true,
			-> [Check if the octant has any children.]
			-> If true,
				-> [Attempt to add octant to all children.]
			-> Else,
				-> [Check if the octant hasn't met the maximum subdivision threshold.]
				-> If true, // May need to subdivide.
					-> [Check if over the maximum/ideal entity count.]
					-> If true, // May need to subdivide.
						-> [Check if the octant is big enough to subdivide.]
						-> If true, // Subdivide.
							-> [Subdivide the octant.]
							-> [Attempt to add octant to all children.]
						-> Else,
							-> [Add entity to the octant's list.]
					-> Else, // No need to subdivide.
						-> [Add entity to the octant's list.]
				-> Else, // No need to subdivide.
					-> [Add entity to the octant's list.]
		-> Else,
			-> [Do not add the entity to the octant's list. (EXIT).]
*/

#pragma endregion

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */