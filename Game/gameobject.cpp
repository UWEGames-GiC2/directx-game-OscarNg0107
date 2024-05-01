#include "pch.h"
#include "gameobject.h"
#include "GameData.h"

GameObject::GameObject()
{
	//set the Gameobject to the origin with no rotation and unit scaling 
	m_pos = Vector3::Zero;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;
	m_scale = Vector3::One;

	m_worldMat = Matrix::Identity;
	m_fudge = Matrix::Identity;
}

GameObject::~GameObject()
{

}

void GameObject::Tick(GameData* _GD)
{
	if (m_physicsOn)
	{
		Vector3 newVel = m_vel + _GD->m_dt * (m_acc - m_drag*m_vel);
		Vector3 newLocalVel = m_localVel + _GD->m_dt * (m_acc - m_drag * m_localVel);
		Vector3 newPos = m_pos + _GD->m_dt * m_vel;

		m_vel = newVel;
		m_localVel = newLocalVel;
		m_pos = newPos;
	}

	//build up the world matrix depending on the new position of the GameObject
	//the assumption is that this class will be inherited by the class that ACTUALLY changes this
	Matrix  scaleMat = Matrix::CreateScale(m_scale);
	m_rotMat = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll); //possible not the best way of doing this!
	Matrix  transMat = Matrix::CreateTranslation(m_pos);

	m_worldMat = m_fudge * scaleMat * m_rotMat * transMat;

	//zero acceleration ready for the next time round
	m_acc = Vector3::Zero;
}

Vector3 GameObject::GetVectorForward()
{
	/*float x = cos(m_pitch) * sin(m_yaw);
	float y = -sin(m_pitch);
	float z = cos(m_pitch) * cos(m_yaw);

	return Vector3(x, y, z);*/

	return Vector3::Forward;
}

Vector3 GameObject::GetVectorUp()
{

	//float x = sin(m_pitch) * sin(m_yaw);
	//float y = cos(m_pitch);
	//float z = sin(m_pitch) * cos(m_yaw);

	//return Vector3(x, y, z);
	return Vector3::Up;
}

Vector3 GameObject::GetVectorRight()
{

	//float x = cos(m_yaw);
	//float y = 0 ;
	//float z = -sin(m_yaw);

	//return Vector3(x, y, z);
	return Vector3::Right;
}