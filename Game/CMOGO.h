#ifndef _CMOGO_H_
#define _CMOGO_H_

//=================================================================
//A Class for loading and displaying CMO as GameObjects
//=================================================================

#include "gameobject.h"
#include "Model.h"
#include <string>
#include <queue>

using namespace std;
using namespace DirectX;

struct GameData;

class CMOGO : public GameObject
{
public:
	CMOGO(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	virtual ~CMOGO();

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	virtual bool Intersects(const CMOGO& other) const; 
	void AddDestination(Vector3 _destination);
	void MoveTo(Vector3 _destination, float _speed, float _acceptanceRadius);
	bool reachDestination(Vector3 _destination, float _acceptanceRadius);

	void SetMovingSpeed(float _speed) { m_movingSpeed = _speed; }

	BoundingOrientedBox&		getCollider()		noexcept { return m_collider; }
	const BoundingOrientedBox&	getCollider() const noexcept { return m_collider; }

	/*BoundingSphere& getSCollider()		noexcept { return m_SphereCollider; }
	const BoundingSphere& getSCollider() const noexcept { return m_SphereCollider; }*/

protected:
	unique_ptr<Model>  m_model;
	BoundingOrientedBox m_collider;
	/*BoundingSphere m_SphereCollider;*/

	//needs a slightly different raster state that the VBGOs so create one and let them all use it
	static ID3D11RasterizerState*  s_pRasterState;
	static int m_count;
	bool m_canMove = true;
	bool m_isMoving = false;
	bool m_isMoveable = false;
	float m_movingSpeed = 1500.0f;
	//Vector3 m_destination;
	float m_acceptanceRadius = 0.0f;

	std::queue<Vector3> destinations;
};

#endif
