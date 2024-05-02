#pragma once
#include "CMOGO.h"
class MovingObjects :public CMOGO
{
public:
	MovingObjects(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, Vector3 _movingOffset, float _movingSpeed, bool _moveFromStart, bool _willKeepMoving);
	~MovingObjects();
	virtual void Tick(GameData* _GD) override;
	virtual void Reset() override;

	void StartMove() { m_isMoveable = true; }

private:
	bool m_willKeepMoving;
	bool m_reverse = false;
	Vector3 m_endPos;
};

