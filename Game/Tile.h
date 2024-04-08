#pragma once
#include <vector>
#include <memory>
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Tile
{
public:
	Tile( Vector3 _pos ,Vector3 _scale, float _width, float _depth, bool _reachable);
	~Tile();

	float GetWidth() { return m_width; }
	float GetDepth() { return m_depth; }
	Vector3 GetPos() { return m_pos; }
	Vector3 GetCentre() { return m_centre; }
	bool isReachable() { return m_reachable; }


protected:
	Vector3 m_pos;
	Vector3 m_scale;
	Vector3 m_centre;
	float m_width;
	float m_depth;
	bool m_reachable;

};

