/*
	MyOctree.h
	Definition for an Octree (octant node manager).
	Ian Effendi (iae2784@g.rit.edu)
*/

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/

#ifndef _MY_OCTREE_H_
#define _MY_OCTREE_H_

#include "Definitions.h"

// MyOctree is added to the simplex namespace.
namespace Simplex 
{

#pragma region //	Forward Declarations

	// Forward declaration of our MyEntityManager class.
	class MyEntity;

	// Forward declaration of our MyEntityManager class.
	class MyEntityManager;

	// Forward declaration of our Octant class. (Remember to #include "MyOctant.h" in .cpp file).
	class MyOctant;

#pragma endregion

	// The Octree manages individual Octants and updates them all in total.
	class MyOctree {

#pragma region //	Static Members

#pragma region //	Constants

		// Default maximum subdivision limit.
		const static uint DMAX_DEPTH = 2;

		// Default maximum capacity value.
		const static uint DMAX_CAPACITY = 5;

		// Default minimum size.
		const static uint DMIN_SIZE = 0.5;

#pragma endregion

#pragma region //	Static Fields

		// Number of octrees in use.		
		static uint s_uOctreeCount; 
		
		// Reference to the head octree.
		static MyOctree* s_pOctreeHead;

		// Initialization flag.
		static bool s_bInitialized;
				
#pragma endregion

#pragma region //	Static Methods

		/*
		USAGE: Initialize the static instance.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static void InitInstance(void);

		/*
		USAGE: Checks if static references have been initialized.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static bool IsInitialized(void);

		/*
		USAGE: Returns collection of available octrees.
		ARGUMENTS:
		- std::vector<MyOctant*> o_lOctrees -> Collection to add the head octree pointers to. (Will overwrite collection).
		- bool a_bEnabledOnly -> Search for only the enabled octree references.
		OUTPUT: Returns true if operation is successful.
		*/
		static bool GetOctrees(std::vector<MyOctree*> const& o_lOctrees, bool a_bEnabledOnly = false);

		/*
		USAGE: Returns collection of the IDs of available octrees.
		ARGUMENTS:
		- std::vector<uint> const& o_lOctreeIDs -> Collection to add the head octree IDs to. (Will overwrite collection).
		- bool a_bEnabledOnly -> Search for only the enabled octree references.
		OUTPUT: ---
		*/
		static bool GetOctreeIDs(std::vector<uint> const& o_lOctreeIDs, bool a_bEnabledOnly = false);

		/*
		USAGE: Returns the number of octrees available.
		ARGUMENTS: ---
		OUTPUT: Returns the number as an unsigned integer.
		*/
		static uint GetOctreeCount(void);
		
		/*
		USAGE: Returns the Octree requested by ID.
		ARGUMENTS: uint a_uIndex -> Index of Octree to find.
		OUTPUT: Returns pointer to Octree. Returns nullptr if ID was invalid.
		*/
		static MyOctree* GetOctree(uint a_uIndex);

		/*
		USAGE: Returns the zero-index octree, if one exists.
		ARGUMENTS: ---
		OUTPUT: Returns pointer to Octree. Returns nullptr if no octrees are available.
		*/
		static MyOctree* GetRootOctree(void);

		/*
		USAGE: Returns the Octree requested by ID.
		ARGUMENTS: uint a_uIndex -> Index of Octree to find.
		OUTPUT: Returns true if the octree exists.
		*/
		static bool HasOctree(uint a_uIndex);

		/*
		USAGE: Appends the input octree to the end of the node list.
		ARGUMENTS: MyOctree const* a_pOctree -> Octree to append.
		OUTPUT: --- 
		*/
		static void AppendOctree(MyOctree const* a_pOctree);

		/*
		USAGE: Release the memory for static instances, if root is nullptr.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static void ReleaseInstance(void);

#pragma endregion

#pragma endregion

	private:

#pragma region //	Fields.

		// Octree ID.
		uint m_uID = -1; 
		
		// Collection of octant ids.
		std::set<uint> m_lOctantIDs;

		// Collection of entity ids.
		std::set<uint> m_lEntityIDs;

		// Subdivision limit for this octree.
		uint m_uSubdivisionLimit = 0;

		// Subdivision capacity trigger.
		uint m_uOctantCapacity = 5;

		// Minimum size limit for this octree.
		float m_uMinimumSize = 0.5f;
				
		// Active flag.
		bool m_bEnabled = false;

		// Visibility flag.
		bool m_bVisible = false;
		
#pragma endregion

#pragma region //	Memory Managed

		// Reference to the mesh manager.
		MeshManager* m_pMeshMngr = nullptr;

		// Reference to the entity manager.
		MyEntityManager* m_pEntityMngr = nullptr;

		// Reference to the next octree.
		MyOctree* m_pNeighbor = nullptr;

		// Pointer to root octant.
		MyOctant* m_pRoot = nullptr;
		
#pragma endregion
		
#pragma region //	Helper Methods

		/*
		USAGE: Sets the ID of this octree, if the input value is valid.
		ARGUMENTS: uint a_uID -> ID value to assign. 
		OUTPUT: ---
		*/
		void SetID(uint a_uID);

		/*
		USAGE: Changes the root of the octree, if, and only if, the current root is null.
		ARGUMENTS: MyOctant const* a_pRoot -> Pointer to the root that will be set.
		OUTPUT: ---
		*/
		void SetRoot(MyOctant const& a_pRoot);

		/*
		USAGE: Sets the capacity of the octants in this octree, if the input value is valid.
		ARGUMENTS: uint a_uCapacity -> value to assign.
		OUTPUT: ---
		*/
		void SetOctantCapacity(uint a_uCapacity);

		/*
		USAGE: Sets the subdivision limit of the octants in this octree, if the input value is valid.
		ARGUMENTS: uint a_uLimit -> value to assign.
		OUTPUT: ---
		*/
		void SetSubdivisionLimit(uint a_uLimit);

		/*
		USAGE: Sets the minimum size of the octants in this octree, if the input value is valid.
		ARGUMENTS: uint a_fSize -> value to assign.
		OUTPUT: ---
		*/
		void SetMinimumSize(float a_fSize);

		/*
		USAGE: Clears the entity list.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearEntityList(void);

		/*
		USAGE: Clears, and releases, all of the octants referenced.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearOctantList(void);

		/*
		USAGE: Adds the entity to the list.
		ARGUMENTS: uint a_uEntityID -> ID of entity.
		OUTPUT: ---
		*/
		void AddEntityToList(uint a_uEntityID);

		/*
		USAGE: Adds the octant to the list.
		ARGUMENTS: uint a_uOctantID -> ID of octant.
		OUTPUT: ---
		*/
		void AddOctantToList(uint a_uOctantID);

#pragma endregion

#pragma region //	Init/Release Methods

		/*
		USAGE: Sets the ID of this octree, if the input value is valid.
		ARGUMENTS: uint a_uID -> ID value to assign.
		OUTPUT: ---
		*/
		void Init(void);
				
		/*
		USAGE: Sets the ID of this octree, if the input value is valid.
		ARGUMENTS: uint a_uID -> ID value to assign.
		OUTPUT: ---
		*/
		void Release(void);

#pragma endregion

	public:
		
#pragma region //	Flag Methods

		/*
		USAGE: Checks if the input value and this Octree's ID matches.
		ARGUMENTS: uint a_uID -> ID to check match for.
		OUTPUT: Returns true if match. Returns false if no match or invalid input.
		*/
		bool Is(uint a_uID) const;

		/*
		USAGE: Checks if the input octant ID exists in this tree's collection.
		ARGUMENTS: uint a_uOctantID -> ID to check for.
		OUTPUT: Returns true if match. Returns false if no match or invalid input.
		*/
		bool ContainsOctant(uint a_uOctantID) const;

		/*
		USAGE: Checks if the input entity ID exists in this tree's collection.
		ARGUMENTS: uint a_uEntityID -> ID to check for.
		OUTPUT: Returns true if match. Returns false if no match or invalid input.
		*/
		bool ContainsEntity(uint a_uEntityID) const;
		
		/*
		USAGE: Returns value of enable flag property.
		ARGUMENTS: ---
		OUTPUT: Returns enabled flag of this octree.
		*/
		bool IsEnabled(void) const;

		/*
		USAGE: Returns value of visibility flag property.
		ARGUMENTS: ---
		OUTPUT: Returns visibility flag of this octree.
		*/
		bool IsVisible(void) const;

		/*
		USAGE: Checks if there are any octants in this tree.
		ARGUMENTS: ---
		OUTPUT: Returns true if there are no octants.
		*/
		bool IsEmpty(void) const;
		
#pragma endregion

#pragma region //	Mutator Methods

		/*
		USAGE: Adds the Entity to the Octree, if input is valid.
		ARGUMENTS: uint a_uEntityID -> ID of entity.
		OUTPUT: ---
		*/
		void AddEntity(uint a_uEntityID);
		
		/*
		USAGE: Removes the Entity from the Octree and its octants, if input is valid and it exists. (Does not delete the entity).
		ARGUMENTS: uint a_uEntityID -> ID of entity.
		OUTPUT: ---
		*/
		void RemoveEntity(uint a_uEntityID);

		/*
		USAGE: Enables this tree and all of the octants within it.
		ARGUMENTS: bool a_bEnabled -> Enable flag value to assign.
		OUTPUT: ---
		*/
		void SetEnabled(bool a_bEnabled);

		/*
		USAGE: Sets visibility of this tree and all of the octants within it.
		ARGUMENTS: bool a_bVisible -> Visibility flag value to assign.
		OUTPUT: ---
		*/
		void SetVisibility(bool a_bVisible);

		/*
		USAGE: Sets enabled flag and visibility of this tree and all of the octants within it.
		ARGUMENTS:
		- bool a_bEnabled -> Enable flag value to assign.
		- bool a_bVisible -> Visibility flag value to assign.
		OUTPUT: ---
		*/
		void SetEnabledAndVisibility(bool a_bEnabled, bool a_bVisible);

#pragma endregion

#pragma region //	Accessor Methods

		/*
		USAGE: Get the ID.
		ARGUMENTS: ---
		OUTPUT: Returns value.
		*/
		uint GetID(void) const;

		/*
		USAGE: Get reference to the root octant.
		ARGUMENTS: MyOctant** o_pOctant -> Pointer that will carry the reference to the root octant pointer.
		OUTPUT: ---
		*/
		void GetRoot(MyOctant** o_pOctant);

		/*
		USAGE: Get reference to the root octant.
		ARGUMENTS: ---
		OUTPUT: MyOctant* -> Returns root pointer.
		*/
		MyOctant* GetRoot(void) const;
		
		/*
		USAGE: Gets the capacity of the octants in this octree.
		ARGUMENTS: ---
		OUTPUT: Returns requested value.
		*/
		uint GetOctantCapacity(void);

		/*
		USAGE: Gets the subdivision limit of the octants in this octree.
		ARGUMENTS: ---
		OUTPUT: Returns requested value.
		*/
		uint GetSubdivisionLimit(void);

		/*
		USAGE: Gets the minimum size of the octants in this octree.
		ARGUMENTS: ---
		OUTPUT: Returns requested value.
		*/
		float GetMinimumSize(void);

		/*
		USAGE: Get the octant count.
		ARGUMENTS: ---
		OUTPUT: Returns octant count.
		*/
		uint GetOctantCount(void) const;

		/*
		USAGE: Get the entity count.
		ARGUMENTS: ---
		OUTPUT: Returns entity count.
		*/
		uint GetEntityCount(void) const;

		/*
		USAGE: Returns entity from the MyEntityManager singleton, if it exists.
		ARGUMENTS: uint a_uEntityID -> ID of entity.
		OUTPUT: MyEntity* -> Returns pointer to entity. nullptr if invalid.
		*/
		MyEntity* GetEntity(uint a_uEntityID) const;

		/*
		USAGE: Returns entity from the MyEntityManager singleton, if it exists.
		ARGUMENTS: 
		- MyEntity** o_pEntity -> Pointer to the pointer of the entity requested, if it exists.
		- uint a_uEntityID -> ID of entity.
		OUTPUT: ---
		*/
		void GetEntity(MyEntity** o_pEntity, uint a_uEntityID);

		/*
		USAGE: Places collection of octant IDs into supplied list, for all the octants the entity resides in.
		ARGUMENTS: 
		- std::vector<MyOctant*> const& o_lDimensions -> List that matching IDs will be added to.
		- uint a_uEntityID -> ID of entity.
		OUTPUT: ---
		*/
		void GetEntityDimensions(std::vector<MyOctant*> const& o_lDimensions, uint a_uEntityID);

		/*
		USAGE: Places collection of octant IDs into supplied list, for all the octants the entity resides in.
		ARGUMENTS: uint a_uEntityID -> ID of entity.
		OUTPUT: std::vector<MyOctant*>* -> Pointer to list that matching IDs will be added to. Returns nullptr if invalid. Returns empty list when not contained within this octree's octants.
		*/
		std::vector<MyOctant*>* GetEntityDimensions(uint a_uEntityID);

		/*
		USAGE: Returns octant from the collection, if it exists.
		ARGUMENTS: uint a_uOctantID -> ID of entity.
		OUTPUT: MyOctant* -> Returns pointer to entity. nullptr if invalid.
		*/
		MyOctant* GetOctant(uint a_uOctantID) const;

		/*
		USAGE: Returns octant from the collection, if it exists.
		ARGUMENTS:
		- MyOctant** o_pOctant -> Pointer to the pointer of the octant requested, if it exists.
		- uint a_uOctantID -> ID of octant.
		OUTPUT: ---
		*/
		void GetOctant(MyOctant** o_pOctant, uint a_uOctantID);
		
#pragma endregion

#pragma region //	Constructor(s).

		/*
		USAGE: Creates an octree, while specifying the subdivision limit, entity threshold, and minimum size of the octants.
		ARGUMENTS:
		- a_uSubdivisionLimit = MyOctree::DMAX_DEPTH -> Depth level limit.
		- a_uEntityThreshold = MyOctree::DMAX_CAPACITY -> Capacity that triggers subdivision.
		- a_fMinimumSize = MyOctree::DMIN_SIZE -> Minimum size allowed for an octant (after subdivision).
		OUTPUT: ---
		*/
		MyOctree(uint a_uSubdivisionLimit = MyOctree::DMAX_DEPTH, uint a_uEntityThreshold = MyOctree::DMAX_CAPACITY, float a_fMinimumSize = MyOctree::DMIN_SIZE);

		/*
		USAGE: Creates an octree from an input root.
		ARGUMENTS: MyOctant const& m_pRoot -> Reference to a root octant that will be (deep) copied into the tree.
		OUTPUT: ---
		*/
		MyOctree(MyOctant const& m_pRoot);

#pragma endregion

#pragma region //	Rule of Three.

		/*
		USAGE: Creates a duplicate octree, performing a deep copy on all of its elements, but, with a new ID.
		ARGUMENTS: MyOctree const& other -> Octree being copied into this class.
		OUTPUT: ---
		*/
		MyOctree(MyOctree const& other);

		/*
		USAGE: Performs a deep copy of an existing octree. Does not keep the ID.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: class object reference.
		*/
		MyOctree& operator=(MyOctree const& other);

		/*
		USAGE: Destructor.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~MyOctree(void);

#pragma endregion
		
#pragma region //	Rule of Five.
		
		/*
		USAGE: Move constructor. Deep copies, but, keeps ID the same.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: class object instance.
		*/
		MyOctree(MyOctree&& other);

		/*
		USAGE: Move assignment operator. Deep copies, but, keeps ID the same.
		ARGUMENTS: other -> class object to copy.
		OUTPUT: class object reference.
		*/
		MyOctree& operator=(MyOctree&& other);

		/*
		USAGE: Changes the object's contents for the input object.
		ARGUMENTS: MyOctree& other -> The other object to swap contents with.
		OUTPUT: ---
		*/
		void Swap(MyOctree& other);

#pragma endregion

#pragma region //	Service Methods

		/*
		USAGE: Updates all of the entities in the octree's root list, adding them if they aren't already in the octants.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void UpdateEntities(void);

		/*
		USAGE: Updates the Entity in the Octree, adding it if it doesn't already exist.
		ARGUMENTS: uint a_uEntityID -> ID of entity.
		OUTPUT: ---
		*/
		void UpdateEntity(uint a_uEntityID);

		/*
		USAGE: Updates the Octants in the Octree, subdividing them if they need to be.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void UpdateOctants(void);

		/*
		USAGE: Updates the Octree, removing empty octant nodes and entities if necessary.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Update(void);

		/*
		USAGE: Will add Octant to the render list.
		ARGUMENTS:
		-	uint a_uOctantID -> Id of the octant to draw.
		-	bool a_bIncludeChildren = true -> draw the octant's children as well?
		-	bool a_bIncludeEmpty = true -> draw octant when empty.
		-	bool a_bOnlyLeafs = true -> draw octant, but only if it is a leaf.
		-	bool a_bRigidBody = false -> draw rigid body?
		OUTPUT: ---
		*/
		void AddOctantToRenderList(uint a_uOctantID = -1, bool a_bIncludeEmpty = true, bool a_bOnlyLeafs = true, bool a_bRigidBody = false);

		/*
		USAGE: Will add Octant to the render list.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AddLeavesToRenderList(void);

		/*
		USAGE: Will add Octant and its children to the render list.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AddBranchToRenderList(void);
		
#pragma endregion
		
	}; // class.
} // Simplex namespace.

#endif // _MY_OCTREE_H_ (include guard).


#pragma region //	_

#pragma endregion

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/