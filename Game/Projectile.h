#pragma once
#include "CMOGO.h"
class Projectile : public CMOGO
{
public:
	Projectile(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _lifetime, float _speed);
	~Projectile();

	virtual void Tick(GameData* _GD) override;

	void Fire(Vector3 _startpos, Vector3 _OwnerForwardVector, float _pitch, float _yaw);
	void SetVelocity(Vector3 _vel) { m_vel = _vel; }
	Vector3 GetVelocity() { return m_vel; }

protected:

	float m_lifetime = 0.0f;
	float currentLifeTime = 0.0f;
	float m_speed = 0.0f;
};

