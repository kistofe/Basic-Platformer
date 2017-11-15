#ifndef __j1Pathfinding_H__
#define __j1Pathfinding_H__

#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Module.h"

struct PathNode
{
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	
	int Score() const;
	
	iPoint position;
	PathNode* parent;
	int g;
	int h;
};

class j1Pathfinding : public j1Module
{
public:
	j1Pathfinding();
	~j1Pathfinding();
	
	iPoint FindNextTile(iPoint origin, iPoint destination); //this method returns the coordinates that the entity has to go to in order to get to its destination
	
};

#endif // __j1Pathfinding_H__