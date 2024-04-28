#pragma once
#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include "grid.h"
#include "Player.h"
#include "GridLocation.h"
#include "grid.h";

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

namespace std {
	/* implement hash function so we can put GridLocation into an unordered_set */
	template <>
	struct hash<std::vector<int>>
	{
		std::size_t operator()(const std::vector<int>& id) const noexcept {
			// NOTE: better to use something like boost hash_combine
			return std::hash<int>()(id[0] ^ (id[1] << 16));
		}
	};
}

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();
	std::unordered_map<GridLocation, GridLocation> breadth_first_search(grid& _gameMap, GridLocation& _start, GridLocation& _destination);
	std::unordered_map<GridLocation, GridLocation> retrace_path(std::unordered_map<GridLocation, GridLocation>& _closed, GridLocation& _destination, GridLocation& _start);


	std::vector<GridLocation> m_path;
};

#endif
