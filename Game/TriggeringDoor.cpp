#include "pch.h"
#include "TriggeringDoor.h"
#include <iostream>
TriggeringDoor::TriggeringDoor(ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, bool _willBacktoStart) :TriggeringPlatform("cube2", _pd3dDevice, _EF, _pos, _pitch, _yaw, _roll, _scale, _willBacktoStart)
{
	m_isMoveable = true;
	
	GameObject::Tick(nullptr); //update my world_transform
	SetDrag(0.7);
	SetPhysicsOn(true);
	m_acc = Vector3(1.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 4.0f, 4.0f);
}

TriggeringDoor::~TriggeringDoor()
{

}

void TriggeringDoor::Triggered()
{
	AddDestination(Vector3(m_startPos.x, m_startPos.y + 100, m_startPos.z));
}

void TriggeringDoor::BackToStart()
{
	AddDestination(m_startPos);
}

void TriggeringDoor::Tick(GameData* _GD)
{
	TriggeringPlatform::Tick(_GD);
}