#include "pch.h"
#include "Projectile.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

Projectile::Projectile(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _lifetime, float _speed) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 10.0f;

	SetDrag(0.7);
	SetPhysicsOn(true);

	m_lifetime = _lifetime;
	m_speed = _speed;
}

Projectile::~Projectile()
{
}

void Projectile::Tick(GameData* _GD)
{
	//std::cout << m_vel.x;
	if(GetIsActive())
	{
		currentLifeTime += _GD->m_dt;
		if(currentLifeTime > m_lifetime)
		{
			SetActive(false);
			currentLifeTime = 0.0f;
		}
	}

	CMOGO::Tick(_GD);
}

void Projectile::Fire(Vector3 _startpos, Vector3 _OwnerForwardVector, float _pitch, float _yaw)
{
	
	Vector3 forwardMove = m_speed * _OwnerForwardVector;
	//Matrix rotMove = Matrix::CreateRotationY(_yaw);
	Matrix rotMove = Matrix::CreateFromYawPitchRoll(_yaw, _pitch, 0);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	SetPos(_startpos);
	SetActive(true);
	SetYaw(_yaw);
	SetPitch(_pitch);
	SetDrag(0.01f);
	SetPhysicsOn(true);
	SetAcceleration(forwardMove * 1000.0f);
}
