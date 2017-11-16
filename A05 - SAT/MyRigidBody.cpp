#include "MyRigidBody.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();

	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Normals = std::vector<vector3>();

	m_v3Normals.push_back(ZERO_V3);
	m_v3Normals.push_back(ZERO_V3);
	m_v3Normals.push_back(ZERO_V3);

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pMeshMngr, a_pOther.m_pMeshMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
vector3 MyRigidBody::ToWorldSpace(matrix4 a_m4WorldMatrix, vector3 a_v3Vector) { return vector3(a_m4WorldMatrix * vector4(a_v3Vector, 1.0f)); }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pMeshMngr = a_pOther.m_pMeshMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };
//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding as pre-test
	bool bColliding = (glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal()) < m_fRadius + a_pOther->m_fRadius);
	
	//if they are colliding check the SAT
	if (bColliding)
	{
		if (SAT(a_pOther) != eSATResults::SAT_NONE) {
			bColliding = false;// reset to false
		}
	}

	if (bColliding) //they are colliding
	{
		this->AddCollisionWith(a_pOther);
		a_pOther->AddCollisionWith(this);
	}
	else //they are not colliding
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}

	return bColliding;
}
void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0) 
		{
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		}
		else
		{
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
		}

		/* // Display normals.
		for (uint i = 0; i < m_v3Normals.size(); i++)
		{
			if (m_v3Normals[i] != ZERO_V3 && i < 3) {
				m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, this->GetCenterLocal(), m_v3Normals[i], C_BROWN, C_BROWN);
			}
		} */
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	/*
	Your code goes here instead of this comment;

	Ian Effendi (iae2784@g.rit.edu)

	So, as a rundown, I really struggled with this assignment.
	On my first attempt, I attempted to use the global space,
	but, ran into issues when it came time to project the translation vector 
	and halfwidths onto the axis.

	On a high-level, I understand the theorem in terms of the pseudocode, but,
	I've only seen the textbook's version (local space) workr, instead of the other way around.

	I do understand how to get all 15 axes, at the very least.

	For this method, if there is an axis that separates the two objects
	then the return will be different than 0; 1 for any separating axis
	is ok if you are not going for the extra credit, if you could not
	find a separating axis you need to return 0, there is an enum in
	Simplex that might help you [eSATResults] feel free to use it.
	(eSATResults::SAT_NONE has a value of 0)
	*/
	
	// Separation Axis Theorem in 3D.

	// Quick check to ensure we aren't colliding against ourselves.
	if (a_pOther != nullptr && this != a_pOther) {




#pragma region Get all the bounding vertices for both objects.

		// Get the bounding points for both AARB's. (8 per object, 16 in total).
		vector3 o_v3MinL = a_pOther->GetMinLocal();
		vector3 o_v3MaxL = a_pOther->GetMaxLocal();

		//Calculate the 8 corners of the other object's cube.
		vector3 o_v3Corner[8];
		vector3 o_v3Globals[8];

		//Back square
		o_v3Corner[0] = o_v3MinL;
		o_v3Corner[1] = vector3(o_v3MaxL.x, o_v3MinL.y, o_v3MinL.z);
		o_v3Corner[2] = vector3(o_v3MinL.x, o_v3MaxL.y, o_v3MinL.z);
		o_v3Corner[3] = vector3(o_v3MaxL.x, o_v3MaxL.y, o_v3MinL.z);

		//Front square
		o_v3Corner[4] = vector3(o_v3MinL.x, o_v3MinL.y, o_v3MaxL.z);
		o_v3Corner[5] = vector3(o_v3MaxL.x, o_v3MinL.y, o_v3MaxL.z);
		o_v3Corner[6] = vector3(o_v3MinL.x, o_v3MaxL.y, o_v3MaxL.z);
		o_v3Corner[7] = o_v3MaxL;

		//Calculate the 8 corners of this object's cube.
		vector3 v3Corner[8];
		vector3 v3Globals[8];

		//Back square
		v3Corner[0] = this->m_v3MinL;
		v3Corner[1] = vector3(this->m_v3MaxL.x, this->m_v3MinL.y, this->m_v3MinL.z);
		v3Corner[2] = vector3(this->m_v3MinL.x, this->m_v3MaxL.y, this->m_v3MinL.z);
		v3Corner[3] = vector3(this->m_v3MaxL.x, this->m_v3MaxL.y, this->m_v3MinL.z);

		//Front square
		v3Corner[4] = vector3(this->m_v3MinL.x, this->m_v3MinL.y, this->m_v3MaxL.z);
		v3Corner[5] = vector3(this->m_v3MaxL.x, this->m_v3MinL.y, this->m_v3MaxL.z);
		v3Corner[6] = vector3(this->m_v3MinL.x, this->m_v3MaxL.y, this->m_v3MaxL.z);
		v3Corner[7] = this->m_v3MaxL;

		//Place both sets of vertices into world space.
		for (uint uIndex = 0; uIndex < 8; ++uIndex)
		{
			v3Globals[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
			o_v3Globals[uIndex] = vector3(a_pOther->m_m4ToWorld * vector4(o_v3Corner[uIndex], 1.0f));
		}

#pragma endregion

#pragma region Get normals / separating axes for each of the 15 cases.
		
		// Set up the face normals, if not set up already.
		if (m_v3Normals.size() < 6) 
		{
			m_v3Normals.push_back(ZERO_V3);
			m_v3Normals.push_back(ZERO_V3);
			m_v3Normals.push_back(ZERO_V3);
			m_v3Normals.push_back(ZERO_V3);
			m_v3Normals.push_back(ZERO_V3);
			m_v3Normals.push_back(ZERO_V3);
		}

		// Get axes for each check.
		vector3 Axes[15];
		vector3 temp;
		uint count = 0;

		vector3 push;
		vector3 axis;
		float minA, maxA, minB, maxB;

#pragma region Get face normals.

		// Check against axes for this object's local x, y, z space.
		// // Get x-face normal
		temp = m_v3Center + AXIS_X;		
		m_v3Normals[count] = temp;
		axis = vector3(m_m4ToWorld * vector4(m_v3Normals[count], 1.0f));

		// Project the center of the objects against the axis.
		vector3 projCA = Projection(this->GetCenterGlobal(), axis);
		vector3 projCB = Projection(a_pOther->GetCenterGlobal(), axis);
		float distance = glm::distance(projCA, projCB); // Distance between the vectors.
		
		// Project the halfwidths.


		/*
		float minA, minB, maxA, maxB;
		minA = minB = ScalarProjection(v3Globals[0], axis);
		maxA = maxB = ScalarProjection(o_v3Globals[0], axis);

		// Loop through and check for min and max projections.
		for (uint i = 0; i < 8; i++) 
		{
			// Get min/max from first object's projections.
			float projA = ScalarProjection(v3Globals[i], axis);
			if (projA < minA) {	minA = projA; }
			else if (projA > maxA) { maxA = projA; }

			// Get min/max from second object's projections.
			float projB = ScalarProjection(o_v3Globals[i], axis);
			if (projA < minB) { minB = projB; }
			else if (projA > maxB) { maxB = projB; }
		}
		*/



		// Separating axis.
		// push = this->GetCenterLocal() + glm::normalize(temp - this->GetCenterLocal()) * (this->GetHalfWidth().x * 1.5f);
		// m_pMeshMngr->AddLineToRenderList(this->GetModelMatrix(), this->GetCenterLocal(), push, C_YELLOW, C_RED);

		// Separating axis (normal, identity).
		//m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(axis) * 2.0f, C_WHITE, C_RED);

		// Way to draw the X-axis separating plane:
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(this->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, -90.0f, AXIS_Y), C_RED); // CCW
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(this->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y), C_RED); // CW

		// // Add axis to array.
		Axes[count] = axis;
		count++;
				
		// // Get y-face normal
		temp = m_v3Center + AXIS_Y;
		m_v3Normals[count] = temp;
		axis = vector3(m_m4ToWorld * vector4(m_v3Normals[count], 1.0f));
		
		// Separating axis.
		// push = this->GetCenterLocal() + glm::normalize(temp - this->GetCenterLocal()) * (this->GetHalfWidth().y * 1.5f);
		// m_pMeshMngr->AddLineToRenderList(this->GetModelMatrix(), this->GetCenterLocal(), push, C_YELLOW, C_GREEN);

		// Separating axis (normal, identity).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(axis) * 2.0f, C_WHITE, C_GREEN);

		// Way to draw the Y-axis separating plane:
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(this->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, -90.0f, AXIS_X), C_GREEN); // CCW
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(this->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_X), C_GREEN); // CW
		
		// // Add axis to array.
		Axes[count] = axis;
		count++;

		// // Get z-face normal
		temp = m_v3Center + AXIS_Z;
		m_v3Normals[count] = temp;
		axis = vector3(m_m4ToWorld * vector4(m_v3Normals[count], 1.0f));

		// Separating axis.
		// push = this->GetCenterLocal() + glm::normalize(temp - this->GetCenterLocal()) * (this->GetHalfWidth().z * 1.5f);
		// m_pMeshMngr->AddLineToRenderList(this->GetModelMatrix(), this->GetCenterLocal(), push, C_YELLOW, C_BLUE);

		// Separating axis (normal, identity).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(axis) * 2.0f, C_WHITE, C_BLUE);

		// Way to draw the Z-axis separating plane:
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(this->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 180.0f, AXIS_X), C_BLUE); // CCW
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(this->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 0.0f, AXIS_X), C_BLUE); // CW
		
		// // Add axis to array.
		Axes[count] = axis;
		count++;
		
		// Check against axes for the other object's local x, y, z space.
		// // Get x-face normal
		temp = a_pOther->GetCenterLocal() + AXIS_X;		
		m_v3Normals[count] = temp;
		axis = vector3(a_pOther->GetModelMatrix() * vector4(m_v3Normals[count], 1.0f));

		// Separating axis.
		// push = a_pOther->GetCenterLocal() + glm::normalize(temp - a_pOther->GetCenterLocal()) * (a_pOther->GetHalfWidth().x * 1.5f);
		// m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), a_pOther->GetCenterLocal(), push, C_YELLOW, C_RED);

		// Separating axis (normal, identity)
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(axis) * 2.0f, C_PURPLE, C_RED);

		// Way to draw the X-axis separating plane:
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(a_pOther->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, -90.0f, AXIS_Y), C_RED); // CCW
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(a_pOther->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y), C_RED); // CW

		// // Add axis to array.
		Axes[count] = axis;
		count++;
		
		// // Get y-face normal
		temp = a_pOther->GetCenterLocal() + AXIS_Y;
		m_v3Normals[count] = temp;
		axis = vector3(a_pOther->GetModelMatrix() * vector4(m_v3Normals[count], 1.0f));
		
		// Separating axis.
		// push = a_pOther->GetCenterLocal() + glm::normalize(temp - a_pOther->GetCenterLocal()) * (a_pOther->GetHalfWidth().y * 1.5f);
		// m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), a_pOther->GetCenterLocal(), push, C_YELLOW, C_GREEN);

		// Separating axis (normal, identity)
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(axis) * 2.0f, C_PURPLE, C_GREEN);

		// Way to draw the Y-axis separating plane:
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(a_pOther->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, -90.0f, AXIS_X), C_GREEN); // CCW
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(a_pOther->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_X), C_GREEN); // CW
		
		// // Add axis to array.
		Axes[count] = axis;
		count++;

		// // Get z-face normal
		temp = a_pOther->GetCenterLocal() + AXIS_Z;
		m_v3Normals[count] = temp;
		axis = vector3(a_pOther->GetModelMatrix() * vector4(m_v3Normals[count], 1.0f));

		// Separating axis.
		// push = a_pOther->GetCenterLocal() + glm::normalize(temp - a_pOther->GetCenterLocal()) * (a_pOther->GetHalfWidth().z * 1.5f);
		// m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), a_pOther->GetCenterLocal(), push, C_YELLOW, C_BLUE);

		// Separating axis (normal, identity)
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(axis) * 2.0f, C_PURPLE, C_BLUE);

		// Way to draw the Z-axis separating plane:
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(a_pOther->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 180.0f, AXIS_X), C_BLUE); // CCW
		// m_pMeshMngr->AddPlaneToRenderList(glm::translate(a_pOther->GetModelMatrix(), push) * glm::rotate(IDENTITY_M4, 0.0f, AXIS_X), C_BLUE); // CW

		// // Add axis to array.
		Axes[count] = axis;
		count++;

#pragma endregion

#pragma region Cross products of edges.
		
		// Check against cross products for each edge, all of which will be in local space.
		vector3 a, b, a1, a2, a3, b1, b2, b3; // Vectors representing the edges to check against.
		
		a1 = this->ToWorldSpace(m_m4ToWorld, v3Corner[1]) - this->ToWorldSpace(m_m4ToWorld, v3Corner[0]);		
		a2 = this->ToWorldSpace(m_m4ToWorld, v3Corner[2]) - this->ToWorldSpace(m_m4ToWorld, v3Corner[0]);
		a3 = this->ToWorldSpace(m_m4ToWorld, v3Corner[4]) - this->ToWorldSpace(m_m4ToWorld, v3Corner[0]);
		b1 = this->ToWorldSpace(a_pOther->GetModelMatrix(), o_v3Corner[1]) - this->ToWorldSpace(a_pOther->GetModelMatrix(), o_v3Corner[0]);
		b2 = this->ToWorldSpace(a_pOther->GetModelMatrix(), o_v3Corner[2]) - this->ToWorldSpace(a_pOther->GetModelMatrix(), o_v3Corner[0]);
		b3 = this->ToWorldSpace(a_pOther->GetModelMatrix(), o_v3Corner[4]) - this->ToWorldSpace(a_pOther->GetModelMatrix(), o_v3Corner[0]);

		m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, v3Corner[0] * 1.3f, v3Corner[1] * 1.3f, C_YELLOW, C_RED); // Draw edge, a1.
		m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, v3Corner[0] * 1.3f, v3Corner[2] * 1.3f, C_YELLOW, C_GREEN); // Draw edge, a2.
		m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, v3Corner[0] * 1.3f, v3Corner[4] * 1.3f, C_YELLOW, C_BLUE); // Draw edge, a3.
		m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), o_v3Corner[0] * 1.3f, o_v3Corner[1] * 1.3f, C_YELLOW, C_RED); // Draw edge, b1.
		m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), o_v3Corner[0] * 1.3f, o_v3Corner[2] * 1.3f, C_YELLOW, C_GREEN); // Draw edge, b2.
		m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), o_v3Corner[0] * 1.3f, o_v3Corner[4] * 1.3f, C_YELLOW, C_BLUE); // Draw edge, b3.

		vector3 offset = ZERO_V3; // AXIS_X * 2.0f;

#pragma region Display rigidbody debug points + edges.

		// Render the rigidbody debug wireframe:
		/* m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, v3Corner[1]) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_YELLOW);
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, v3Corner[0]) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_YELLOW);
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, v3Corner[2]) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_YELLOW);
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, v3Corner[4]) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_YELLOW);
		m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, v3Corner[0], v3Corner[1], C_YELLOW, C_RED); // X-axis edge.
		m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, v3Corner[0], v3Corner[2], C_YELLOW, C_GREEN); // Y-axis edge.
		m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, v3Corner[0], v3Corner[4], C_YELLOW, C_BLUE); // Z-axis edge.

		// Render the second rigidbody debug wireframe.
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, o_v3Corner[1] - offset) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_PURPLE);
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, o_v3Corner[0] - offset) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_PURPLE);
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, o_v3Corner[2] - offset) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_PURPLE);
		m_pMeshMngr->AddSphereToRenderList(glm::translate(IDENTITY_M4, o_v3Corner[4] - offset) * glm::scale(IDENTITY_M4, vector3(0.1f)), C_PURPLE);
		m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, o_v3Corner[0] - offset, o_v3Corner[1] - offset, C_YELLOW, C_RED); // X-axis edge.
		m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, o_v3Corner[0] - offset, o_v3Corner[2] - offset, C_YELLOW, C_GREEN); // Y-axis edge.
		m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, o_v3Corner[0] - offset, o_v3Corner[4] - offset, C_YELLOW, C_BLUE); // Z-axis edge. */
		
#pragma endregion
		
		// // Get axis from cross product of a1 x b1
		a = glm::cross(a1, b1);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a1 and b1. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_RED, C_RED);
		// m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, this->GetCenterLocal(), this->GetCenterLocal() + glm::normalize(a) * 2.0f, C_RED, C_RED);
		// m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), a_pOther->GetCenterLocal(), a_pOther->GetCenterLocal() + glm::normalize(a) * 2.0f, C_RED, C_RED);

		Axes[count] = axis;
		count++;
		

		// // Get axis from cross product of a1 x b2
		a = glm::cross(a1, b2);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a1 and b2. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_RED, C_GREEN);
		// m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, this->GetCenterLocal(), this->GetCenterLocal() + glm::normalize(a) * 2.0f, C_GREEN, C_GREEN);
		// m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), a_pOther->GetCenterLocal(), a_pOther->GetCenterLocal() + glm::normalize(a) * 2.0f, C_GREEN, C_GREEN);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a1 x b3
		a = glm::cross(a1, b3);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a1 and b3. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_RED, C_BLUE);
		// m_pMeshMngr->AddLineToRenderList(m_m4ToWorld, this->GetCenterLocal(), this->GetCenterLocal() + glm::normalize(a) * 2.0f, C_BLUE, C_BLUE);
		// m_pMeshMngr->AddLineToRenderList(a_pOther->GetModelMatrix(), a_pOther->GetCenterLocal(), a_pOther->GetCenterLocal() + glm::normalize(a) * 2.0f, C_BLUE, C_BLUE);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a2 x b1
		a = glm::cross(a2, b1);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a2 and b1. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_GREEN, C_BLUE);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a2 x b2
		a = glm::cross(a2, b2);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a2 and b2. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_GREEN, C_GREEN);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a2 x b3
		a = glm::cross(a2, b3);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a2 and b3. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_GREEN, C_BLUE);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a3 x b1
		a = glm::cross(a3, b1);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a3 and b1. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_BLUE, C_RED);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a3 x b2
		a = glm::cross(a3, b2);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a3 and b2. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_BLUE, C_GREEN);

		Axes[count] = axis;
		count++;

		// // Get axis from cross product of a3 x b3
		a = glm::cross(a3, b3);
		if (glm::abs(a.x) <= FLT_EPSILON) { a.x = FLT_EPSILON; }
		if (glm::abs(a.y) <= FLT_EPSILON) { a.y = FLT_EPSILON; }
		if (glm::abs(a.z) <= FLT_EPSILON) { a.z = FLT_EPSILON; }
		axis = a;

		// Display the axis orthogonal to both a3 and b3. (The separation axis).
		// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, glm::normalize(a) * 2.0f, C_BLUE, C_BLUE);

		Axes[count] = axis;
		count++;
		
#pragma endregion

#pragma region Display debug lines involving edge cross products. (indices 6 through 14 (elements 7 through 15)).

		/*
		for (uint i = 6; i < sizeof(Axes) / sizeof(Axes[0]); i++)
		{
			m_pMeshMngr->AddLineToRenderList(glm::translate(IDENTITY_M4, offset), ZERO_V3, Axes[i], C_YELLOW, C_PURPLE);
			// m_pMeshMngr->AddLineToRenderList(IDENTITY_M4, ZERO_V3, Axes[i], C_YELLOW, C_YELLOW);
		}
		*/

#pragma endregion

#pragma endregion
		
#pragma region Check projections against each of the axes.
		
	/*

	// Check each of the faces for this rigidbody.
	for (uint axisIndex = 0; axisIndex < sizeof(Axes) / sizeof(Axes[0]); axisIndex++) 
	{
		// Get the axis to project onto, and normalize it.
		axis = glm::normalize(Axes[axisIndex]);

		// Set initial min and max.
		minA = glm::dot(v3Globals[0], axis);
		minB = glm::dot(o_v3Globals[0], axis);
		maxA = minA;
		maxB = minB;
		
		// Find the minimum and maximum dot values.
		for (uint projIndex = 1; projIndex < 8; projIndex++)
		{
			float projA = glm::dot(v3Globals[projIndex], axis);
			float projB = glm::dot(o_v3Globals[projIndex], axis);

			if (projA < minA)
			{
				minA = projA;
			}
			else if (projA > maxA)
			{
				maxA = projA;
			}

			if (projB < minB)
			{
				minB = projB;
			}
			else if (projB > maxB)
			{
				maxB = projB;
			}

			// Check for overlap.
			if (maxA < minB && minA < maxB) {
				// There is a gap.
				return 1; // No collision.
			}
		}
	}

	*/

#pragma endregion

	}

	//there is no axis test that separates this two objects
	return eSATResults::SAT_NONE;
}
vector3 MyRigidBody::Normalize(vector3 const a_pVector)
{
	return glm::normalize(vector3(a_pVector.x, a_pVector.y, a_pVector.z));
}
float MyRigidBody::Dot(vector3 const a_pA, vector3 const a_pB) 
{
	vector3 a = a_pA;
	vector3 b = a_pB;
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
vector3 MyRigidBody::Projection(vector3 const a_pVector, vector3 const a_pAxis)
{
	vector3 proj = a_pAxis;
	float dot = Dot(a_pVector, a_pAxis);
	float lengthSqr = a_pAxis.x * a_pAxis.x + a_pAxis.y * a_pAxis.y + a_pAxis.z * a_pAxis.z;

	proj.x = (dot / lengthSqr) * a_pAxis.x;
	proj.y = (dot / lengthSqr) * a_pAxis.y;

	return proj;
}
float MyRigidBody::ScalarProjection(vector3 const a_pVector, vector3 const a_pAxis)
{
	vector3 proj = a_pAxis;
	float dot = Dot(a_pVector, a_pAxis);
	float length = glm::sqrt(a_pAxis.x * a_pAxis.x + a_pAxis.y * a_pAxis.y + a_pAxis.z * a_pAxis.z);

	return (dot / length);
}
