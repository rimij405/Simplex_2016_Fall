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
			-> [Validate as a vector3 where every dimension is positive and non-zezo.]
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

#pragma region //	Static Members

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

#pragma region //	Constructor(s)

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
		// Validate the input parameters.
		if (a_uSubdivisionLimit <= 0 || a_uSubdivisionLimit == -1) { a_uSubdivisionLimit = DMAX_DEPTH; }
		if (a_uIdealEntityCount <= 0 || a_uIdealEntityCount == -1) { a_uIdealEntityCount = DMAX_CAPACITY; }
		if (a_uMinimumSize <= 0 || a_uMinimumSize == -1) { a_uMinimumSize = DMIN_SIZE; }
		
		// Assign static members.
		MyOctant::sOctantCount++;
		MyOctant::sDepthThreshold = a_uSubdivisionLimit;
		MyOctant::sEntityThreshold = a_uIdealEntityCount;
		MyOctant::sMinimumSize = a_uMinimumSize;

		// Initialize the octant.
		this->Init();

		// TODO.
		// Add additional code for reading in entities.
	}

	/*
	USAGE: Constructs an octant at a center point using uniform sides.
	ARGUMENTS:
	- vector3 a_v3Center -> Center of the octant in global space.
	- float a_fSize -> Size of each of the sides of the octant volume.
	OUTPUT: class object.
	*/
	MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) 
	{
		// Validate the input parameters.
		if (a_fSize <= 0) { a_fSize = MyOctant::sMinimumSize; }
		 
		// Assign static members.
		MyOctant::sOctantCount++;

		// Initialize the octant.
		this->Init();

		// Assign input values. (Overwrites initialized values).
		this->m_v3Center = a_v3Center; // Assign the global space center.
		this->m_v3HalfWidths = vector3(a_fSize / 2); // Assign half width as same for each of the three dimensions.
	}

	/*
	USAGE: Constructs an octant at a center point using varying sides.
	ARGUMENTS:
	- vector3 a_v3Center -> Center of the octant in global space.
	- float a_v3HalfWidths -> Half size of each of the sides of the octant volume.
	OUTPUT: class object.
	*/
	MyOctant::MyOctant(vector3 a_v3Center, vector3 a_v3HalfWidths)
	{
		// Assign static members.
		MyOctant::sOctantCount++;

		// Initialize the octant.
		this->Init();

		// Assign input values. (Overwrite initialized values).
		this->m_v3Center = a_v3Center;
		this->m_v3HalfWidths = a_v3HalfWidths;
	}

#pragma endregion

#pragma region //	Rule of Three

	/*
	USAGE: Copies the input octant at face value, retaining the same values overall.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object instance.
	*/
	MyOctant::MyOctant(MyOctant const& other)
	{
		// Instances.
		this->m_pMeshManager = other.m_pMeshManager;
		this->m_pEntityManager = other.m_pEntityManager;

		// Flags.
		this->m_bEnabled = other.m_bEnabled;
		this->m_bVisibleOBB = other.m_bVisibleOBB;
		
		// Values.
		this->m_uID = 0;
		this->m_uDepth = 0;
		this->m_uChildCount = 0;

		// Colors.
		this->m_v3Active = other.m_v3Active;
		this->m_v3Inactive = other.m_v3Inactive;

		// Vectors.
		this->m_v3HalfWidths = other.m_v3HalfWidths;
		this->m_v3Center = other.m_v3Center;
		this->m_v3Maximum = other.m_v3Maximum;
		this->m_v3Minimum = other.m_v3Minimum;
		
		// Collections.
		this->m_lEntityQueue = other.m_lEntityQueue;
		this->m_lEntities = other.m_lEntities;
		this->m_lActiveChildren = other.m_lActiveChildren;

		// Pointers
		this->m_pParent = other.m_pParent;
		this->m_pRoot = other.m_pRoot;

		// Shallow copy array.
		this->m_pChild[8] = new MyOctant[8];
		if (!other.IsLeaf()) {
			// If the other has children, copy them over.
			for (uint i = 0; i < 8; i++)
			{
				this->m_pChild[i] = other.m_pChild[i];
			}
		}
	}

	/*
	USAGE: Copies the input octant at face value, retaining the same values overall.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: ---
	*/
	MyOctant& MyOctant::operator=(MyOctant const& other)
	{
		// Check to make sure this isn't the same octant.
		if (this != &other) {

			// Clear this object.
			this->Release();

			// Initialize this object again.
			this->Init();

			// Use the copy constructor.
			MyOctant temp(other);

			// Swap with temp object.
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
		// Release memory.
		Release();
	}

#pragma endregion

#pragma region //	Rule of Five

	/*
	USAGE: Move constructor.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object instance.
	*/
	MyOctant::MyOctant(MyOctant&& other) 
	{				
		// Use the move assignment operator.
		(*this) = MyOctant(other);
	}

	/*
	USAGE: Move assignment operator.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: ---
	*/
	MyOctant& MyOctant::operator=(MyOctant&& other)
	{
		// Check if same.
		if (this != &other) 
		{
			// Release the data.
			this->Release();

			// Initialize once again.
			this->Init();
			
			// Swap values.
			this->Swap(other);

			// Release the other.
			other.Release();		
		}
		
		// Return reference.
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
		std::swap(m_uChildCount, other.m_uChildCount);

		std::swap(m_bEnabled, other.m_bEnabled);
		std::swap(m_bVisibleOBB, other.m_bVisibleOBB);
		
		// Swap vector3s.
		std::swap(m_v3Center, other.m_v3Center);
		std::swap(m_v3Maximum, other.m_v3Maximum);
		std::swap(m_v3Minimum, other.m_v3Minimum);
		std::swap(m_v3HalfWidths, other.m_v3HalfWidths);

		std::swap(m_v3Active, other.m_v3Active);
		std::swap(m_v3Inactive, other.m_v3Inactive);
				
		// Swap collections.
		std::swap(m_lEntities, other.m_lEntities);
		std::swap(m_lEntityQueue, other.m_lEntityQueue);
		std::swap(m_lActiveChildren, other.m_lActiveChildren);
		std::swap(m_pChild, other.m_pChild);

		// Swap memory.
		std::swap(m_pMeshManager, other.m_pMeshManager);
		std::swap(m_pEntityManager, other.m_pEntityManager);
		std::swap(m_pParent, other.m_pParent);
		std::swap(m_pRoot, other.m_pRoot);
	}

#pragma endregion

#pragma region //	Initialization/Memory Management Methods

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

		// Clear the entity list of this octant and clear its children.
		this->ClearEntityList();
		this->KillBranches();

		// Close flags.
		this->m_bEnabled = false;
		this->m_bVisibleOBB = false;

		// Overwrite values.
		m_uID = 0;
		m_uDepth = 0;
		m_v3HalfWidths = ZERO_V3;
		m_v3Center = ZERO_V3;
		m_v3Minimum = ZERO_V3;
		m_v3Maximum = ZERO_V3;

		// Release pointers.
		m_pMeshManager = nullptr; // DO NOT DELETE.
		m_pEntityManager = nullptr; // DO NOT DELETE.
		m_pParent = nullptr; // DO NOT DELETE.
		m_pRoot = nullptr; // DO NOT DELETE.

		// Collections.
		this->m_lEntityQueue.clear();
		this->m_lActiveChildren.clear();
	}

	/*
	USAGE: Initializes the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Init(void)
	{
		// Initialize the singletons.
		m_pMeshManager = MeshManager::GetInstance();
		m_pEntityManager = MyEntityManager::GetInstance();

		// Initialize fields.
		InitVariables();
	}

	/*
	USAGE: Allocates member field data and initializes the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::InitVariables(void)
	{		
		// // Initialize the fields to their default values.

		// Flags.
		m_bEnabled = true;
		m_bVisibleOBB = false;

		// Unsigned int values.
		m_uID = 0;
		m_uDepth = 0;
		m_uChildCount = 0;

		// Colors.
		m_v3Active = C_YELLOW;
		m_v3Inactive = C_WHITE;

		// Vector3s.
		m_v3HalfWidths = ZERO_V3;
		m_v3Center = ZERO_V3;
		m_v3Minimum = ZERO_V3;
		m_v3Maximum = ZERO_V3;

		// Collections.
		m_lEntityQueue = std::deque<uint>();
		m_lEntities = std::set<uint>();
		m_lActiveChildren = std::vector<MyOctant*>();

		// Initializes memory managed members.
		m_pParent = nullptr; // Will be set after init is called.
		m_pRoot = nullptr; // Reference to the root octant. (Will also be set after init is called).
						   
		// Creates a pointer array that will hold 8 octants.
		m_pChild[8] = { }; // Sets it to null.
		for (uint i = 0; i < 8; i++) 
		{
			// Fill array with nullptrs.
			m_pChild[i] = nullptr;
		}

	}

	/*
	USAGE: Creates the list of all leaves that contain objects.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ConstructList(void)
	{
		// Check if this is a leaf.
		if (IsLeaf()) 
		{
			// Check if this has entities.
			if (!IsEmpty()) 
			{
				// If this leaf has entities, it is active. (EXIT CONDITION).
				this->m_pRoot->m_lActiveChildren.push_back(this);
			}			
		} 
		else
		{
			// If this is the root, clear the list first.
			if (IsRoot()) 
			{
				// We don't want to delete the pointers; only remove our collection of them.
				this->m_lActiveChildren.clear();
			}

			// Call construct list on all children.
			for (uint i = 0; i < 8; i++) 
			{
				// If child isn't a nullptr, call the construct list function.
				MyOctant* child = m_pChild[i];
				if (child) { child->ConstructList(); }
			}
		}
	}

#pragma endregion

#pragma region //	Flag Methods

	/*
	USAGE: Asking if there is a collision with an Entity as supplied by the input index value.
	ARGUMENTS: uint a_uRBIndex -> Index of the Entity in the EntityManager.
	OUTPUT: Check of the collision.
	*/
	bool MyOctant::IsColliding(uint a_uRBIndex)
	{
		// Check if the index exists.
		if (a_uRBIndex < 0 || a_uRBIndex == -1) { return false; }

		// Check if the entity exists.
		MyEntity* entity = m_pEntityManager->GetEntity(a_uRBIndex);
		if (entity == nullptr) { return false; }

		// Check if rigidbody exists.
		MyRigidBody* entityRB = m_pEntityManager->GetRigidBody();
		if (entityRB == nullptr) { return false; }

		// Create the octant's rigidbody.
		std::vector<vector3> points = { this->m_v3Maximum, this->m_v3Minimum };
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
		// If parent is null, then this is the root.
		return (m_pParent == nullptr);
	}

	/*
	USAGE: Asks if this octant has no children.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsLeaf(void) const
	{
		// If the child count is equal to zero, then it is a leaf.
		return (m_uChildCount == 0);
	}

	/*
	USAGE: Checks to see if this octant has more than the input number of entities available.
	ARGUMENTS: uint a_uEntities -> Number of entities to check for.
	OUTPUT: Returns check.
	*/
	bool MyOctant::ContainsMoreThan(uint a_uEntities)
	{
		// Ensure our input value is greater than or equal to 0.
		if (a_uEntities < 0 || a_uEntities == -1) { return false; }
		else 
		{
			// Return a true, if the number of entities in the octant are greater than the input value.
			return (a_uEntities < GetEntityCount());
		}

		// When uint is a -1, then return false.
		return false;
	}

	/*
	USAGE: Asks if this octant has no entities.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsEmpty(void) const
	{
		return (this->m_lEntities.empty());
	}

	/*
	Usage: Gets visibility of oriented bounding box.
	Arguments: ---
	Output: Returns check.
	*/
	bool MyOctant::IsOBBVisible(void) const
	{
		return (this->m_bVisibleOBB);
	}

	/*
	Usage: Gets enabled flag of octant.
	Arguments: ---
	Output: Returns check.
	*/
	bool MyOctant::IsEnabled(void) const
	{
		return (this->m_bEnabled);
	}

#pragma endregion

#pragma region //	Mutator Methods

	/*
	Usage: Sets enabled flag of octant.
	Arguments: bool a_bVisibleOBB -> Input value to set OBB visibility flag to.
	Output: ---
	*/
	void MyOctant::SetOBBVisbile(bool a_bVisibleOBB)
	{
		this->m_bVisibleOBB = a_bVisibleOBB;
	}

	/*
	Usage: Sets enabled flag of octant.
	Arguments: bool a_bEnabled -> Input value to set enable flag to.
	Output: ---
	*/
	void MyOctant::SetEnabled(bool a_bEnabled)
	{
		this->m_bEnabled = a_bEnabled;
	}

	/*
	USAGE: Sets this octant's size. (via half-width property).
	ARGUMENTS: a_fSize -> Length of sides of the octant.
	OUTPUT: ---
	*/
	void MyOctant::SetSize(float a_fSize)
	{
		if (a_fSize < sMinimumSize) { a_fSize = sMinimumSize; }
		float a_fHalfSize = a_fSize / 2;
		this->m_v3HalfWidths = vector3(a_fHalfSize);
	}

	/*
	USAGE: Sets this octant's half widths.
	ARGUMENTS: a_v3HalfWidths -> Value to set half widths to.
	OUTPUT: ---
	*/
	void MyOctant::SetHalfWidths(vector3 a_v3HalfWidths)
	{
		// Validate the input parameters.
		float min = sMinimumSize / 2.0f;
		if (a_v3HalfWidths == ZERO_V3) { a_v3HalfWidths = vector3(min); }
		else {
			if (a_v3HalfWidths.x < min) { a_v3HalfWidths.x = min; }
			if (a_v3HalfWidths.y < min) { a_v3HalfWidths.y = min; }
			if (a_v3HalfWidths.z < min) { a_v3HalfWidths.z = min; }
		}
		this->m_v3HalfWidths = a_v3HalfWidths;
	}

	/*
	USAGE: Set the center of the octant in global space.
	ARGUMENTS: a_v3Center -> Value to set center to.
	OUTPUT: ---
	*/
	void MyOctant::GetCenterGlobal(vector3 a_v3Center)
	{
		float min = FLT_EPSILON;
		if (a_v3Center == ZERO_V3) { a_v3Center = vector3(min); }
		else {
			if (a_v3Center.x < min) { a_v3Center.x = min; }
			if (a_v3Center.y < min) { a_v3Center.y = min; }
			if (a_v3Center.z < min) { a_v3Center.z = min; }
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
		float min = FLT_EPSILON;
		if (a_v3Maximum == ZERO_V3) { a_v3Maximum = vector3(min); }
		else {
			if (a_v3Maximum.x < min) { a_v3Maximum.x = min; }
			if (a_v3Maximum.y < min) { a_v3Maximum.y = min; }
			if (a_v3Maximum.z < min) { a_v3Maximum.z = min; }
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
		float min = FLT_EPSILON;
		if (a_v3Minimum == ZERO_V3) { a_v3Minimum = vector3(min); }
		else {
			if (a_v3Minimum.x < min) { a_v3Minimum.x = min; }
			if (a_v3Minimum.y < min) { a_v3Minimum.y = min; }
			if (a_v3Minimum.z < min) { a_v3Minimum.z = min; }
		}
		this->m_v3Minimum = a_v3Minimum;
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
		// Validate input parameters.
		if (a_uChild < 0 || a_uChild == -1 || a_pChild == nullptr) { return; }
		
		// If it has children, overwrite the existing space.
		if (!IsLeaf()) 
		{
			m_pChild[a_uChild] = a_pChild;
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
	}

	/*
	USAGE: Set the inactive color of the octant.
	ARGUMENTS: vector3 a_v3Color -> Input color value to assign.
	OUTPUT: ---
	*/
	void MyOctant::SetInactiveColor(vector3 a_v3Color)
	{
		this->m_v3Inactive = a_v3Color;
	}

#pragma endregion

#pragma region //	Accessor Methods

	/*
	USAGE: Gets this octant's size.
	ARGUMENTS: ---
	OUTPUT: Returns the size of the octant as a float.
	*/
	float MyOctant::GetSize(void) const
	{
		// This will return the average of the full widths in all dimensions.
		return (2 * (m_v3HalfWidths.x + m_v3HalfWidths.y + m_v3HalfWidths.z)) / 3;
	}

	/*
	USAGE: Gets this octant's half widths.
	ARGUMENTS: ---
	OUTPUT: Returns the half widths of the octant as a vector3.
	*/
	vector3 MyOctant::GetHalfWidths(void) const
	{
		// This will return a vector3 containing the Octant's halfwidths.
		return m_v3HalfWidths;
	}

	/*
	USAGE: Get the center of the octant in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetCenterGlobal(void) const
	{
		// This will return a vector3 containing the octant's center in global space.
		return m_v3Center;
	}

	/*
	USAGE: Get the maximum vector in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetMaxGlobal(void) const
	{
		// This will return a vector3 containing the octant's maximum vector, in global space.
		return m_v3Maximum;
	}

	/*
	USAGE: Get the minimum vector in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetMinGlobal(void) const
	{
		// This will return a vector3 containing the octant's minimum vector, in global space.
		return m_v3Minimum;
	}

	/*
	USAGE: Returns the child at the specified index (if possible).
	ARGUMENTS: uint a_uChild -> index of the child [0, 7].
	OUTPUT: MyOctant object (child at index).
	*/
	MyOctant* MyOctant::GetChild(uint a_uChild) const
	{
		// Check if index value is within range.
		if (a_uChild < 0 || a_uChild == -1) { return nullptr; }
		else if (a_uChild >= 0 && a_uChild <= 7) {
			// Check if there are children.
			if (!IsLeaf())
			{
				// If this octant has children (aka, is not a leaf).
				return m_pChild[a_uChild];
			}
			return nullptr;
		}
	}

	/*
	USAGE: Returns the parent of this octant.
	ARGUMENTS: ---
	OUTPUT: MyOctant object (parent of this octant).
	*/
	MyOctant* MyOctant::GetParent(void) const
	{
		// Check if this is the root.
		if (!IsRoot()) 
		{
			return m_pParent;
		}
		return nullptr;
	}

	/*
	USAGE: Return the total number of octants in the world.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	uint MyOctant::GetOctantCount(void) const
	{
		return MyOctant::sOctantCount;
	}

	/*
	USAGE: Return the number of entities within the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	uint MyOctant::GetEntityCount(void) const
	{
		return m_lEntities.size();
	}

	/*
	USAGE: Get the active color of the octant.
	ARGUMENTS: ---
	OUTPUT: Returns the color.
	*/
	vector3 MyOctant::GetActiveColor(void) const
	{
		return m_v3Active;
	}

	/*
	USAGE: Get the inactive color of the octant.
	ARGUMENTS: ---
	OUTPUT: Returns the color.
	*/
	vector3 MyOctant::GetInactiveColor(void) const
	{
		return m_v3Inactive;
	}

#pragma endregion

#pragma region //	Service Methods

	/*
	USAGE: Adds a collection of entities to the update queue.
	ARGUMENTS: uint a_lIndices -> Indices of the entities to be added.
	OUTPUT: ---
	*/
	void MyOctant::AddEntities(std::vector<uint> a_lIndices)
	{
		// Add entity for each element in the collection.
		for (uint i = 0; i < a_lIndices.size(); i++) 
		{
			this->AddEntity(a_lIndices[i]);
		}
	}

	/*
	USAGE: Adds a single entity to the update queue.
	ARGUMENTS: uint a_uIndex -> Index of the entity to be added.
	OUTPUT: ---
	*/
	void MyOctant::AddEntity(uint a_uIndex) 
	{
		// Confirms index is potentially valid.
		if (a_uIndex < 0 || a_uIndex == -1) { return; }

		// Adds index to queue.
		this->m_lEntityQueue.push_front(a_uIndex);
	}

	/*
	USAGE: Adds (or updates) all entities present in the queue.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::AddQueue(void)
	{
		// Check if a queue even exists.
		if (m_lEntityQueue.empty()) { return; }

		// Check if there will be a process limit on the amount of entities to add in one go.
		bool noLimit = (sMaximumProcesses == -1);

		// Get values for loop management.
		uint queueIndex = (m_lEntityQueue.size() - 1); // We know there is at least one item, since it isn't empty.
		uint processCount = 0;

		// This will loop through the queue on a couple of important conditions:
		// 1. While the queueIndex is not at zero.
		// 2a. While there is no limit on the process count.
		// 2b. OR, while there is a limit AND the process count is less than or equal to the maximum processes.
		while ((queueIndex >= 0) && (noLimit || processCount <= sMaximumProcesses))
		{
			// Get the entity ID.
			uint entityID = m_lEntityQueue[queueIndex];
			
			// Check if the value is valid.
			if(entityID != -1) 
			{
				// We use set because it will automatically handle duplicates (meaning it won't add them in those cases).
				m_lEntities.insert(entityID);				
			}
			
			// We consider the entity ID processed, even if it happens to be invalid.
			// At the end of every loop, update exit condition values.
			m_lEntityQueue.pop_back(); // Remove the entity reference from the end of the queue.
			queueIndex = (m_lEntityQueue.size() - 1); // We know there is at least one item, since it isn't empty.
			processCount++; // Increment the process count by one.
		}

		// If there are still entities remaining in the queue, we'll simply deal with them on the next method call.
		// Enabling this variable early out means we can batch process entities in groups at a time.
		// When we enact a process limit, however, it means there may be some frames where collision detection doesn't take place.
		// This means we'll want to add entities in a priority fashion.
	}

	// TODO : Update Tree

	/*
	USAGE: Display the MyOctant object, specified by index, including the objects underneath. (Used to display children).
	ARGUMENTS:
	- uint a_uIndex -> MyOctant to be displayed.
	- vector3 a_v3Color = C_YELLOW -> Color to be displayed in.
	OUTPUT: ---
	*/
	void MyOctant::Display(uint a_uIndex, vector3 a_v3Color)
	{
		// Check index validity.
		if (a_uIndex < 0 || a_uIndex == -1) { return; }

		// Check if this index matches the input.
		if (m_uID == a_uIndex) 
		{
			// Display using this color.
			this->Display(a_v3Color);
		}
		else if (IsLeaf()) 
		{
			// If it has no children, and the index doesn't match, return.
			return;
		}
		else
		{
			// Continue searching for children.
			for (uint i = 0; i < 8; i++) 
			{
				m_pChild[i]->Display(a_uIndex, a_v3Color);
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
		this->m_pMeshManager->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(m_v3HalfWidths * 2.0f), a_v3Color);

		// Run this display on all children, if any.
		if (IsLeaf())
		{
			// If it is a leaf, then there are no children.
			return;
		} 
		else
		{
			// If not a leaf, it will continue to perform recursion.
			for (uint i = 0; i < 8; i++) 
			{
				// Display child volumes underneath this one.
				m_pChild[i]->Display(a_v3Color);
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
		// Check if it's a leaf.
		if (IsLeaf()) 
		{
			// If it has no children (ie. is a leaf), check if it has entities.
			if (!IsEmpty()) 
			{
				// Display this octant.
				this->Display(a_v3Color);
			}
		}
		else 
		{
			// If it has children (ie. not a leaf), call this on its children.
			for (uint i = 0; i < 8; i++)
			{
				// Display leaves in this branch.
				m_pChild[i]->DisplayLeafs(a_v3Color);
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
		if (!IsEmpty()) 
		{
			// Clear and reinitialize the entity list.
			m_lEntities.clear();
			m_lEntities = std::set<uint>();
		}

		if (!IsLeaf()) 
		{
			// If there are children, clear the entity list for each child octant.
			for (uint i = 0; i < 8; i++) 
			{
				// Clear entity list in child.
				m_pChild[i]->ClearEntityList();
			}
		}
	}

	/*
	USAGE: Recursively deletes all children (and its children). (Array must be re-initialized after this).
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ClearChildren(void) 
	{
		if (!IsLeaf())
		{
			// If there are children, delete them.
			for (uint i = 0; i < 8; i++)
			{
				// Delete each of the elements in the array.
				m_pChild[i]->ClearChildren();
				m_pChild[i]->m_uChildCount = 0;
				delete m_pChild[i];
				m_pChild[i] = nullptr; // We need to do this so we can reuse the array.
			}
		}
	}

	/*
	USAGE: Allocates 8 smaller octants in the child pointers.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Subdivide(void)
	{
		// TODO.
	}

	/*
	USAGE: Recursively deletes all children (and children of children).
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::KillBranches(void)
	{
		this->ClearChildren();
	}

	/*
	USAGE: Creates a tree using subdivisions, the max number of objects and levels.
	ARGUMENTS: uint a_uSubdivisionThreshold = 3 -> Sets the maximum level of the tree it is constructing.
	OUTPUT: ---
	*/
	void MyOctant::ConstructTree(uint a_uSubdivisionThreshold)
	{
		// TODO.
	}

	/*
	USAGE: Traverse the tree to the leafs and sets the objects in them to the index.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::AssignID(void)
	{
		// Goal: Set and confirm entity Dimension as being the octant's ID.		 
		if (!IsLeaf())
		{
			// So I found out that the verb is to 'recurse' and not to 'recur', in the context of computer programming.
			// Recurse on the children if a leaf hasn't been reached.
			for (uint i = 0; i < 8; i++) 
			{
				this->m_pChild[i]->AssignID();
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
						
				}				
			}

			// Exit condition: The octant has no child octants (is a leaf).
			// We do not perform recursion again.
		}
	}

#pragma endregion
	
} // namespace Simplex