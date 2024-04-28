#include "pch.h"
#include "TriggeringPlatform.h"
TriggeringPlatform::TriggeringPlatform(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, bool _willBacktoStart) :CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
	m_willBackToStart = _willBacktoStart;

	GameObject::Tick(nullptr); //update my world_transform
}

TriggeringPlatform::~TriggeringPlatform()
{
	//Nothing additional here but add this just in case
}

void TriggeringPlatform::Triggered()
{
	if(!isTriggered)
	{
		isTriggered = true;
	}
}

void TriggeringPlatform::BackToStart()
{
	if (isTriggered)
	{
		isTriggered = false;
	}
}

void TriggeringPlatform::Tick(GameData* _GD)
{

	CMOGO::Tick(_GD);
}