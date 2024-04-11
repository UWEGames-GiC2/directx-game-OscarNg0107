#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF,float _aspectRatio , Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, float _mapTileWidth, float _mapTileDepth) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;

	GameObject::Tick(nullptr); //update my world_transform

	SetDrag(0.7);
	SetPhysicsOn(true);
	m_acc = Vector3(1.0f, 0.0f, 0.0f);

	m_mapTileWidth = _mapTileWidth;
	m_mapTileDepth = _mapTileDepth;
	int m_gridPosX = floor(m_pos.x / m_mapTileWidth);
	int m_gridPosY = floor(m_pos.z / m_mapTileDepth);

	m_gridPos = { m_gridPosX, m_gridPosY };
	
}

Enemy::~Enemy()
{
}

void Enemy::Tick(GameData* _GD)
{
	MoveTo(Vector3(300.0f, 20.0f, 300.0f), 1500.0f, 0.1f);
	CMOGO::Tick(_GD);
}

template<typename Location, typename Graph> std::unordered_map<Location, Location> Enemy::pathfinding(Graph& _graph, Location& _start, Location& _destination)
{
	return Pathfinding::breadth_first_search(_graph, _start, _destination);
}