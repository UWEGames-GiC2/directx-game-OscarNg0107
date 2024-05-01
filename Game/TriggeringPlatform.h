#pragma once
#include "CMOGO.h"
class TriggeringPlatform :public CMOGO
{
public:
	TriggeringPlatform(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, bool _willBackToStart);
	~TriggeringPlatform();
	virtual void Triggered();
	virtual void BackToStart();

	virtual void Tick(GameData* _GD) override;
	virtual void Reset() override;

protected:
	bool m_willBackToStart;
	bool isTriggered = false;


};

