#include "j1Pathfinding.h"


// Module Pathfinding methods
j1Pathfinding::j1Pathfinding()
{
	name.create("pathfinding");
}


j1Pathfinding::~j1Pathfinding()
{
}

iPoint j1Pathfinding::GetNextTile(const iPoint & origin, const iPoint & destination)
{
	// check if origin or destination are not walkable and return origin if that's the case

	// check if origin is the same as destination
	if (origin == destination)
		return origin;

	PathList open;
	PathList close;
	PathList adjacents;

	// create a node for the origin and add it to the open list
	PathNode path_origin(0, origin.DistanceManhattan(destination), origin, nullptr);
	open.list.add(path_origin);

	// iterate while there exist tiles in the open list

	while (open.list.start)
	{
		// pass the node in the open list with the lowest score to the closed list
		PathNode temp = open.GetNodeLowestScore()->data;
		close.list.add(temp);
		open.list.del(open.GetNodeLowestScore());

		if (temp.position == destination)
			break;

		// fill adjacents list with the adjacents of the node that is currently being checked
		temp.FindWalkableAdjacents(adjacents, &close.Find(temp.position)->data);
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

// PathNode methods

//Constructors
PathNode::PathNode()
{}

PathNode::PathNode(int g, int h, const iPoint & pos, const PathNode * parent) : g(g), h(h), position(pos), parent(parent)
{}

PathNode::PathNode(const PathNode & node) : g(node.g), h(node.h), position(node.position), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList & list_to_fill, const PathNode* parent) const
{
	iPoint cell;

	// north
	cell.create(position.x, position.y - 1);
	list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	// south
	cell.create(position.x, position.y + 1);
	list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	// east
	cell.create(position.x + 1, position.y);
	list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	// west
	cell.create(position.x - 1, position.y);
	list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	return uint();
}

//Calculates the score (F) of a node
int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint & destination)
{
	g = parent->g + 1;
	h = position.DistanceManhattan(destination);

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