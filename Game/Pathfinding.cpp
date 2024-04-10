#include "pch.h"
#include "Pathfinding.h"


Pathfinding :: Pathfinding()
{
}

Pathfinding::~Pathfinding()
{

}

template<typename Location, typename Graph> std::unordered_map<Location, Location> Pathfinding::breadth_first_search(Graph& _graph, Location& _start, Location& _destination)
{
	std::queue<Location> frontier;
	frontier.push(_start);

	std::unordered_map<Location, Location> came_from;
	came_from[_start] = _start;

	while(!frontier.empty())
	{
		Location current = frontier.front();
		frontier.pop();

		if(current == _destination)
		{
			break;
		}

		for(Location next : _graph.Neighbours(current))
		{
			if(came_from.find(next) == came_from.end())
			{
				frontier.push(next);
				came_from[next] = current;
				//std::cout << "{" << current[0] << " ," << current[1] << " }" << std::endl;
			}
		}
	}
	return came_from;
}