#include "pch.h"
#include "Pathfinding.h"


Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{

}

//std::unordered_map<GridLocation, GridLocation> Pathfinding::breadth_first_search(grid& _gameMap, GridLocation& _startCoord, GridLocation& _endCoord)
//{
	//empty the path

	//m_path.clear();

	//std::queue<GridLocation> open;
	//open.push(_startCoord);
	//std::unordered_map<GridLocation,GridLocation> came_from;
	//bool foundpath = false;
	//GridLocation currentCoord = open.front();

	//while(open.size() > 0)
	//{
	//	currentCoord = open.front();
	//	open.pop();
	//	came_from.insert({ currentCoord, currentCoord });

	//	// if reacdh destination
	//	if(currentCoord.x == _endCoord.x && currentCoord.y == _endCoord.y)
	//	{
	//		foundpath = true;
	//		break;
	//	}

	//	
	//	for(GridLocation next : _gameMap.Neighbours(currentCoord))
	//	{
	//		if(came_from.find(next) == came_from.end())
	//		{
	//			open.push(next);
	//			
	//		}
	//	}
	//}
	//if(foundpath)
	//{

	//}
	//else
	//{
	//	
	//}
	

	//std::queue<Location> frontier;
	//frontier.push(_start);

	//std::unordered_map<Location, Location> came_from;
	//came_from[_start] = _start;

	//while(!frontier.empty())
	//{
	//	Location current = frontier.front();
	//	frontier.pop();

	//	if(current == _endCoord)
	//	{
	//		break;
	//	}

	//	for(Location next : _graph.Neighbours(current))
	//	{
	//		if(came_from.find(next) == came_from.end())
	//		{
	//			frontier.push(next);
	//			came_from[next] = current;
	//			//std::cout << "{" << current[0] << " ," << current[1] << " }" << std::endl;
	//		}
	//	}
	//}
	//return came_from;
//}

//std::unordered_map<GridLocation, GridLocation> retrace_path(std::unordered_map<GridLocation, GridLocation>& _closed, GridLocation& _destination, GridLocation& _start)
//{
//	GridLocation currentCoord = _destination;
//	
//	//while(currentCoord )
//}