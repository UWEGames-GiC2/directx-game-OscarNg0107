#pragma once
#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include "grid.h"
#include "Player.h"
#include "GridLocation.h"

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>


class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();
	template<typename Location, typename Graph> std::unordered_map<Location, Location> breadth_first_search(Graph& _graph, Location& _start, Location& _destination);
};

#endif
