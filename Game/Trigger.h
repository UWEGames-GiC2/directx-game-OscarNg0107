#pragma once
#include "CMOGO.h"
#include "TriggeringPlatform.h"
enum TiggerType{
	
};
class Trigger : public CMOGO
{
public:
	Trigger(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale);
	~Trigger();
	bool OnIntersect();
	void OnIntersect(TriggeringPlatform& _platforms);
	virtual void Tick(GameData* _GD) override { _GD; };

protected:

};