#ifndef __j1Pathfinding_H__
#define __j1Pathfinding_H__

#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "p2Log.h"
#include "j1Module.h"

class j1Pathfinding : public j1Module
{
public:
	j1Pathfinding();
	~j1Pathfinding();
	
	iPoint GetNextTile(const iPoint& origin, const iPoint& destination); // method that gives the next tile in the path from origin to destination

private:
	p2DynArray<iPoint> last_path;
};

// forward declaration
struct PathList;

struct PathNode
{
	PathNode(); //default constructor
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node); //copy constructor

	// Fills a PathList of all walkable adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill, const PathNode* parent) const;
	// Computes the score of the tile
	int Score() const;
	// Compute the F for a given destination tile
	int CalculateF(const iPoint& destination);
	
	iPoint position;
	const PathNode* parent;
	int g;
	int h;
};

struct PathList
{
	p2List_item<PathNode>* Find(const iPoint& point) const;

	p2List_item<PathNode>* GetNodeLowestScore() const;

	p2List<PathNode> list;
};


#endif // __j1Pathfinding_H__