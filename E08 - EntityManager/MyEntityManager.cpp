#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	// If the instance pointer is null, set it equal to this reference.
	if (m_pInstance == nullptr) { m_pInstance = this; }

	// Sets up empty list.
	std::vector<MyEntity*> m_entityList = std::vector<MyEntity*>();
	m_uEntityCount = 0;
}
void MyEntityManager::Release(void)
{
	// Releases all the entities.
	for (uint count = 0; count < m_entityList.size(); count++)
	{
		// Check if entity needs to be deleted.
		MyEntity* e = m_entityList[count];
		if (e != nullptr) 
		{
			delete e;
			e = nullptr;
		}

		// Set to null reference.
		m_entityList[count] = nullptr;
	}

	// Sets up empty list.
	std::vector<MyEntity*> m_entityList = std::vector<MyEntity*>();
	m_uEntityCount = 0;
}
MyEntityManager* MyEntityManager::GetInstance(void)
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance(void)
{
	if (m_pInstance != nullptr) 
	{
		m_pInstance->Release();
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++) 
	{
		MyEntity* entity = GetEntity(i);
		if (entity != nullptr) 
		{
			if (entity->GetUniqueID() == a_sUniqueID) {
				return i;
			}
		}
	}

	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount) 
	{
		uint indexOf = m_uEntityCount = 1;

		if (a_uIndex < 0) 
		{
			indexOf = a_uIndex;
		}

		return this->GetEntity(indexOf)->GetModel();
	}
	
	// Index out of bounds.
	return nullptr;
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	uint indexOf = GetEntityIndex(a_sUniqueID);
	if(indexOf < 0 || m_uEntityCount == 0)
	{
		return nullptr;
	}

	return this->GetModel(indexOf);
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount)
	{
		uint indexOf = m_uEntityCount = 1;

		if (a_uIndex < 0)
		{
			indexOf = a_uIndex;
		}

		return this->GetEntity(indexOf)->GetRigidBody();
	}

	// Index out of bounds.
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	uint indexOf = GetEntityIndex(a_sUniqueID);
	if (indexOf < 0 || m_uEntityCount == 0)
	{
		return nullptr;
	}

	return this->GetRigidBody(indexOf);
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount)
	{
		uint indexOf = m_uEntityCount = 1;

		if (a_uIndex < 0)
		{
			indexOf = a_uIndex;
		}

		return this->GetEntity(indexOf)->GetModelMatrix();
	}

	// Index out of bounds.
	return IDENTITY_M4;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	uint indexOf = GetEntityIndex(a_sUniqueID);
	if (indexOf < 0 || m_uEntityCount == 0)
	{
		return IDENTITY_M4;
	}

	return this->GetModelMatrix(indexOf);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	if (a_sUniqueID.size() > 0) {
		this->SetModelMatrix(a_m4ToWorld, this->GetEntityIndex(a_sUniqueID));
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (a_uIndex > 0 && a_uIndex < m_uEntityCount) {
		MyEntity* entity = GetEntity(a_uIndex);
		if (entity != nullptr)
		{
			entity->SetModelMatrix(a_m4ToWorld);
		}
	}
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	for (uint i = 0; i < m_uEntityCount; i++) 
	{
		// Update each entity here.
		// No specifications as to what this method is supposed to do, was included.
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	// Add an entry to the list and increment collection count.
	this->m_entityList.push_back(new MyEntity(a_sFileName, a_sUniqueID));
	this->m_uEntityCount++;
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (m_uEntityCount > 0 && a_uIndex < m_uEntityCount) {

		// When less than zero, remove last added.
		uint remove = m_uEntityCount = 1;

		if (a_uIndex > 0) 
		{
			// Set index if it's not less than zero.
			remove = a_uIndex;
		}

		// Erase element at specified location.
		m_entityList.erase(m_entityList.begin() + remove);
		m_uEntityCount--;
	}
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	if (a_sUniqueID != "" && m_uEntityCount > 0) 
	{
		uint indexOf = GetEntityIndex(a_sUniqueID);
		if (indexOf > 0 < m_uEntityCount) {
			RemoveEntity(indexOf);
		}
	}
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if(m_uEntityCount <= 0) {
		return "";
	}
	else 
	{
		// When less than zero, get from last added.
		uint indexOf = m_uEntityCount - 1;

		if (a_uIndex > 0) 
		{
			// Set index if it's not less than zero.
			indexOf = a_uIndex;
		}	
		
		// Return UniqueID of entity at position.
		return this->GetEntity(indexOf)->GetUniqueID();
	}
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount)
	{
		if (a_uIndex < 0)
		{
			return m_entityList[m_uEntityCount - 1];
		}

		// Return model associated with entity at specified index.
		return m_entityList[a_uIndex];
	}

	// If index is out of range.
	return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{

}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{

}