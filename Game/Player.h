#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "CMOGO.h"
#include "Projectile.h"
#include "FPSCamera.h"
#include "CamTarget.h"
#include"GridLocation.h"

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

	GridLocation& GetGridPos() { return m_gridPos; }

	void SetisFalling(bool _isFalling) { m_isFalling = _isFalling; }

	std::vector<std::shared_ptr<Projectile>> projectiles;

	std::shared_ptr <FPSCamera> m_FPScamP;

	std::shared_ptr <CamTarget> m_CamTargetP;

	void SetCheckPoint(Vector3 _pos) { m_checkpoint = _pos; }
	void SetJumpCount(int _count) { m_jumpCount = _count; }

	void Respawn();

	Ray test;

	void Jump();
protected:
	GridLocation m_gridPos;
	float m_mapTileWidth;
	float m_mapTileDepth;
	bool m_isFalling = true;
	Vector3 m_checkpoint;
	int m_jumpCount = 0;

};

#endif