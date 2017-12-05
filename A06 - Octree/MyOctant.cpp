/*
	MyOctant.cpp
	Implementation for the Octree node.
	Ian Effendi (iae2784@g.rit.edu)
*/

#include "MyOctant.h"
namespace Simplex {

#pragma region Constructor(s)

	/*
	USAGE: Constructor will create an octant containing all the Entities that the MeshManager currently contains.
	ARGUMENTS:
	- uint a_uSubdivisionLimit = 2 -> Sets the maximum level of subdivisions.
	- uint a_uIdealEntityCount = 5 -> Sets the ideal level of objects per octant.
	OUTPUT: class object.
	*/
	MyOctant::MyOctant(uint a_uSubdivisionLimit, uint a_uIdealEntityCount) 
	{
		// TODO.
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
		// TODO.
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
		// TODO.
	}

#pragma endregion

#pragma region Rule of Three
	
	/*
	USAGE: Copy constructor.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: class object instance.
	*/
	MyOctant::MyOctant(MyOctant const& other)
	{
		// TODO.
	}

	/*
	USAGE: Copy assignment operator.
	ARGUMENTS: other -> class object to copy.
	OUTPUT: ---
	*/
	MyOctant& MyOctant::operator=(MyOctant const& other)
	{
		// TODO.
	}

	/*
	USAGE: Destructor.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	MyOctant::~MyOctant(void)
	{
		// TODO.
	}

#pragma endregion

#pragma region Initialization/Memory Management Methods

	/*
	USAGE: Deallocates the member fields in need of memory management.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Release(void) 
	{
		// TODO.
	}

	/*
	USAGE: Initializes the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::Init(void)
	{
		// TODO.
	}

	/*
	USAGE: Allocates member field data and initializes the octant.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void MyOctant::InitVariables(void)
	{
		// TODO.
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

#pragma region Flag Methods

	/*
	USAGE: Asking if there is a collision with an Entity as supplied by the input index value.
	ARGUMENTS: uint a_uRBIndex -> Index of the Entity in the EntityManager.
	OUTPUT: Check of the collision.
	*/
	bool MyOctant::IsColliding(uint a_uRBIndex)
	{
		// TODO.
	}

	/*
	USAGE: Asks if this octant has no parents.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsRoot(void)
	{
		// TODO.
	}

	/*
	USAGE: Asks if this octant has no children.
	ARGUMENTS: ---
	OUTPUT: Returns check.
	*/
	bool MyOctant::IsLeaf(void)
	{
		// TODO.
	}

	/*
	USAGE: Checks to see if this octant has more than the input number of entities available.
	ARGUMENTS: uint a_uEntities -> Number of entities to check for.
	OUTPUT: Returns check.
	*/
	bool MyOctant::ContainsMoreThan(uint a_uEntities)
	{
		// TODO.
	}

#pragma endregion

#pragma region Accessor Methods

	/*
	USAGE: Gets this octant's size.
	ARGUMENTS: ---
	OUTPUT: Returns the size of the octant as a float.
	*/
	float MyOctant::GetSize(void)
	{
		// TODO.
	}

	/*
	USAGE: Gets this octant's half widths.
	ARGUMENTS: ---
	OUTPUT: Returns the half widths of the octant as a vector3.
	*/
	vector3 MyOctant::GetHalfWidths(void)
	{
		// TODO.
	}

	/*
	USAGE: Get the center of the octant in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetCenterGlobal(void)
	{
		// TODO.
	}

	/*
	USAGE: Get the maximum vector in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetMaxGlobal(void)
	{
		// TODO.
	}

	/*
	USAGE: Get the minimum vector in global space.
	ARGUMENTS: ---
	OUTPUT: Returns a vector3.
	*/
	vector3 MyOctant::GetMinGlobal(void)
	{
		// TODO.
	}

	/*
	USAGE: Returns the child at the specified index (if possible).
	ARGUMENTS: uint a_uChild -> index of the child [0, 7].
	OUTPUT: MyOctant object (child at index).
	*/
	MyOctant* MyOctant::GetChild(uint a_uChild)
	{
		// TODO.
	}

	/*
	USAGE: Returns the parent of this octant.
	ARGUMENTS: ---
	OUTPUT: MyOctant object (parent of this octant).
	*/
	MyOctant* MyOctant::GetParent(void)
	{
		// TODO.
	}

	/*
	USAGE: Return the total number of octants in the world.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	uint MyOctant::GetOctantCount(void)
	{
		// TODO.
	}

#pragma endregion

#pragma region Service Methods

	/*
	USAGE: Changes the object's contents for the input object.
	ARGUMENTS: MyOctant& other -> The other object to swap contents with.
	OUTPUT: ---
	*/
	void MyOctant::Swap(MyOctant& other)
	{
		// TODO.
	}

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
		// TODO.
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