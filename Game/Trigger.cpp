#include "pch.h"
#include "Trigger.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

Trigger::Trigger(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos = _pos;

	m_pitch = _pitch;
	m_yaw = _yaw;
	m_roll = _roll;
	m_scale = _scale;

	GameObject::Tick(nullptr); //update my world_transform

}

Trigger::~Trigger()
{
	
}

void Trigger::OnIntersect()
{
	
		std::cout << "Triggering" << std::endl;
	
}
