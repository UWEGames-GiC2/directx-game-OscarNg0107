#include "pch.h"
#include "MovingObjects.h"


MovingObjects::MovingObjects(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, Vector3 _movingOffset) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
	m_isMoveable = true;
	m_willKeepMoving = true;
	m_startPos = m_pos;
	m_endPos = m_startPos+ _movingOffset;
	AddDestination(m_endPos);

	GameObject::Tick(nullptr); //update my world_transform
}

MovingObjects::~MovingObjects()
{

}
	
void MovingObjects::Tick(GameData * _GD)
{

	if (destinations.empty())
	{
		if (m_willKeepMoving)
		{
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
	CMOGO::Tick(_GD);
}