#pragma once
#include "TriggeringPlatform.h"
class TriggeringDoor : public TriggeringPlatform
{
public:
	TriggeringDoor(ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, bool _willBackToStart);
	~TriggeringDoor();
	virtual void Triggered() override;
	virtual void BackToStart() override;

	virtual void Tick(GameData* _GD) override;

private:
	Vector3 m_startPos;

};

