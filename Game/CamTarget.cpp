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
	float speed = -0.1f  * _GD->m_dt;

	m_Yaw = m_targetObject->GetYaw();


	m_Pitch += m_radius * sin(speed * _GD->m_MS.y);

	//limit the max and minimum y rotation of the camera
	if (m_Pitch < -3.0f)
	{
		m_Pitch = -3.0f;
	}
	else if (m_Pitch > -0.05f)
	{
		m_Pitch = -0.05f;
	}

	Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_Yaw, m_Pitch, 0);
	Vector3 offset = Vector3(0.0f, 0.1f, 0.0f);
	Vector3 forwardOffset = 1.0f * m_targetObject->GetVectorForward();
	Matrix rotMove = Matrix::CreateRotationY(m_targetObject->GetYaw());
	forwardOffset = Vector3::Transform(forwardOffset, rotMove);
	Vector3 upOffset = m_targetObject->GetVectorUp() * 15.0f;
	m_pos = m_targetObject->GetPos() + forwardOffset + upOffset + Vector3::Transform(offset, rotCam);

	CMOGO::Tick(_GD);

}