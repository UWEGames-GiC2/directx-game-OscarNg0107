#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _aspectRatio) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	
}

Enemy::~Enemy()
{
}

void Enemy::Tick(GameData* _GD)
{
	
}