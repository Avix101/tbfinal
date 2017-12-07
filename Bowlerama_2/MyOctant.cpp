#include "MyOctant.h"
using namespace Simplex;

//Set some static variables first
int MyOctant::octantCount = 0;
int MyOctant::maximumLvl = 3;
int MyOctant::idealEntities = 5;


//Constructors, destructor, release
Simplex::MyOctant::MyOctant(uint maxLevel, uint ideal) {
	//Call init function to set variables
	Init();

	//Set some initial values to the passed in ones
	octantCount = 0;
	maximumLvl = maxLevel;
	idealEntities = ideal;

	//ID equals the octant count
	ID = octantCount;

	//Set this first octant to the root
	// then, clear the child list at first
	root = this;
	childList.clear();

	//Vector for holding mins and maxs, as vector3's
	std::vector<vector3> minsMaxs;

	//Get the number of entities in the entity manager singleton
	uint numEntities = entityManager->GetEntityCount();
	for (uint i = 0; i < numEntities; i++) {
		//For all the entities, push their mins and maxs to the numEntities vector
		MyEntity* entity = entityManager->GetEntity(i);
		MyRigidBody* rigidB = entity->GetRigidBody();
		minsMaxs.push_back(rigidB->GetMinGlobal());
		minsMaxs.push_back(rigidB->GetMaxGlobal());
	}
	MyRigidBody* rigidBody = new MyRigidBody(minsMaxs);

	//Get the half width from the newly created rigidBody from the mins and maxs vector
	vector3 halfWidth = rigidBody->GetHalfWidth();

	//Get the largest maximum value
	float max = halfWidth.x;
	for (int i = 1; i < 3; i++) {
		if (max < halfWidth[i]) {
			max = halfWidth[i];
		}
	}

	//Get the local center of the rigidBody
	vector3 center = rigidBody->GetCenterLocal();

	//Get rid of the vector, delete the rigidBody pointer
	minsMaxs.clear();
	SafeDelete(rigidBody);

	//The octant size needs to be twice the size of the largest maximum x value,
	// to contain all of the entities in the scene
	octantSize = max * 2.0f;
	//Set the center for this root of the tree
	centerPoint = center;
	//Min and max of the root
	minVector = center - (vector3(max));
	maxVector = center + (vector3(max));

	//The octant count is now 1, because this is for the root
	octantCount++;

	//Make the tree now
	ConstructTree(maximumLvl);
}

Simplex::MyOctant::MyOctant(vector3 center, float size) {
	//Call init, set the passed in values accordingly
	Init();
	centerPoint = center;
	octantSize = size;

	//Set the min and max vector3's
	minVector = centerPoint - (vector3(octantSize) / 2.0f);
	maxVector = centerPoint + (vector3(octantSize) / 2.0f);

	//Increase the octant count
	octantCount++;
}

void Simplex::MyOctant::Release(void) {
	//Clear the branches
	if (currentLevel == 0) {
		ClearAllBranches();
	}
	//Reset the children, size, and index list
	numChildren = 0;
	octantSize = 0.0f;
	entityIndexList.clear();
	childList.clear();
}

Simplex::MyOctant::~MyOctant(void) {
	//Call Release
	Release();
}


//Gets and Sets come next
float Simplex::MyOctant::GetSize(void) {
	return octantSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void) {
	return centerPoint;
}

vector3 Simplex::MyOctant::GetMinGlobal(void) {
	return minVector;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void) {
	return maxVector;
}

MyOctant* Simplex::MyOctant::GetChild(uint index) {
	if (index > 7) {
		return nullptr;
	}
	return children[index];
}

MyOctant* Simplex::MyOctant::GetParent(void) {
	return parent;
}

uint Simplex::MyOctant::GetOctantCount(void) {
	return octantCount;
}


//Dislay functions
void Simplex::MyOctant::Display(uint index, vector3 color) {
	if (ID == index) {
		//Add a wire cube to the render list
		// It's model matrix is it's center point scaled by the size of that octant
		meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, centerPoint) *
			glm::scale(vector3(octantSize)), color, RENDER_WIRE);

		return;
	}

	//Draw all the children as well, make it recursive
	for (uint i = 0; i < numChildren; i++) {
		children[i]->Display(index, C_YELLOW);
	}
}

void Simplex::MyOctant::Display(vector3 color) {
	//Draw all the children, make it recursive
	for (uint i = 0; i < numChildren; i++) {
		children[i]->Display(color);
	}

	//Add a wire cube to the render list
	// It's model matrix is it's center point scaled by the size of that octant
	meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, centerPoint) *
		glm::scale(vector3(octantSize)), color, RENDER_WIRE);
}


//Other functions
void Simplex::MyOctant::Init(void) {
	//Get the singletons for the mesh and entity managers
	meshManager = MeshManager::GetInstance();
	entityManager = MyEntityManager::GetInstance();

	//The pointers should start null
	root = nullptr;
	parent = nullptr;
	for (uint i = 0; i < 8; i++) {
		children[i] = nullptr;
	}

	//Set initial values of a bunch of variables
	ID = octantCount;
	currentLevel = 0;
	numChildren = 0;

	octantSize = 0.0f;

	centerPoint = vector3(0.0f,0.0f,0.0f);
	minVector = vector3(0.0f, 0.0f, 0.0f);
	maxVector = vector3(0.0f, 0.0f, 0.0f);
}

void Simplex::MyOctant::AssignIDtoEntity(void) {
	//Recursively call this function for all the children
	for (uint c = 0; c < numChildren; c++) {
		children[c]->AssignIDtoEntity();
	}
	//If there are no children...
	if (numChildren == 0) {
		//... Add a dimension to the entity manager, with its ID,
		// also, add it to the entity index list
		uint entityCount = entityManager->GetEntityCount();
		for (uint i = 0; i < entityCount; i++) {
			if (IsColliding(i)) {
				entityIndexList.push_back(i);
				entityManager->AddDimension(i, ID);
			}
		}
	}
}

void Simplex::MyOctant::ConstructList(void) {
	//Make it recursive
	for (uint c = 0; c < numChildren; c++) {
		children[c]->ConstructList();
	}

	//Push this to the root's childlist
	if (entityIndexList.size() > 0) {
		root->childList.push_back(this);
	}
}

bool Simplex::MyOctant::IsColliding(uint index) {
	uint entityCount = entityManager->GetEntityCount();
	if (index >= entityCount) {
		return false;
	}
	
	//Get the entity in question (The index that's passed in)
	MyEntity* entity = entityManager->GetEntity(index);
	//Get its rigidBody
	MyRigidBody* rigidBody = entity->GetRigidBody();
	//Get that rigidBody's min and max global vector3's
	vector3 min = rigidBody->GetMinGlobal();
	vector3 max = rigidBody->GetMaxGlobal();

	//This is basically the check that happens in the MyRigidBody class,
	// but I moved it here instead, because we need a collision check somewhere.
	if (maxVector.x < min.x) {
		return false;
	}
	if (minVector.x > max.x) {
		return false;
	}

	if (maxVector.y < min.y) {
		return false;
	}
	if (minVector.y > max.y) {
		return false;
	}

	if (maxVector.z < min.z) {
		return false;
	}
	if (minVector.z > max.z) {
		return false;
	}

	return true;
}

void Simplex::MyOctant::ClearEntityList(void) {
	//Make it recursive
	for (uint c = 0; c < numChildren; c++) {
		children[c]->ClearEntityList();
	}
	//Clear the entity index list
	entityIndexList.clear();
}

void Simplex::MyOctant::Subdivide(void) {
	//Don't subidivide if we're at the max level
	if (currentLevel >= maximumLvl) {
		return;
	}
	
	//If there's nothing to divide into, then don't
	if (numChildren != 0) {
		return;
	}

	//Make 8 children, hence, the lovely Octree
	numChildren = 8;

	//The size is the current octant size, divided by 4
	float size = octantSize / 4.0f;
	//The doubleSize is for the size of the whole octant, like the cube it takes up
	float sizeDouble = size * 2.0f;

	//Make a new center point, starting at this, the 'parent's' center
	vector3 center;
	center = centerPoint;
	//Now, make the new center of the first octant child, so it takes up a quarter of the parent's size
	center.x -= size;
	center.y -= size;
	center.z -= size;
	//Make a new octant, with the new center and it's smaller size
	children[0] = new MyOctant(center, sizeDouble);

	//Increase just the X value, and make the next child
	center.x += sizeDouble;
	children[1] = new MyOctant(center, sizeDouble);

	//Increase just the Z
	center.z += sizeDouble;
	children[2] = new MyOctant(center, sizeDouble);

	//Decrease the X now, so it's next to the first one
	center.x -= sizeDouble;
	children[3] = new MyOctant(center, sizeDouble);

	//Now, start the octants above the first 4
	center.y += sizeDouble;
	children[4] = new MyOctant(center, sizeDouble);

	//Decrease the Z this time
	center.z -= sizeDouble;
	children[5] = new MyOctant(center, sizeDouble);

	//Increase the X
	center.x += sizeDouble;
	children[6] = new MyOctant(center, sizeDouble);

	//Increase the Z for the last child
	center.z += sizeDouble;
	children[7] = new MyOctant(center, sizeDouble);

	//Set the childrens values here
	for (uint i = 0; i < 8; i++) {
		//The root is the same for all
		children[i]->root = root;

		//The parent will be this one, using keyword 'this'
		children[i]->parent = this;

		//Increase the level of Octant-ness
		children[i]->currentLevel = currentLevel + 1;

		//Now, we have to check and see if we can subdivide again
		int count = 0;
		int entityCount = entityManager->GetEntityCount();

		for (int i = 0; i < entityCount; i++) {
			if (IsColliding(i)) {
				count++;
			}
		}

		//Subdivide, if the number of those colliding is more than ideal
		if (count > idealEntities) {
			children[i]->Subdivide();
		}
	}
}

void Simplex::MyOctant::ClearAllBranches(void) {
	//Clear everything, reset the children, make it recursive
	for (uint c = 0; c < numChildren; c++) {
		children[c]->ClearAllBranches();
		delete children[c];
		children[c] = nullptr;
	}
	numChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint maxLevel) {
	if (currentLevel != 0) {
		return;
	}

	//Time to make a tree, make maximumLevel = the passed in level
	maximumLvl = maxLevel;
	//We only have one octant
	octantCount = 1;
	//Clear the index list
	entityIndexList.clear();
	//Get rid of all branches
	ClearAllBranches();
	//Clear the child list
	childList.clear();

	//Subdivide if possible
	int count = 0;
	int entityCount = entityManager->GetEntityCount();

	for (int i = 0; i < entityCount; i++) {
		if (IsColliding(i)) {
			count++;
		}
	}

	if (count > idealEntities) {
		Subdivide();
	}

	//Assign ID's to the entities, then, make the list
	AssignIDtoEntity();
	ConstructList();
}