#include "j1Pathfinding.h"


// Module Pathfinding methods
j1Pathfinding::j1Pathfinding()
{
}


j1Pathfinding::~j1Pathfinding()
{
}

iPoint j1Pathfinding::FindNextTile(iPoint origin, iPoint destination)
{
	p2List<iPoint> path_tiles;
	p2List<iPoint> breadcrumbs;

	//Find adjacent tiles
	iPoint neighbors[4];
	neighbors[0].create(origin.x - 1, origin.y); //left neighbor
	neighbors[1].create(origin.x + 1, origin.y); //right neighbor
	neighbors[2].create(origin.x, origin.y - 1); //upper neighbor
	neighbors[3].create(origin.x, origin.y + 1); //down neighbor

	int lowest_distance = 10000000; //set the lowest distance to an absurdly high value which will never be reached, so any distance checked later will be lower than this

	for (int i = 0; i < 4; i++)
	{
		if (neighbors[i].DistanceManhattan(destination) < lowest_distance)
		{
			lowest_distance = neighbors[i].DistanceManhattan(destination);
			path_tiles.add(neighbors[i]);
		}
	}

	return { 0,0 };
}

// PathNode methods
PathNode::PathNode(int g, int h, const iPoint & pos, const PathNode * parent)
{
}

int PathNode::Score() const
{
	return 0;
}
