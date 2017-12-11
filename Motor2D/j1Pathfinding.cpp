#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"

#include "Brofiler\Brofiler.h"


// Module Pathfinding methods
j1Pathfinding::j1Pathfinding()
{
	name.create("pathfinding");
}


j1Pathfinding::~j1Pathfinding()
{
}

iPoint j1Pathfinding::GetNextTile(const iPoint & origin, const iPoint & destination, const bool is_ground)
{
	BROFILER_CATEGORY("Pathfinding Method", Profiler::Color::Khaki);
	// check if origin or destination are not walkable and return origin if that's the case
	if (!IsWalkable(origin) || !IsWalkable(destination))
		return origin;

	// check if origin is the same as destination
	if (origin == destination)
		return origin;

	PathList open;
	PathList close;
	PathList adjacents;

	// create a node for the origin and add it to the open list
	PathNode path_origin(0, origin.DistanceNoSqrt(destination), origin, nullptr);
	open.list.add(path_origin);

	// iterate while there exist tiles in the open list

	while (open.list.start)
	{
		// pass the node in the open list with the lowest score to the closed list
		close.list.add(open.GetNodeLowestScore()->data);
		open.list.del(open.GetNodeLowestScore());

		if (close.list.end->data.position == destination)
			break;

		// fill adjacents list with the adjacents of the node that is currently being checked
		close.list.end->data.FindWalkableAdjacents(adjacents);
		for (p2List_item<PathNode>* iterator = adjacents.list.start; iterator; iterator = iterator->next) // loop that iterates the adjacents found (max loops: 4)
		{
			if (close.Find(iterator->data.position))
				continue;
			if (close.Find(iterator->data.position) == NULL)
				iterator->data.CalculateF(destination), open.list.add(iterator->data);
			else if (open.Find(iterator->data.position) != NULL)
				if (iterator->data.g < open.Find(iterator->data.position)->data.g)
					open.Find(iterator->data.position)->data.parent = iterator->data.parent;
		}
		adjacents.list.clear();
	}

	p2DynArray<iPoint> path;
	p2List_item<PathNode>* path_backtracker = close.list.end;

	while (path_backtracker && path_backtracker->data.parent != NULL)
	{
		path.PushBack(path_backtracker->data.position);
		path_backtracker->data = *path_backtracker->data.parent;
	}

	path.Flip();

	return *path.At(0);
}

void j1Pathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;
	
//	if (map)
	//	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

bool j1Pathfinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1Pathfinding::IsWalkable(const iPoint& pos, const bool is_ground) const
{
	uchar t = GetTileAt(pos);
	if (is_ground)
	{
		if (t != 0 || t != 1)
			return true;
		else if (t == 0 || t == 1)
			return false;
	}
	if (t != 0)
		return true;
	else if (t == 0)
		return false;
}

// Utility: return the walkability value of a tile
uint j1Pathfinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return 0;
}

// PathNode methods

//Constructors
PathNode::PathNode()
{}

PathNode::PathNode(int g, int h, const iPoint & pos, const PathNode * parent) : g(g), h(h), position(pos), parent(parent)
{}

PathNode::PathNode(const PathNode & node) : g(node.g), h(node.h), position(node.position), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList & list_to_fill, const bool is_ground) const
{
	iPoint cell;

	// north
	cell.create(position.x, position.y - 1);
	if (App->pathfinding->IsWalkable(cell, is_ground))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(position.x, position.y + 1);
	if (App->pathfinding->IsWalkable(cell, is_ground))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(position.x + 1, position.y);
	if (App->pathfinding->IsWalkable(cell, is_ground))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(position.x - 1, position.y);
	if (App->pathfinding->IsWalkable(cell, is_ground))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

//Calculates the score (F) of a node
int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint & destination)
{
	g = parent->g + 1;
	h = position.DistanceNoSqrt(destination);

	return g + h;
}


// PathList methods
p2List_item<PathNode>* PathList::Find(const iPoint & point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.position == point)
			return item;
		item = item->next;
	}

	return NULL;
}

p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}