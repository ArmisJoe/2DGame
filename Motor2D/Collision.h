#ifndef __Collision_H__
#define __Collision_H__

#include "Module.h"
#include "p2Point.h"

class Unit;
class Entity;
class Building;
class Resource;
class StaticQuadTree;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_UNIT,
	COLLIDER_BUILDING,
	COLLIDER_RESOURCE,
	COLLIDER_LOS,
	COLLIDER_RANGE,

	COLLIDER_MAX
};

struct Collider
{
	iPoint pos = { 0,0 };
	int r = 0;
	int quadtree_node = 0;
	bool colliding = false;
	COLLIDER_TYPE type = COLLIDER_NONE;
	Entity* entity = nullptr;
	Module* callback = nullptr;

	Collider(iPoint position, int radius, COLLIDER_TYPE type, Module* callback, Entity* entity) :
		pos(position),
		type(type),
		r(radius),
		callback(callback),
		entity(entity)
	{}

	void SetPos(int x, int y)
	{
		pos.x = x;
		pos.y = y;
	}

	bool CheckCollision(Collider* c2) const;
	Unit* GetUnit();
	Building* GetBuilding();
	Resource* GetResource();
};

class Collision : public Module
{
public:

	Collision();
	virtual ~Collision();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Collider* AddCollider(iPoint position, int radius, COLLIDER_TYPE type, Module* callback = nullptr, Entity* entity = nullptr);
	void DeleteCollider(Collider* collider);

	Collider* FindCollider(iPoint point, int radius = 0, Collider* collider_to_ignore = nullptr);
	bool CheckCollisionsIn(iPoint worldPos);
	void DebugDraw();

private:

	list<Collider*> colliders;
	list<Collider*> colliders_to_delete;
	
	list<Collider*> potential_collisions;

public:
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	StaticQuadTree* quadTree = nullptr;
	Unit* relevant_unit = nullptr;
	bool debug = false;
};

#endif // __ModuleCollision_H__

