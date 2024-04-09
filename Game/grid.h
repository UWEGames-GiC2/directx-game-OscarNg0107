#pragma once

#include <vector>
#include <memory>
#include "SimpleMath.h"
#include"Tile.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class grid
{
public:
	grid(float _width, float _depth, int m, int n );
	~grid();
	std::vector<Vector3> m_pos;
	std::vector<Tile> m_gridmap;

	std::vector<Tile> m_walls;

	Vector3 GetCentre() { return m_centre; }
	float GetTileWidth();
	float GetTileDepth();

	
private:
	Vector3 m_centre;

};

