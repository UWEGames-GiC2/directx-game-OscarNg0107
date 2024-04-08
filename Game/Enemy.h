#pragma once
#include "CMOGO.h"
#include "Projectile.h"

class Enemy : public CMOGO
{
public:
	Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _aspectRatio);
	~Enemy();

	virtual void Tick(GameData* _GD) override;


	std::vector<std::shared_ptr<Projectile>> projectiles;

protected:
};

