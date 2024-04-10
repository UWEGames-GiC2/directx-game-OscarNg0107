#pragma once
#include <vector>
#include <memory>
#include "SimpleMath.h"
#include"GridLocation.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Tile
{
public:
	Tile(int _mapPosX, int _mapPosZ, Vector3 _pos ,Vector3 _scale, float _width, float _depth, bool _reachable);
	~Tile();
	
	GridLocation& GetMapPos() { return m_mapPos; }
	float GetWidth() { return m_width; }
	float GetDepth() { return m_depth; }
	Vector3 GetPos() { return m_pos; }
	Vector3 GetCentre() { return m_centre; }
	bool isReachable() { return m_reachable; }


protected:
	GridLocation m_mapPos;
	Vector3 m_pos;
	Vector3 m_scale;
	Vector3 m_centre;
	float m_width;
	float m_depth;
	bool m_reachable;

};

