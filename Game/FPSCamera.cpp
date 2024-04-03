#include "pch.h"
#include "FPSCamera.h"
#include "GameData.h"
#include <iostream>

FPSCamera::FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _target, std::shared_ptr<GameObject> _owner, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target.get();
	m_dpos = _dpos;
	m_owner = _owner.get();
}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::Tick(GameData* _GD)
{
	m_target = m_targetObject->GetPos();
	Vector3 forwardMove = 1.0f * m_owner->GetVectorForward();
	Matrix rotMove = Matrix::CreateRotationY(m_owner->GetYaw());
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	m_pos = m_owner->GetPos() + forwardMove + m_owner->GetVectorUp() * 15.0f;

	//std::cout << "x: " << forwardMove.x << std::endl;
	//std::cout << "y: " << forwardMove.y << std::endl;
	//std::cout << "z: " << forwardMove.z << std::endl;

	//and then set up proj and view matrices
	Camera::Tick(_GD);
}
