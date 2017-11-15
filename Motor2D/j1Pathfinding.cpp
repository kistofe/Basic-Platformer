#include "j1Pathfinding.h"



j1Pathfinding::j1Pathfinding()
{
}


j1Pathfinding::~j1Pathfinding()
{
}

iPoint j1Pathfinding::FindNextTile(iPoint origin, iPoint destination)
{
	iPoint ret;
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
			ret = neighbors[i];
			lowest_distance = neighbors[i].DistanceManhattan(destination);
		}
	}

	return ret;
}