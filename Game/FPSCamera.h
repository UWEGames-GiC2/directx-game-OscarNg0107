#pragma once
#include"camera.h"
class FPSCamera : public Camera
{
	public:
		FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _target, Vector3 _up, Vector3 _dpos);
		virtual ~FPSCamera();

		virtual void Tick(GameData * _GD) override;

	protected:
		GameObject* m_targetObject; //I'm following this object
		Vector3	m_dpos; //I'll lurk this far behind and away from it
};

