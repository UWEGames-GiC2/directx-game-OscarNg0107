#include "pch.h"
#include "CamTarget.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

CamTarget::CamTarget(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _moveSpeed, float _radius, std::shared_ptr<GameObject> _owner) : CMOGO(_fileName, _pd3dDevice, _EF)
{
		m_fudge = Matrix::CreateRotationY(XM_PI);

		m_pos = _owner->GetPos();
		m_targetObject = _owner.get();
		m_moveSpeed = _moveSpeed;
		m_radius = _radius;
		SetActive(false);

		SetScale(Vector3(0.1f, 0.1f, 0.1f));
}

CamTarget::~CamTarget()
{
}

void CamTarget::Tick(GameData* _GD)
{
	float speed = -0.0005f;

	m_Yaw += m_radius *sin(speed * _GD->m_MS.x);


	m_Pitch += m_radius * sin(speed * _GD->m_MS.y);
	if (m_Pitch < -3.0f)
	{
		m_Pitch = -3.0f;
	}
	else if (m_Pitch > -0.05f)
	{
		m_Pitch = -0.05f;
	}

	Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_Yaw, 0, m_Pitch);
	Vector3 offset = Vector3(0.0f, 0.1f, 0.0f);
	Vector3 forwardMove = 2.0f * m_targetObject->GetVectorForward();
	Matrix rotMove = Matrix::CreateRotationY(m_targetObject->GetYaw());
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	m_pos = m_targetObject->GetPos() + forwardMove + m_targetObject->GetVectorUp() * 15.0f + Vector3::Transform(offset, rotCam);

	CMOGO::Tick(_GD);

}