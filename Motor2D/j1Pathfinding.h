#ifndef __j1Pathfinding_H__
#define __j1Pathfinding_H__

#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "p2Log.h"
#include "j1Module.h"

#define INVALID_WALK_CODE 255

class j1Pathfinding : public j1Module
{
public:
	j1Pathfinding();
	~j1Pathfinding();
	
	iPoint GetNextTile(const iPoint& origin, const iPoint& destination); // method that gives the next tile in the path from origin to destination

	void SetMap(uint width, uint height, uchar* data);
	bool CheckBoundaries(const iPoint& pos) const;
	bool IsWalkable(const iPoint& pos) const; // checks walkability of the tile in given position
	uint GetTileAt(const iPoint& pos) const;

private:

	// size of the map
	uint width;
	uint height;
	// all map walkability values [0..255]
	uchar* map;
	// we store the created path here
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
	uint FindWalkableAdjacents(PathList& list_to_fill) const;
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