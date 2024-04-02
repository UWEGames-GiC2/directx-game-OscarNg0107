#pragma once
#include "CMOGO.h"
class CamTarget :  public CMOGO
{
public:
	CamTarget(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _moveSpeed, float _radius, std::shared_ptr<GameObject> _owner);
	~CamTarget();
	virtual void Tick(GameData* _GD) override;

private:
	GameObject* m_targetObject;
	float m_moveSpeed = 10.0f;
	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;
	float m_radius;
};

