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

	/*
	Usage: Sets the minimum size of all octants.
	Arguments: uint a_uMinimumSize -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMinimumSize(uint a_uMinimumSize) 
	{
		if (a_uMinimumSize <= 0 || a_uMinimumSize == -1) { a_uMinimumSize = DMIN_SIZE; }
		MyOctant::m_uMinimumSize = a_uMinimumSize;
	}

	/*
	Usage: Sets the maximum subdivision level an octant can have.
	Arguments: uint a_uDepthThreshold -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMaximumDepth(uint a_uDepthThreshold) 
	{
		if (a_uDepthThreshold <= 0 || a_uDepthThreshold == -1) { a_uDepthThreshold = DMAX_DEPTH; }
		MyOctant::m_uDepthThreshold = a_uDepthThreshold;
	}

	/*
	Usage: Sets the ideal entity count.
	Arguments: uint m_uEntityThreshold -> Input value to set.
	Output: ---
	*/
	void MyOctant::SetMaximumCapacity(uint a_uEntityThreshold)
	{
		if (a_uEntityThreshold <= 0 || a_uEntityThreshold == -1) { a_uEntityThreshold = DMAX_CAPACITY; }
		MyOctant::m_uEntityThreshold = a_uEntityThreshold;
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
		MyOctant::m_uOctantCount++;
		MyOctant::m_uDepthThreshold = a_uSubdivisionLimit;
		MyOctant::m_uEntityThreshold = a_uIdealEntityCount;
		MyOctant::m_uMinimumSize = a_uMinimumSize;

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
		if (a_fSize <= 0) { a_fSize = MyOctant::m_uMinimumSize; }
		 
		// Assign static members.
		MyOctant::m_uOctantCount++;

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
		// Validate the input parameters.
		float min = m_uMinimumSize / 2.0f;
		if (a_v3HalfWidths == ZERO_V3) { a_v3HalfWidths = vector3(min); }
		else {
			if (a_v3HalfWidths.x == 0.0f) { a_v3HalfWidths.x = min; }
			if (a_v3HalfWidths.y == 0.0f) { a_v3HalfWidths.y = min; }
			if (a_v3HalfWidths.z == 0.0f) { a_v3HalfWidths.z = min; }
		}

		// Assign static members.
		MyOctant::m_uOctantCount++;

		// Initialize the octant.
		this->Init();

		// Assign input values. (Overwrite initialized values).
		this->m_v3Center = a_v3Center;
		this->m_v3HalfWidths = a_v3HalfWidths;
	}

#pragma endregion

#pragma region //	Rule of Three

	/*
	USAGE: Performs a deep copy of an existing octant. Keeps the ID.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object instance.
	*/
	MyOctant::MyOctant(MyOctant const& other)
	{
		// Assign static members.
		MyOctant::m_uOctantCount++; // When using a copy constructor, we want to increase our octant count.

		// Initializations.
		m_lActiveChildren = std::vector<MyOctant*>(); // This must be rebuilt using the copied children, later on.

		// Value copy operations.
		m_uID = other.m_uID;
		m_uDepth = other.m_uDepth;
		m_uChildCount = other.m_uChildCount;

		m_v3HalfWidths = other.m_v3HalfWidths;
		m_v3Center = other.m_v3Center;
		m_v3Minimum = other.m_v3Minimum;
		m_v3Maximum = other.m_v3Maximum;

		// Shallow copy references.		
		m_pMeshManager = other.m_pMeshManager;
		m_pEntityManager = other.m_pEntityManager;
				
#pragma region //	 Deep copy references.

		if (!other.IsEmpty()) 
		{
			// If there are no entities inside the source.
			m_lEntities = other.m_lEntities; // std::vector does a deep copy of its contents.
		}
		else 
		{
			// If there are entities inside the source.
			m_lEntities = std::vector<uint>();
		}

		if (!other.IsRoot()) 
		{
			// If the source is not a root.
			m_pParent = new MyOctant(*other.m_pParent);
			m_pRoot = new MyOctant(*other.m_pRoot);
		}
		else 
		{
			// If the source is root.
			m_pParent = nullptr;
			m_pRoot = nullptr;
		}
		
		if (!other.IsLeaf()) 
		{
			// If the source has children.
			m_pChild[8] = new MyOctant[8];
			for (uint i = 0; i < 8; i++) 
			{
				// Fill array with other's.
				m_pChild[i] = new MyOctant(*(other.m_pChild[i]));
			}
		}
		else 
		{
			// If the source doesn't have children.
			m_pChild[8] = new MyOctant[8];
			for (uint i = 0; i < 8; i++)
			{
				// Fill array with nullptrs.
				m_pChild[i] = nullptr;
			}
		}

#pragma endregion

	}

	/*
	USAGE: Performs a deep copy of an existing octant. Keeps the ID.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: ---
	*/
	MyOctant& MyOctant::operator=(MyOctant const& other)
	{
		// Check to make sure this isn't the same octant.
		if (this != &other) {

			// Free this data.
			this->Release();

			// Assign static members.
			MyOctant::m_uOctantCount++; // When using a copy assignment, we want to increase our octant count.

			// Initializations.
			m_lActiveChildren = std::vector<MyOctant*>(); // This must be rebuilt using the copied children, later on.

			// Value copy operations.
			m_uID = other.m_uID;
			m_uDepth = other.m_uDepth;
			m_uChildCount = other.m_uChildCount;

			m_v3HalfWidths = other.m_v3HalfWidths;
			m_v3Center = other.m_v3Center;
			m_v3Minimum = other.m_v3Minimum;
			m_v3Maximum = other.m_v3Maximum;

			// Shallow copy references.		
			m_pMeshManager = other.m_pMeshManager;
			m_pEntityManager = other.m_pEntityManager;

#pragma region //	 Deep copy references.

			if (!other.IsEmpty())
			{
				// If there are no entities inside the source.
				m_lEntities = other.m_lEntities; // std::vector does a deep copy of its contents.
			}
			else
			{
				// If there are entities inside the source.
				m_lEntities = std::vector<uint>();
			}

			if (!other.IsRoot())
			{
				// If the source is not a root.
				m_pParent = new MyOctant(*other.m_pParent);
				m_pRoot = new MyOctant(*other.m_pRoot);
			}
			else
			{
				// If the source is root.
				m_pParent = nullptr;
				m_pRoot = nullptr;
			}

			if (!other.IsLeaf())
			{
				// If the source has children.
				m_pChild[8] = new MyOctant[8];
				for (uint i = 0; i < 8; i++)
				{
					// Fill array with other's.
					m_pChild[i] = new MyOctant(*(other.m_pChild[i]));
				}
			}
			else
			{
				// If the source doesn't have children.
				m_pChild[8] = new MyOctant[8];
				for (uint i = 0; i < 8; i++)
				{
					// Fill array with nullptrs.
					m_pChild[i] = nullptr;
				}
			}

#pragma endregion
		
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
		// Reduce octant count by one.
		m_uOctantCount--;
		if (m_uOctantCount == -1) { m_uOctantCount = 0; }
		this->ClearEntityList();

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
		// Move over data.
		(*this) = MyOctant(other);

		// Release the other.
		other.Release();
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

			// Move over data.
			(*this) = MyOctant(other);

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
		// Swap instance references.
		std::swap(m_pMeshManager, other.m_pMeshManager);
		std::swap(m_pEntityManager, other.m_pEntityManager);

		// Swap values.
		std::swap(m_uID, other.m_uID);
		std::swap(m_uDepth, other.m_uDepth);
		std::swap(m_uChildCount, other.m_uChildCount);
		
		std::swap(m_v3Center, other.m_v3Center);
		std::swap(m_v3Maximum, other.m_v3Maximum);
		std::swap(m_v3Minimum, other.m_v3Minimum);
		std::swap(m_v3HalfWidths, other.m_v3HalfWidths);


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

		// Overwrite default information.
		m_uID = 0;
		m_uDepth = 0;
		m_uChildCount = 0;
		m_v3HalfWidths = ZERO_V3;
		m_v3Center = ZERO_V3;
		m_v3Minimum = ZERO_V3;
		m_v3Maximum = ZERO_V3;
		m_lEntities.clear();

		// Release pointers.
		m_pMeshManager = nullptr;
		m_pEntityManager = nullptr;
		m_pParent = nullptr;
		m_pRoot = nullptr;

		// Delete values.
		delete[] m_pChild;

		// If there are elements in a list.
		if (!m_lActiveChildren.empty())
		{
			for (uint i = 0; i < m_lActiveChildren.size(); i++)
			{
				if (m_lActiveChildren[i] != nullptr) {
					delete m_lActiveChildren[i];
					m_lActiveChildren[i] = nullptr;
				}
			}
			m_lActiveChildren.clear();
		}
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
		
		// Initialize the fields to their default values.
		m_uID = 0; 
		m_uDepth = 0;
		m_uChildCount = 0;

		m_v3HalfWidths = ZERO_V3;
		m_v3Center = ZERO_V3;
		m_v3Minimum = ZERO_V3;
		m_v3Maximum = ZERO_V3;

		std::vector<uint> m_lEntityQueue = std::vector<uint>();
		std::vector<uint> m_lEntities = std::vector<uint>();
		std::vector<MyOctant*> m_lActiveChildren = std::vector<MyOctant*>();

		// Initializes memory managed members.
		m_pParent = nullptr; // Will be set after init is called.
		m_pRoot = nullptr; // Reference to the root octant. (Will also be set after init is called).
						   
		// Creates a pointer array that will hold 8 octants.
		m_pChild[8] = new MyOctant[8]; 
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
		// TODO.
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
		// TODO.

		// Pseudocode:
		// Get the Entity from the EntityManager, using the supplied index.
		// If, and only if, the Entity exists, check if it is colliding with the octant volume.
		// If it isn't colliding OR if the Entity does not exist, return false.
		// Else, return true.
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
		return (GetEntityCount() == 0);
	}

#pragma endregion

#pragma region //	Mutator Methods

	// TODO.

#pragma endregion

#pragma region //	Accessor Methods

	/*
	USAGE: Gets this octant's size.
	ARGUMENTS: ---
	OUTPUT: Returns the size of the octant as a float.
	*/
	float MyOctant::GetSize(void) const
	{
		// This will return the average of the widths in all dimensions.
		return (m_v3HalfWidths.length() * 2.0);
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
		return MyOctant::m_uOctantCount;
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

#pragma endregion

#pragma region //	Service Methods
	
	/*
	USAGE: Display the MyOctant object, specified by index, including the objects underneath. (Used to display children).
	ARGUMENTS:
	- uint a_uIndex -> MyOctant to be displayed.
	- vector3 a_v3color = C_YELLOW -> Color to be displayed in.
	OUTPUT: ---
	*/
	void MyOctant::Display(uint a_uIndex, vector3 a_v3color = C_YELLOW)
	{
		// TODO.
	}

	/*
	USAGE: Displays the MyOctant volume in the color specified.
	ARGUMENTS: vector3 a_v3Color = C_YELLOW -> Color to be displayed in.
	OUTPUT: ---
	*/
	void MyOctant::Display(vector3 a_v3Color = C_YELLOW)
	{
		// TODO.
	}

	/*
	USAGE: Displays the non-empty leaf nodes in the octree.
	ARGUMENTS:
	- vector3 a_v3Color = C_YELLOW -> Color to display leaf octant in.
	OUTPUT: ---
	*/
	void MyOctant::DisplayLeafs(vector3 a_v3Color = C_YELLOW)
	{
		// TODO.
	}

	/*
	USAGE: Clears the entity list for each node.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::ClearEntityList(void)
	{
		// Checks if it has any entities.
		if(!IsEmpty()) 
		{
			m_lEntities.clear();
		}

		// Checks if it has any children.
		if (!IsLeaf()) 
		{
			for (uint i = 0; i < 8; i++) 
			{
				MyOctant child = *m_pChild[i];
				child.ClearEntityList();
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
		// TODO.
	}

	/*
	USAGE: Creates a tree using subdivisions, the max number of objects and levels.
	ARGUMENTS: uint a_uSubdivisionThreshold = 3 -> Sets the maximum level of the tree it is constructing.
	OUTPUT: ---
	*/
	void MyOctant::ConstructTree(uint a_uSubdivisionThreshold = 3)
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
		// TODO.
	}

#pragma endregion
	
} // namespace Simplex