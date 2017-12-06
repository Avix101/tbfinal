#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{

class MyOctant
{
	//Mesh and entity manager singletons
	MeshManager* meshManager = nullptr;
	MyEntityManager* entityManager = nullptr;

	//The root and the list of octant children
	MyOctant* root = nullptr;
	std::vector<MyOctant*> childList;

	//The array of children, and the parent pointers
	MyOctant* parent = nullptr;
	MyOctant* children[8];

	//Int's for the octant count, max level, and ideal num of entities
	static int octantCount;
	static int maximumLvl;
	static int idealEntities;

	//Int's for ID, the current level, and number of children
	uint ID = 0;
	uint currentLevel = 0;
	uint numChildren = 0;

	//Size of the octant
	float octantSize = 0.0f;

	//Vector3's for the center, maximum vector, and minimum vector
	vector3 centerPoint = vector3(0.0f);
	vector3 minVector = vector3(0.0f);
	vector3 maxVector = vector3(0.0f);

	//A vector, standard one, for the index list for the entities
	std::vector<uint> entityIndexList;

public:
	//Constructors and destructor, and release, which is pretty much the destructor
	MyOctant(uint maxLevel, uint ideal);

	MyOctant(vector3 center, float size);

	void Release(void);

	~MyOctant(void);


	//Gets and Sets here
	float GetSize(void);

	vector3 GetCenterGlobal(void);

	vector3 GetMinGlobal(void);

	vector3 GetMaxGlobal(void);

	MyOctant* GetChild(uint index);

	MyOctant* GetParent(void);

	uint GetOctantCount(void);


	//Display functions
	void Display(uint a_nIndex, vector3 color);

	void Display(vector3 color);


	//Other functions
	void Init(void);

	void AssignIDtoEntity(void);

	void ConstructTree(uint maxLevel = 3);

	void ClearAllBranches(void);

	void ConstructList(void);

	bool IsColliding(uint index);

	void ClearEntityList(void);

	void Subdivide(void);
};

}

#endif