#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "CMOGO.h"
#include "Projectile.h"
#include "Pathfinding.h"
#include "GridLocation.h"

class Enemy : public CMOGO, public Pathfinding
{
public:
	Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _aspectRatio, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, float _mapTileWidth, float _mapTileDepth);
	~Enemy();

	virtual void Tick(GameData* _GD) override;

	template<typename Location, typename Graph> std::unordered_map<Location, Location> pathfinding(Graph& _graph, Location& _start, Location& _destination);


	std::vector<std::shared_ptr<Projectile>> projectiles;
	GridLocation m_gridPos;
protected:
	
	float m_mapTileWidth;
	float m_mapTileDepth;
};

#endif

