/*
	MyOctant.cpp
	Implementation for the Octree node.
	Ian Effendi (iae2784@g.rit.edu)
*/

#pragma region Table of Contents.

	// TODO.

#pragma endregion

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
						-> [Check if the octant has met the maximum subdivision threshold.]
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

#include "MyOctant.h"

namespace Simplex {
	
#pragma region Static Members

	// Set default values for static values.
	uint MyOctant::sOctantCount = 0;
	uint MyOctant::sMinimumSize = MyOctant::DMIN_SIZE;
	uint MyOctant::sDepthThreshold = MyOctant::DMAX_DEPTH;
	uint MyOctant::sEntityThreshold = MyOctant::DMAX_CAPACITY;
	uint MyOctant::sMaximumProcesses = MyOctant::DMAX_PROCESSES;

	/*
	Usage: Sets the minimum size of all octants.
	Arguments: uint a_uMinimumSize -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMinimumSize(uint a_uMinimumSize)
	{
		if (a_uMinimumSize <= 0 || a_uMinimumSize == -1) { a_uMinimumSize = MyOctant::DMIN_SIZE; }
		MyOctant::sMinimumSize = a_uMinimumSize;
	}

	/*
	Usage: Sets the maximum subdivision level an octant can have.
	Arguments: uint a_uDepthThreshold -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMaximumDepth(uint a_uDepthThreshold)
	{
		if (a_uDepthThreshold <= 0 || a_uDepthThreshold == -1) { a_uDepthThreshold = MyOctant::DMAX_DEPTH; }
		MyOctant::sDepthThreshold = a_uDepthThreshold;
	}

	/*
	Usage: Sets the ideal entity count.
	Arguments: uint m_uEntityThreshold -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMaximumCapacity(uint a_uEntityThreshold)
	{
		if (a_uEntityThreshold <= 0 || a_uEntityThreshold == -1) { a_uEntityThreshold = MyOctant::DMAX_CAPACITY; }
		MyOctant::sEntityThreshold = a_uEntityThreshold;
	}

	/*
	Usage: Sets the maximum entity queue process count.
	Arguments: uint a_uMaximumProcesses -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMaximumProcesses(uint a_uMaximumProcesses)
	{
		if (a_uMaximumProcesses <= 0 || a_uMaximumProcesses == -1) { a_uMaximumProcesses = MyOctant::DMAX_PROCESSES; }
		MyOctant::sMaximumProcesses = a_uMaximumProcesses;
	}

#pragma endregion

#pragma region Private
	
#pragma region // 	Fields

	/*

	uint m_uID = 0;	// This octant's current ID.
	uint m_uDepth = 0;	// The current subdivision level
	uint m_uChildCount = 0;	// Number of children in the octant.

	vector3 m_v3Active = C_YELLOW; // Active color (when the octant or its children contain entities).
	vector3 m_v3Inactive = C_WHITE; // Inactive color (when the octant has no children and is empty).

	float m_fSize = 0.0f; // Size of a side of the octant.
	vector3 m_v3Center = ZERO_V3; // Stores the center point of the octant. (Global)
	vector3 m_v3Minimum = ZERO_V3; // Stores the minimum vector of the octant. (Global)
	vector3 m_v3Maximum = ZERO_V3; // Stores the maximum vector of the octant. (Global)

	// We use a set to take advantage of it's automatic handling of duplicates.
	std::set<uint> m_lEntities;  // List of entities contained within the octant.	
	std::vector<uint> m_lRoot; // All entities contained in the octree.

	// Reference to list of nodes that will contain objects.
	std::vector<MyOctant*> m_lActiveChildren;
	std::vector<MyOctant*> m_lChildren;

	bool m_bEnabled = false; // Octant enabled. (Read on the root octant).
	bool m_bVisibleOBB = false; // Visibility of the octant bounding box.

	MeshManager* m_pMeshManager = nullptr; // Mesh manager reference.
	MyEntityManager* m_pEntityManager = nullptr; // Entity manager reference.

	MyOctant* m_pParent = nullptr; // Parent octant.

	*/

#pragma endregion

#pragma region // 	Helper Methods

	/*
	USAGE: Deallocates the member fields in need of memory management.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Release(void) 
	{
		// Reduce octant count by one.
		sOctantCount--;
		if (sOctantCount == -1) { sOctantCount = 0; }
		
		// Clear the active children list.
		this->m_lRoot.clear();
		this->m_lActiveChildren.clear();

		// Delete children.
		this->ClearChildren();

		// Remove null pointer references to parent.
		this->SetParent(nullptr);

		// Remove references to singletons.
		this->m_pMeshManager = nullptr;
		this->m_pEntityManager = nullptr;
	}

	/*
	USAGE: Initializes the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Init(void) 
	{
		// Initialize fields.
		InitVariables();

		// Collections.
		this->m_lEntities = std::set<uint>();
		this->m_lRoot = std::set<uint>();
		this->m_lActiveChildren = std::vector<MyOctant*>();
		this->m_lChildren = std::vector<MyOctant*>();

		// Initialize memory.
		this->SetParent(nullptr);
		
		// Set singletons.
		this->m_pMeshManager = MeshManager::GetInstance();
		this->m_pEntityManager = MyEntityManager::GetInstance();				
	}

	/*
	USAGE: Allocates member field data and initializes the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::InitVariables(void) 
	{
		// Set default properties.
		this->SetEnabled(true);
		this->SetOBBVisbile(false);
		
		// Set values.
		this->SetSize(sMinimumSize);

		// Set colors.
		vector3 m_v3Active = C_YELLOW;
		vector3 m_v3Inactive = C_WHITE;
	}

	/*
	USAGE: Creates the list of all leaves that contain objects.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ConstructList(void) 
	{
		// Check if this is a leaf.
		if (IsLeaf() && !IsEmpty()) 
		{
			// If there are no children and this contains entities, add it to the active children collection.
			this->GetRoot().m_lActiveChildren.push_back(this);
			this->GetParent()->m_lActiveChildren.push_back(this);
		}
		else if(!IsLeaf())
		{
			// If there are children, query each of them.
			for (uint i = 0; i < m_lChildren.size(); i++) 
			{
				MyOctant child = *this->GetChild(i);
				child.ConstructList();	
			}
		}
	}

#pragma endregion

#pragma region // 	Private Mutator Methods

	/*
	Usage: Sets the octant ID.
	Arguments: uint a_uID -> Octant ID to assign. If -1, gets ID based off of the octant count.
	Output: ---
	*/
	void MyOctant::SetID(uint a_uID) 
	{
		if (a_uID == -1) { a_uID = (this->GetOctantCount() - 1); }
		if (a_uID == -1) { a_uID = 0; }
		this->m_uID = a_uID;
	}

	/*
	Usage: Sets the octant depth level.
	Arguments: uint a_uDepth -> Depth level to give the octant.
	Output: ---
	*/
	void MyOctant::SetDepth(uint a_uDepth) 
	{
		// If invalid value, do not do anything.
		if (a_uDepth == -1) { return; }
		this->m_uDepth = a_uDepth;
	}

	/*
	USAGE: Sets this octant's size. (via half-width property).
	ARGUMENTS: a_fSize -> Length of sides of the octant.
	OUTPUT: ---
	*/
	void MyOctant::SetSize(float a_fSize) 
	{
		// Clamp to the minimum size.
		if (a_fSize < sMinimumSize) { a_fSize = sMinimumSize; }
		this->m_fSize = (a_fSize + sMinimumSize * 4.0f) / 2.0f;

		// Calculate the min and max corners.
		this->m_v3Maximum = m_v3Center + vector3(m_fSize / 2.0f); // vector3(m_v3Center.x + fdelta, m_v3Center.y + fdelta, m_v3Center.z + fdelta);
		this->m_v3Minimum = m_v3Center - vector3(m_fSize / 2.0f); // vector3(m_v3Center.x - fdelta, m_v3Center.y - fdelta, m_v3Center.z - fdelta);
	}

	/*
	USAGE: Set the center of the octant in global space.
	ARGUMENTS: a_v3Center -> Value to set center to.
	OUTPUT: ---
	*/
	void MyOctant::SetCenterGlobal(vector3 a_v3Center) 
	{
		// Validate vector.
		float min = FLT_EPSILON;
		if (a_v3Center == ZERO_V3) { a_v3Center = vector3(min); }
		else {
			if (a_v3Center.x == ZERO_V3.x) { a_v3Center.x = min; }
			if (a_v3Center.y == ZERO_V3.y) { a_v3Center.y = min; }
			if (a_v3Center.z == ZERO_V3.z) { a_v3Center.z = min; }
		}
		this->m_v3Center = a_v3Center;
	}

	/*
	USAGE: Set the maximum vector in global space.
	ARGUMENTS: a_v3Maximum -> Value to set maximum vector to.
	OUTPUT: ---
	*/
	void MyOctant::SetMaxGlobal(vector3 a_v3Maximum)
	{
		// Validate vector.
		float min = FLT_EPSILON;
		if (a_v3Maximum == ZERO_V3) { a_v3Maximum = vector3(min); }
		else {
			if (a_v3Maximum.x == ZERO_V3.x) { a_v3Maximum.x = min; }
			if (a_v3Maximum.y == ZERO_V3.y) { a_v3Maximum.y = min; }
			if (a_v3Maximum.z == ZERO_V3.z) { a_v3Maximum.z = min; }
		}
		this->m_v3Maximum = a_v3Maximum;
	}

	/*
	USAGE: Set the minimum vector in global space.
	ARGUMENTS: a_v3Minimum -> Value to set minimum vector to.
	OUTPUT: ---
	*/
	void MyOctant::SetMinGlobal(vector3 a_v3Minimum)
	{
		// Validate vector.
		float min = -FLT_EPSILON;
		if (a_v3Minimum == ZERO_V3) { a_v3Minimum = vector3(min); }
		else {
			if (a_v3Minimum.x == ZERO_V3.x) { a_v3Minimum.x = min; }
			if (a_v3Minimum.y == ZERO_V3.y) { a_v3Minimum.y = min; }
			if (a_v3Minimum.z == ZERO_V3.z) { a_v3Minimum.z = min; }
		}
		this->m_v3Maximum = a_v3Minimum;
	}

	/*
	USAGE: Sets the octant reference at the specified index to the input value (if possible).
	ARGUMENTS:
	- uint a_uChild -> index of the child [0, 7].
	- MyOctant* const a_pChild -> pointer child to add.
	OUTPUT: ---
	*/
	void MyOctant::SetChild(uint a_uChild, MyOctant* const a_pChild) 
	{
		// If index is not valid, no child to set.
		if (a_uChild == -1 || a_uChild > 7 || IsLeaf()) { return; }
		this->m_lChildren[a_uChild] = a_pChild;
	}

	/*
	USAGE: Sets the octant's parent.
	ARGUMENTS: MyOctant* const a_pParent -> pointer parent to set.
	OUTPUT: ---
	*/
	void MyOctant::SetParent(MyOctant* const a_pParent) 
	{
		// Check to make sure this isn't a circular reference to itself.
		if (this != a_pParent) {
			this->m_pParent = a_pParent;
		}

		if (this->m_pParent == nullptr) 
		{
			// Set the depth level (correctly) to zero, if there is no parent.
			this->SetDepth(0);
		}
		else 
		{
			// Set depth level to 1 more than the parent's.
			this->SetDepth(this->m_pParent->GetDepth() + 1);
		}
	}
	
#pragma endregion

#pragma region // 	Private Accessor Methods

	/*
	USAGE: Gets this octant's size.
	ARGUMENTS: ---
	OUTPUT: Returns the size of the octant as a float.
	*/
	float MyOctant::GetSize(void) const 
	{
		return this->m_fSize;
	}

	/*
	USAGE: Get the center of the octant in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetCenterGlobal(void) const 
	{
		return this->m_v3Center;
	}

	/*
	USAGE: Get the maximum vector in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetMaxGlobal(void) const 
	{
		return this->m_v3Maximum;
	}

	/*
	USAGE: Get the minimum vector in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetMinGlobal(void) const
	{
		return this->m_v3Minimum;
	}

	/*
	USAGE: Returns the child at the specified index (if possible).
	ARGUMENTS: uint a_uChild -> index of the child [0, 7].
	OUTPUT: MyOctant object (child at index).
	*/
	MyOctant* MyOctant::GetChild(uint a_uChild) const 
	{
		// If index is out of bounds or if this is has no children, return nullptr.
		if (a_uChild == -1 || a_uChild > m_lChildren.size() || IsLeaf()) { return nullptr; }
		return this->m_lChildren[a_uChild];
	}

#pragma endregion

#pragma endregion
	
#pragma region Public
	
#pragma region // 	Constructor(s)

	/*
	USAGE: Constructor will create an octant containing all the Entities that the EntityManager currently contains.
	ARGUMENTS:
	- uint a_uSubdivisionLimit = 2 -> Sets the maximum level of subdivisions.
	- uint a_uIdealEntityCount = 5 -> Sets the ideal level of objects per octant.
	- uint a_uMinimumSize = 5 -> Sets the minimum size threshold an octant must have if it is to be subdivided.
	OUTPUT: class object.
	*/
	MyOctant::MyOctant(uint a_uSubdivisionLimit, uint a_uIdealEntityCount, uint a_uMinimumSize) 
	{
		// Set the global settings.
		this->SetMaximumDepth(a_uSubdivisionLimit);
		this->SetMaximumCapacity(a_uIdealEntityCount);
		this->SetMinimumSize(a_uMinimumSize);
		
		// Initialize the octant.
		this->Init();

		// Prepare the root.
		this->SetID(0);
		this->SetOBBVisbile(true); // The root will show its OBB by default.
		
		// The root doesn't have a parent; that's what makes it a root. It's a rebel.
		this->SetParent(nullptr); // Also sets the depth to zero.
		
		// Increase the octant count on creation.
		sOctantCount++;
	}

	/*
	USAGE: Constructs an octant at a center point using uniform sides.
	ARGUMENTS:
	- MyOctant* a_pParent -> Parent octant pointer.
	- vector3 a_v3Center -> Center of the octant in global space.
	- float a_fSize -> Size of each of the sides of the octant volume.
	OUTPUT: class object.
	*/
	MyOctant::MyOctant(MyOctant* a_pParent, vector3 a_v3Center, float a_fSize) 
	{
		// Initialize the octant.
		this->Init();

		// Prepare the octant.
		this->SetOBBVisbile(true); // The root will show its OBB by default.
		this->SetParent(a_pParent);

		// Set the center and size.
		this->m_v3Center = a_v3Center;
		this->SetSize(a_fSize);
		
		// Increase the octant count on creation.
		sOctantCount++;
		this->SetID(-1); // Sets the octant ID to one less than the octant count.		
	}

#pragma endregion

#pragma region // 	Rule of Three

	/*
	USAGE: Makes a deep clone of the octant.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object instance.
	*/
	MyOctant::MyOctant(MyOctant const& other) 
	{
		// Copy the fields.
		this->SetID(other.GetID());
		this->SetDepth(other.GetDepth());

		// References
		this->m_pMeshManager = other.m_pMeshManager;
		this->m_pEntityManager = other.m_pEntityManager;

		// Other values.
		this->SetEnabled(other.m_bEnabled);
		this->SetOBBVisbile(other.m_bVisibleOBB);

		this->SetParent(other.GetParent());
		this->SetCenterGlobal(other.GetCenterGlobal());
		this->SetSize(other.GetSize());

		this->m_lActiveChildren = other.m_lActiveChildren;
		this->m_lChildren = other.m_lChildren;
		this->m_lEntities = other.m_lEntities;
	}

	/*
	USAGE: Makes a deep clone of the octant.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object reference.
	*/
	MyOctant& MyOctant::operator=(MyOctant const& other)
	{
		// If not the same as the other.
		if (this != &other) 
		{
			this->Release(); // Release this object's resources.
			MyOctant temp(other); // Make a temporary copy.
			this->Swap(temp);		
		}

		return *this;
	}

	/*
	USAGE: Destructor.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	MyOctant::~MyOctant(void) 
	{
		// Release resources.
		this->Release();
	}

#pragma endregion

#pragma region // 	Rule of Five

	/*
	USAGE: Move constructor.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object instance.
	*/
	MyOctant::MyOctant(MyOctant&& other) 
	{
		// Copy the fields.
		this->SetID(other.GetID());
		this->SetDepth(other.GetDepth());

		// References
		this->m_pMeshManager = other.m_pMeshManager;
		this->m_pEntityManager = other.m_pEntityManager;

		// Other values.
		this->SetEnabled(other.m_bEnabled);
		this->SetOBBVisbile(other.m_bVisibleOBB);

		this->SetParent(other.GetParent());
		this->SetCenterGlobal(other.GetCenterGlobal());
		this->SetSize(other.GetSize());

		this->m_lActiveChildren = other.m_lActiveChildren;
		this->m_lChildren = other.m_lChildren;
		this->m_lEntities = other.m_lEntities;

		// Release temporary.
		other.Release();
	}

	/*
	USAGE: Move assignment operator.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: ---
	*/
	MyOctant& MyOctant::operator=(MyOctant&& other)
	{
		// If not the same as the other.
		if (this != &other)
		{
			this->Release(); // Release this object's resources.
			MyOctant temp(other); // Make a temporary copy.
			this->Swap(temp);
			other.Release(); // Release the other object.
		}

		return *this;
	}

	/*
	USAGE: Changes the object's contents for the input object.
	ARGUMENTS: MyOctant& other -> The other object to swap contents with.
	OUTPUT: ---
	*/
	void MyOctant::Swap(MyOctant& other)
	{
		// Swap values.
		std::swap(m_uID, other.m_uID);
		std::swap(m_uDepth, other.m_uDepth);

		std::swap(m_bEnabled, other.m_bEnabled);
		std::swap(m_bVisibleOBB, other.m_bVisibleOBB);

		// Swap vector3s.
		std::swap(m_fSize, other.m_fSize);
		std::swap(m_v3Center, other.m_v3Center);
		std::swap(m_v3Maximum, other.m_v3Maximum);
		std::swap(m_v3Minimum, other.m_v3Minimum);

		std::swap(m_v3Active, other.m_v3Active);
		std::swap(m_v3Inactive, other.m_v3Inactive);

		// Swap collections.;
		std::swap(m_lEntities, other.m_lEntities);
		std::swap(m_lActiveChildren, other.m_lActiveChildren);
		std::swap(m_lChildren, other.m_lChildren);

		// Swap memory.
		std::swap(m_pMeshManager, other.m_pMeshManager);
		std::swap(m_pEntityManager, other.m_pEntityManager);
		std::swap(m_pParent, other.m_pParent);
	}

#pragma endregion

#pragma region // 	Flag Methods

	/*
	USAGE: Asking if there is a collision with an Entity as supplied by the input index value.
	ARGUMENTS: uint a_uRBIndex -> Index of the Entity in the EntityManager.
	OUTPUT: Check of the collision.
	*/
	bool MyOctant::IsColliding(uint a_uRBIndex) 
	{
		// Validate.
		if (a_uRBIndex == -1) { return false; }

		// Check if the entity exists.
		MyEntity* entity = m_pEntityManager->GetEntity(a_uRBIndex);
		if (entity == nullptr) { return false; }

		// Check if rigidbody exists.
		MyRigidBody* entityRB = m_pEntityManager->GetRigidBody();
		if (entityRB == nullptr) { return false; }

		// Create the octant's rigidbody.
		std::vector<vector3> points =
		{
			this->m_v3Center + vector3(m_fSize / 2.0f),
			this->m_v3Center - vector3(m_fSize / 2.0f)
		};
		MyRigidBody* octantRB = new MyRigidBody(points);

		octantRB->SetVisibleARBB(false);
		octantRB->SetVisibleOBB(false);
		octantRB->SetVisibleBS(false);
		
		// Check if the entityRB is colliding with the octant.
		bool collision = entityRB->IsColliding(octantRB);

		// Release memory.
		delete octantRB;

		// Return value of the collision.
		return collision;
	}

	/*
	USAGE: Asks if this octant has no parents.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsRoot(void) const 
	{
		// If the parent is null, this is the root.
		return (this->GetParent() == nullptr);
	}

	/*
	USAGE: Asks if this octant has no children.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsLeaf(void) const 
	{
		// If there are no children, this is a leaf.
		return (this->m_lChildren.size() == 0);
	}

	/*
	USAGE: Asks if this octant has no entities.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsEmpty(void) const 
	{
		// If there are entities in the list, this is not empty.
		return (this->GetEntityCount() == 0);
	}

	/*
	USAGE: Checks to see if this octant has more than the input number of entities available.
	ARGUMENTS: uint a_uEntities -> Number of entities to check for.
	OUTPUT: Returns check.
	*/
	bool MyOctant::ContainsMoreThan(uint a_uEntities) 
	{
		// If -1 is input, return false.
		if (a_uEntities == -1) { return false; }
		return (this->GetEntityCount() > a_uEntities);
	}

	/*
	Usage: Gets visibility of oriented bounding box.
	Arguments: ---
	Output: Returns check.
	*/
	bool MyOctant::IsOBBVisible(void) const 
	{
		return this->m_bVisibleOBB;
	}

	/*
	Usage: Gets enabled flag of octant.
	Arguments: ---
	Output: Returns check.
	*/
	bool MyOctant::IsEnabled(void) const 
	{
		return this->m_bEnabled;
	}

#pragma endregion

#pragma region // 	Mutator Methods

	/*
	Usage: Sets visibility of octant OBB.
	Arguments: bool a_bVisibleOBB -> Input value to set OBB visibility flag to.
	Output: ---
	*/
	void MyOctant::SetOBBVisbile(bool a_bVisibleOBB) 
	{
		this->m_bVisibleOBB = a_bVisibleOBB;

		// Set same to children.
		if (!IsLeaf()) 
		{
			for (uint i = 0; i < m_lChildren.size(); i++) 
			{
				m_lChildren[i]->SetOBBVisbile(a_bVisibleOBB);
			}
		}
	}

	/*
	Usage: Sets enabled flag of octant.
	Arguments: bool a_bEnabled -> Input value to set enable flag to.
	Output: ---
	*/
	void MyOctant::SetEnabled(bool a_bEnabled) 
	{
		this->m_bEnabled = a_bEnabled;

		// Set same to children.
		if (!IsLeaf())
		{
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				m_lChildren[i]->SetEnabled(a_bEnabled);
			}
		}
	}
	
	/*
	USAGE: Set the active color of the octant.
	ARGUMENTS: vector3 a_v3Color -> Input color value to assign.
	OUTPUT: ---
	*/
	void MyOctant::SetActiveColor(vector3 a_v3Color) 
	{
		this->m_v3Active = a_v3Color;

		// Set same to children.
		if (!IsLeaf())
		{
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				m_lChildren[i]->SetActiveColor(a_v3Color);
			}
		}
	}

	/*
	USAGE: Set the inactive color of the octant.
	ARGUMENTS: vector3 a_v3Color -> Input color value to assign.
	OUTPUT: ---
	*/
	void MyOctant::SetInactiveColor(vector3 a_v3Color)
	{
		this->m_v3Inactive = a_v3Color;

		// Set same to children.
		if (!IsLeaf())
		{
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				m_lChildren[i]->SetInactiveColor(a_v3Color);
			}
		}
	}

#pragma endregion

#pragma region // 	Accessor Methods

	/*
	USAGE: Gets the subdivision limit.
	ARGUMENTS: ---
	OUTPUT: Returns the subdivision limit.
	*/
	uint MyOctant::GetSubdivisionLimit(void)
	{
		return MyOctant::sDepthThreshold;
	}

	/*
	USAGE: Gets an octant by it's ID. If called from the root, it can return any.
	ARGUMENTS: a_uID -> ID for octant.
	OUTPUT: Returns an octant pointer.
	*/
	void MyOctant::GetOctant(uint a_uID, MyOctant* o_pOctant) 
	{
		// If -1, return nullptr.
		if (a_uID == -1 || a_uID >= this->GetOctantCount()) { o_pOctant = nullptr; }

		if (this->IsLeaf()) 
		{
			if (this->GetID() == a_uID) 
			{
				// Octant with matching ID has been found.
				o_pOctant = this;
			}			
			else 
			{
				o_pOctant = nullptr;
			}
		}
		else 
		{
			if (this->GetID() == a_uID) 
			{
				// If Octant matches, return it.
				o_pOctant = this;
			}
			else 
			{
				// If it doesn't match, search children for a match.
				MyOctant* result = nullptr;
				for (uint i = 0; i < m_lChildren.size(); i++)
				{
					this->m_lChildren[i]->GetOctant(a_uID, result);
				}
				o_pOctant = result;
			}
		}
	}

	/*
	USAGE: Gets this octant's ID.
	ARGUMENTS: ---
	OUTPUT: Returns the ID of the octant.
	*/
	uint MyOctant::GetID(void) const 
	{
		return this->m_uID;
	}

	/*
	USAGE: Gets this octant's depth.
	ARGUMENTS: ---
	OUTPUT: Returns the depth level of the octant.
	*/
	uint MyOctant::GetDepth(void) const
	{
		return this->m_uDepth;
	}

	/*
	USAGE: Returns the parent of this octant.
	ARGUMENTS: ---
	OUTPUT: MyOctant object (parent of this octant).
	*/
	MyOctant* MyOctant::GetParent(void) const 
	{
		return this->m_pParent;
	}

	/*
	USAGE: Returns the root.
	ARGUMENTS: ---
	OUTPUT: MyOctant object (root of this octant's octree).
	*/
	MyOctant& MyOctant::GetRoot(void) 
	{
		if(IsRoot())
		{
			// Return the root.
			return (*this);
		}
		else 
		{
			// If not the root, call the parent.
			return this->GetParent()->GetRoot();
		}
	}

	/*
	USAGE: Get the active color of the octant.
	ARGUMENTS: ---
	OUTPUT: Returns the color.
	*/
	vector3 MyOctant::GetActiveColor(void) const 
	{
		return this->m_v3Active;
	}

	/*
	USAGE: Get the inactive color of the octant.
	ARGUMENTS: ---
	OUTPUT: Returns the color.
	*/
	vector3 MyOctant::GetInactiveColor(void) const
	{
		return this->m_v3Inactive;
	}

	/*
	USAGE: Return the total number of octants in the world.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	uint MyOctant::GetOctantCount(void) const 
	{
		return sOctantCount;
	}

	/*
	USAGE: Return the number of entities within the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	uint MyOctant::GetEntityCount(void) const 
	{
		return this->m_lEntities.size();
	}

#pragma endregion

#pragma region // 	Service Methods
	
	/*
	USAGE: Send the octree to a string.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ToString(void) 
	{
		std::cout << "\nOctant [" << m_uID << "]: (Depth: " << m_uDepth << ", Children: " << m_lChildren.size() << " , Entities: " << m_lEntities.size() << ") Total Octants: [" << this->GetOctantCount() << "].";
	}

	/*
	USAGE: Adds a collection of entities to the tree.
	ARGUMENTS: uint a_lIndices -> Indices of the entities to be added.
	OUTPUT: ---
	*/
	void MyOctant::AddEntities(std::set<uint> a_lIndices) 
	{
		// Add entity for each element in the collection.
		std::set<uint>::iterator i;
		for (i = a_lIndices.begin(); i != a_lIndices.end(); i++)
		{
			this->AddEntity(*i);
		}
	}
	
	/*
	USAGE: Adds a single entity to the tree.
	ARGUMENTS: uint a_uIndex -> Index of the entity to be added.
	OUTPUT: ---
	*/
	void MyOctant::AddEntity(uint a_uIndex) 
	{
		// Validate index and enable status.
		if (a_uIndex == -1 || !this->IsEnabled()) { return; }
		else 
		{			
			m_pMeshManager->AddWireCubeToRenderList(glm::translate(this->m_pEntityManager->GetEntity(a_uIndex)->GetRigidBody()->GetCenterGlobal()) * glm::scale(vector3(4.0f)), C_CYAN);

			// If not colliding with this entity, so just return.
			if (!IsRoot() && !this->IsColliding(a_uIndex)) { return; }
			else {
				// Check for children.
				if (!this->IsLeaf())
				{
					// If the octant has children, attempt to add it to all its children.
					for (uint i = 0; i < 8; i++)
					{
						this->m_lChildren[i]->AddEntity(a_uIndex);
					}
				}
				else
				{
					std::cout << "\nChecking if subdivision needed: ";
					this->ToString();

					// If the octant has NO children, it is a leaf, so, check if subdivision is needed.
					if (this->GetDepth() >= sDepthThreshold) { this->m_lEntities.insert(a_uIndex); return; } // No subdivision necessary.
					else if (!this->ContainsMoreThan(sEntityThreshold)) { this->m_lEntities.insert(a_uIndex); return; } // No subdivision necessary.
					else if ((this->GetSize() / 2.0f) < sMinimumSize) { this->m_lEntities.insert(a_uIndex); return; } // The subdivision size is too small, so we will not subdivide.
					else {						
						// Show id.
						std::cout << "\nBefore subdivision: ";
						this->ToString();

						// Subdivision is necessary and possible.
						this->Subdivide();

						// Show id.
						std::cout << "\nAfter subdivision: ";
						this->ToString();
						
						// Add it to this octant again.
						this->AddEntity(a_uIndex);
					}
				}				
			}
		}		
	}

	/*
	USAGE: Display the MyOctant object, specified by index, including the objects underneath. (Used to display children).
	ARGUMENTS:
	- uint a_uIndex -> MyOctant to be displayed.
	- vector3 a_v3color = C_YELLOW -> Color to be displayed in.
	OUTPUT: ---
	*/
	void MyOctant::Display(uint a_uIndex, vector3 a_v3Color) 
	{
		// Check index validity.
		if (a_uIndex == -1) { return; }

		// Check if this index matches the input.
		if (this->GetID() == a_uIndex)
		{
			// Display using this color.
			this->Display(a_v3Color);
		}
		else if (!IsLeaf())
		{
			// Continue searching for children.
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				m_lChildren[i]->Display(a_uIndex, a_v3Color);
			}
		}		
	}

	/*
	USAGE: Displays the MyOctant volume in the color specified.
	ARGUMENTS: vector3 a_v3Color = C_YELLOW -> Color to be displayed in.
	OUTPUT: ---
	*/
	void MyOctant::Display(vector3 a_v3Color) 
	{
		// Display the octant using the input color.
		if (this->IsOBBVisible()) {
			// If set to be visible, then display.
			this->m_pMeshManager->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
			this->m_pMeshManager->AddWireSphereToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(1.5f)), C_PURPLE);
			this->m_pMeshManager->AddSphereToRenderList(glm::translate(m_v3Minimum) * glm::scale(vector3(1.5f)), C_GREEN);
			this->m_pMeshManager->AddSphereToRenderList(glm::translate(m_v3Maximum) * glm::scale(vector3(1.5f)), C_GREEN);
		}

		// If this has children.
		if (!this->IsLeaf()) 
		{
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				m_lChildren[i]->SetOBBVisbile(this->IsOBBVisible());
				m_lChildren[i]->Display(a_v3Color);
			}
		}
	}

	/*
	USAGE: Displays the non-empty leaf nodes in the octree.
	ARGUMENTS:
	- vector3 a_v3Color = C_YELLOW -> Color to display leaf octant in.
	OUTPUT: ---
	*/
	void MyOctant::DisplayLeafs(vector3 a_v3Color) 
	{
		if (!this->IsLeaf()) 
		{
			// Displays the leaves of the octree.
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				m_lChildren[i]->DisplayLeafs(a_v3Color);
			}
		}
		else 
		{
			if (!IsEmpty()) 
			{
				Display(a_v3Color);
			}
		}
	}

	/*
	USAGE: Clears the entity list for each node.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ClearEntityList(void) 
	{
		// If there are entities to clear, it will clear them.
		if (!IsEmpty()) 
		{
			this->m_lEntities.clear();
		}
	}

	/*
	USAGE: Deletes each child octant in the array. (Will cause recursive delete).
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ClearChildren(void) 
	{
		// Delete all of the children, if octant has children.
		if (!IsLeaf()) 
		{
			// Loop through collection.
			for (uint i = 0; i < m_lChildren.size(); i++) 
			{
				MyOctant* temp = m_lChildren[i];
				delete temp;
				m_lChildren[i] = nullptr;
			}

			// Clear it.
			m_lChildren = std::vector<MyOctant*>();
		}
	}

	/*
	USAGE: Allocates 8 smaller octants in the child pointers.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Subdivide(void) 
	{
		// If there are no children already.
		if (IsLeaf() && IsEnabled()) {
			
			// Calculate the centers for the eight cubes.
			vector3 center = this->GetCenterGlobal();			
			vector3 quarter = vector3(m_fSize / 4.0f);

			/*
				// The centers:
				// o = Origin (center of original octant).
				// a = o + half.x + half.y - half.z
				// b = o + half.x + half.y + half.z
				// c = o + half.x - half.y - half.z
				// d = o + half.x - half.y + half.z
				// a = o - half.x + half.y - half.z
				// b = o - half.x + half.y + half.z
				// c = o - half.x - half.y - half.z
				// d = o - half.x - half.y + half.z
			*/

			vector3 centers[8] = {
				center + vector3(quarter.x, quarter.y, -quarter.z),
				center + vector3(quarter.x, quarter.y, quarter.z),
				center + vector3(quarter.x, -quarter.y, -quarter.z),
				center + vector3(quarter.x, -quarter.y, quarter.z),
				center + vector3(-quarter.x, quarter.y, -quarter.z),
				center + vector3(-quarter.x, quarter.y, quarter.z),
				center + vector3(-quarter.x, -quarter.y, -quarter.z),
				center + vector3(-quarter.x, -quarter.y, quarter.z)
			}; // Collection of centers.		
			
			// Add the subdivisions as children.
			for (uint i = 0; i < 8; i++)
			{
				m_pMeshManager->AddWireSphereToRenderList(glm::translate(centers[i]) * glm::scale(vector3(2.5f)), C_CYAN);

				MyOctant* child = new MyOctant(this, centers[i], m_fSize);

				// Create a child, from one of the eight calculated centers, using a consistent half width vector.
				this->m_lChildren.push_back(child); // Make empty space.
				child->SetOBBVisbile(this->IsOBBVisible());
				child->SetEnabled(this->IsEnabled());

				// Attempt to add entities if any in current list.
				if (!this->IsEmpty())
				{
					std::set<uint>::iterator i;
					for(i = m_lEntities.begin(); i != m_lEntities.end(); i++)
					{
						child->AddEntity(*i);
					}
				}
			}

			// Remove the entity list from the octant that was subdivided.
			this->ClearEntityList();
		}
	}
	
	/*
	USAGE: Creates a tree using subdivisions, the max number of objects and levels.
	ARGUMENTS: uint a_uSubdivisionThreshold -> Sets the maximum level of the tree it is constructing.
	OUTPUT: ---
	*/
	void MyOctant::ConstructTree(uint a_uSubdivisionThreshold) 
	{
		// Set the subdivision threshold.
		this->SetMaximumDepth(a_uSubdivisionThreshold);
		
		// Construct tree.
		ConstructTree();
	}
	
	/*
	USAGE: Creates a tree using global settings.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ConstructTree() 
	{
		// In the root we want to build our new octree.
		if (IsRoot())
		{
			// Create the new bounds.
			std::set<uint> indices = std::set<uint>();
			std::vector<vector3> points = std::vector<vector3>();

			// Add each entity's minimum and maximum values.
			for (uint i = 0; i < m_pEntityManager->GetEntityCount(); i++)
			{
				std::cout << "\nEntity [" << i << "]";

				// Insert the index.
				indices.insert(i);

				// Get the entity's min and max.
				vector3 entityMinimum = m_pEntityManager->GetEntity(i)->GetRigidBody()->GetMaxGlobal() * 1.2f;
				vector3 entityMaximum = m_pEntityManager->GetEntity(i)->GetRigidBody()->GetMinGlobal() * 1.2f;

				// Add the entity's values to the points collection.
				//Back square
				points.push_back(entityMinimum);
				points.push_back(vector3(entityMaximum.x, entityMinimum.y, entityMinimum.z));
				points.push_back(vector3(entityMinimum.x, entityMaximum.y, entityMinimum.z));
				points.push_back(vector3(entityMaximum.x, entityMaximum.y, entityMinimum.z));

				//Front square
				points.push_back(vector3(entityMinimum.x, entityMinimum.y, entityMaximum.z));
				points.push_back(vector3(entityMaximum.x, entityMinimum.y, entityMaximum.z));
				points.push_back(vector3(entityMinimum.x, entityMaximum.y, entityMaximum.z));
				points.push_back(entityMaximum);
			}

			// Set minimum and maximum to first element.
			m_v3Maximum = m_v3Minimum = points[0];

			//get the new max and min for the global box
			for (uint i = 1; i < points.size(); ++i)
			{
				// std::cout << "\nPoint [" << i << "]: " << glm::to_string(points[i]);

				if (m_v3Maximum.x < points[i].x) m_v3Maximum.x = points[i].x;
				else if (m_v3Minimum.x > points[i].x) m_v3Minimum.x = points[i].x;

				if (m_v3Maximum.y < points[i].y) m_v3Maximum.y = points[i].y;
				else if (m_v3Minimum.y > points[i].y) m_v3Minimum.y = points[i].y;

				if (m_v3Maximum.z < points[i].z) m_v3Maximum.z = points[i].z;
				else if (m_v3Minimum.z > points[i].z) m_v3Minimum.z = points[i].z;
			}

			// Get the center.
			// this->m_v3Center = FindCenter(points);
			this->m_v3Center = (m_v3Maximum + m_v3Minimum) / 2.0f;
			std::cout << "\nCenter: " << glm::to_string(m_v3Center);
			std::cout << "\nMaximum: " << glm::to_string(m_v3Maximum);
			std::cout << "\nMinimum: " << glm::to_string(m_v3Minimum);

			// Max distance between planes.
			float maxDistance = 0.0f;
			float distances[4];
			distances[0] = glm::length(m_v3Maximum - m_v3Minimum);
			distances[1] = m_v3Maximum.x - m_v3Minimum.x;
			distances[2] = m_v3Maximum.y - m_v3Minimum.y;
			distances[3] = m_v3Maximum.z - m_v3Minimum.z;

			maxDistance = distances[0];
			for(uint i = 1; i < 4; i++) 
			{
				if (maxDistance < distances[i]) { maxDistance = distances[i]; }
			}
			
			// Resize root.
			//we calculate the distance between min and max vectors
			this->SetSize(maxDistance);
			std::cout << "\nSize: " << glm::to_string(maxDistance) << ", (" << maxDistance << ")";

			this->AddEntities(indices);
		}

		this->AssignID();
	}

	/*
	USAGE: Traverse the tree to the leafs and sets the objects in them to the index.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::AssignID(void) 
	{
		// Assumptions: Our entity set has already been created.
		// Goal: Set and confirm entity Dimension as being the octant's ID.		 
		if (!IsLeaf())
		{
			// So I found out that the verb is to 'recurse' and not to 'recur', in the context of computer programming.
			// Recurse on the children if a leaf hasn't been reached.
			for (uint i = 0; i < m_lChildren.size(); i++)
			{
				this->m_lChildren[i]->AssignID();
			}
		}
		else
		{
			// In the else case, this is a leaf (ie. has no children), so we check to see if there are any entities to assign.
			if (!IsEmpty())
			{
				// Loop through the set of entities, using an iterator.
				std::set<uint>::iterator i; // Iterator.
				for (i = m_lEntities.begin(); i != m_lEntities.end(); i++)
				{
					// For every entity ID in the set, we'll want to assign a dimension via the manager.
					this->m_pEntityManager->AddDimension(*i, m_uID);
				}
			}

			// Exit condition: The octant has no child octants (is a leaf).
			// We do not perform recursion again.
		}
	}

	/*
	USAGE: Returns a vector that represents the center.
	ARGUMENTS: std::vector<vector3> points -> Contains a set of vector3 points.
	OUTPUT: Returns the center vector.
	*/
	vector3 MyOctant::FindCenter(std::vector<vector3> points)
	{
		// Check size of the set.
		if (points.empty()) { return ZERO_V3; }

		// Get the center of all the vectors.
		vector3 center = points[0];

		// Loop through for sum.
		for (uint i = 0; i < points.size(); i++) 
		{
			center += points[i];
		}

		// Average the vector.
		float size = points.size();
		center = vector3(center.x / size, center.y / size, center.z / size);

		// Return the resulting vector.
		return center;
	}

	/*
	USAGE: Returns a vector that represents the maximum extent.
	ARGUMENTS: std::vector<vector3> points -> Contains a set of vector3 points.
	OUTPUT: Returns the maximum vector.
	*/
	vector3 MyOctant::FindMaximum(std::vector<vector3> points)
	{
		// Check size of the set.
		if (points.empty()) { return ZERO_V3; }

		// Create iterator for the set.
		std::vector<vector3>::iterator i = points.begin();

		// Create storage for result.
		vector3 max = *i;

		// Loop over the points and find the maximum.
		for (i = points.begin(); i != points.end(); i++)
		{
			if (i != points.begin()) {
				vector3 value = *i;
				if (max.x < value.x) { max.x = value.x; }
				if (max.y < value.y) { max.y = value.y; }
				if (max.z < value.z) { max.z = value.z; }
			}
		}

		// Return the resulting vector.
		return max;
	}

	/*
	USAGE: Returns a vector that represents the minimum extent.
	ARGUMENTS: std::vector<vector3> points -> Contains a set of vector3 points.
	OUTPUT: Returns the minimum vector.
	*/
	vector3 MyOctant::FindMinimum(std::vector<vector3> points)
	{
		// Check size of the set.
		if (points.empty()) { return ZERO_V3; }

		// Create iterator for the set.
		std::vector<vector3>::iterator i = points.begin();

		// Create storage for result.
		vector3 min = *i;

		// Loop over the points and find the minimum.
		for (i = points.begin(); i != points.end(); i++)
		{
			if (i != points.begin()) {
				vector3 value = *i;
				if (min.x > value.x) { min.x = value.x; }
				if (min.y > value.y) { min.y = value.y; }
				if (min.z > value.z) { min.z = value.z; }
			}
		}

		// Return the resulting vector.
		return min;
	}

#pragma endregion

#pragma endregion

} // namespace Simplex.