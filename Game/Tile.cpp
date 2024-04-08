#include "pch.h"
#include "Tile.h"

Tile::Tile(Vector3 _pos, Vector3 _scale, float _width, float _depth, bool _reachable)
{
	m_pos = _pos;
	m_scale = _scale;
	m_width = _width;
	m_depth = _depth;
	m_reachable = _reachable;

	m_centre = Vector3(m_pos.x + m_width / 2, m_pos.y, m_pos.z + m_depth / 2);
}

Tile::~Tile()
{

}

