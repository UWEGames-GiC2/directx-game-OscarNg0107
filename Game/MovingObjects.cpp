#include "pch.h"
#include "MovingObjects.h"
#include <iostream>

MovingObjects::MovingObjects(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, Vector3 _movingOffset, float _movingSpeed, bool _willKeepMoving) : CMOGO(_fileName, _pd3dDevice, _EF, _pos)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
	m_isMoveable = true;
	m_willKeepMoving = _willKeepMoving;
	m_endPos = m_startPos+ _movingOffset;
	m_movingSpeed = _movingSpeed;
	AddDestination(m_endPos);

	GameObject::Tick(nullptr); //update my world_transform
}

MovingObjects::~MovingObjects()
{

}
	
void MovingObjects::Tick(GameData * _GD)
{
	CMOGO::Tick(_GD);
	//std::cout << destinations.size() << std::endl;;
	if (destinations.empty())
	{
		if (m_willKeepMoving)
		{
			std::cout << "hi" << std::endl;
			m_reverse = !m_reverse;
			if (!m_reverse)
			{
				
				AddDestination(m_endPos);
			}
			else
			{
				AddDestination(m_startPos);
			}
		}
	}
}