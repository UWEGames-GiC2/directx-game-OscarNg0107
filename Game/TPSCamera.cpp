#include "pch.h"
#include "TPSCamera.h"
#include "GameData.h"
#include <iostream>

TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target.get();
	m_dpos = _dpos;
}

TPSCamera::~TPSCamera()
{

}

void TPSCamera::Tick(GameData* _GD)
{
	//Set up position of camera and target position of camera based on new position and orientation of target object

	if (_GD->m_GS == GS_PLAY_TPS_CAM)
		{
			float speed = -0.0005f;

			m_camYaw += sin(speed * _GD->m_MS.x) * m_dpos.z;

			m_camPitch += sin(speed * _GD->m_MS.y) * m_dpos.z;

			std::cout << m_camPitch << std::endl;
			if(m_camPitch < -1.3f)
			{
				m_camPitch = -1.3f;
			}
			else if(m_camPitch > 1.5f)
			{
				m_camPitch = 1.5f;
			}
		}

	else 
	{
		m_camYaw = m_targetObject->GetYaw();
	}
	Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_camYaw, m_camPitch, 0.0f);
	Vector3 offset = Vector3(0.0f, 20.0f, 0.0f);
	m_target = m_targetObject->GetPos() + offset;
	m_pos = m_target + Vector3::Transform(m_dpos, rotCam);

	

	//and then set up proj and view matrices
	Camera::Tick(_GD);
}

