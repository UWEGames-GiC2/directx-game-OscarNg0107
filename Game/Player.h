#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "CMOGO.h"
#include "Projectile.h"
#include "FPSCamera.h"
#include "CamTarget.h"

//=================================================================
//Base Player Class (i.e. a GO the player controls)
//=================================================================

class Player : public CMOGO, public std::enable_shared_from_this<Player>
{

public:
	Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _aspectRatio , float _mapTileWidth, float _mapTileDepth);
	~Player();

	virtual void Tick(GameData* _GD) override;

	void AddCameraChild(std::shared_ptr<FPSCamera> _child);
	void AddCamTargetChild(std::shared_ptr<CamTarget> _child);

	std::vector<std::shared_ptr<Projectile>> projectiles;

	std::shared_ptr <FPSCamera> m_FPScamP;

	std::shared_ptr <CamTarget> m_CamTargetP;

	Ray test;
protected:
	float m_gridPosX;
	float m_gridPosY;
	float m_mapTileWidth;
	float m_mapTileDepth;
};

#endif