#pragma once

#include <vector>
#include <memory>
#include "SimpleMath.h"
#include"Tile.h"
#include"GridLocation.h"
#include<unordered_set>


using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace std {
	/* implement hash function so we can put GridLocation into an unordered_set */
	template <>
	struct hash<GridLocation>
	{
		std::size_t operator()(const GridLocation& id) const noexcept {
			// NOTE: better to use something like boost hash_combine
			return std::hash<int>()(id.x ^ (id.y << 16));
		}
	};
}

class grid
{
public:
	grid(float _width, float _depth, int m, int n );
	~grid();
	std::vector<Vector3> m_pos;
	std::vector<Tile> m_gridmap;

	std::vector<Tile> m_walls;

	std::vector<GridLocation>& Neighbours(GridLocation _currentTile);
	//std::pair<int, int> gridLoc = std::make_pair(0, 0);
	std::unordered_set<std::pair<int, int>, std::pair<int, int>> set;

	Vector3 GetCentre() { return m_centre; }
	float GetTileWidth();
	float GetTileDepth();

	
private:
	Vector3 m_centre;

	//std::unordered_set<GridLocation, boost::hash>

};

