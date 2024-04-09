#include "pch.h"
#include "Player.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

Player::Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, float _aspectRatio) : CMOGO(_fileName, _pd3dDevice, _EF) 
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 10.0f;

	SetDrag(0.7);
	SetPhysicsOn(true);

	test = Ray(Vector3(m_pos.x, m_pos.y + 15.0f, m_pos.z), Vector3::Forward);
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GD)
{
	float x = cos(m_pitch) * sin(m_yaw);
	float y = -sin(m_pitch);
	float z = cos(m_pitch) * cos(m_yaw);

	test.position = m_pos;
	test.direction = Vector3(x, y, z);

	//std::cout << test.direction.z << std::endl;

	//switch (_GD->m_GS)
	//{
	//case GS_PLAY_MAIN_CAM:
	//{
	//	//TURN AND FORWARD CONTROL HERE
	//	Vector3 forwardMove = 40.0f * Vector3::Forward;
	//	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	//	forwardMove = Vector3::Transform(forwardMove, rotMove);
	//	if (_GD->m_KBS.W)
	//	{
	//		m_acc += forwardMove;
	//	}
	//	if (_GD->m_KBS.S)
	//	{
	//		m_acc -= forwardMove;
	//	}
	//	break;
	//	{
	//		//MOUSE CONTROL SCHEME HERE
	//		float speed = 10.0f;
	//		m_acc.x += speed * _GD->m_MS.x;
	//		m_acc.z += speed * _GD->m_MS.y;
	//		break;
	//	}
	//}
	//case GS_PLAY_TPS_CAM:
	//{
	//	//TURN AND FORWARD CONTROL HERE
	//	Vector3 forwardMove = 40.0f * Vector3::Forward;
	//	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	//	forwardMove = Vector3::Transform(forwardMove, rotMove);
	//	if (_GD->m_KBS.W)
	//	{
	//		m_acc += forwardMove;
	//	}
	//	if (_GD->m_KBS.S)
	//	{
	//		m_acc -= forwardMove;
	//	}
	//	break;
	//}
	//}

	//FORWARD CONTROL HERE

	Vector3 forwardMove = 40.0f * Vector3::Forward;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	if (_GD->m_KBS.W)
	{
		m_acc += forwardMove;
	}
	if (_GD->m_KBS.S)
	{
		m_acc -= forwardMove;
	}

	//Left/Right CONTROL HERE
	Vector3 rightMove = 40.0f * Vector3::Right;
	//Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	rightMove = Vector3::Transform(rightMove, rotMove);
	if (_GD->m_KBS.D)
	{
		m_acc += rightMove;
	}
	if (_GD->m_KBS.A)
	{
		m_acc -= rightMove;
	}


	//change orinetation of player
	float rotSpeed = -1.0f * _GD->m_dt;
	if(_GD->m_MS.x)
	{
		m_yaw += rotSpeed * _GD->m_MS.x;
	}

	//move player up and down
	if (_GD->m_KBS.R)
	{
		m_acc.y += 40.0f;
	}

	if (_GD->m_KBS.F)
	{
		m_acc.y -= 40.0f;
	}

	//Shoot Projectile
	if(_GD->m_MS.leftButton)
	{
		bool foundProjectile = false;
		for(size_t i =0; i < projectiles.size(); i++)
		{
			if(!projectiles[i]->GetIsActive())
			{
				//float lookPitch = atan2(-(m_FPScamP->GetView()(3, 1)), sqrtf(pow((m_FPScamP->GetView()(3, 2)), 2) + pow((m_FPScamP->GetView()(3, 3)), 2)));
				Vector3 moveDir = (m_CamTargetP->GetPos() - m_FPScamP->GetPos());
				moveDir.Normalize();
				Vector3 forwardOffset = Vector3::Transform(Vector3::Forward, Matrix::CreateRotationY(m_yaw));
				Vector3 upOffset = Vector3::Up * 15;
				projectiles[i]->Fire(this->GetPos() + forwardOffset + upOffset, Vector3::Forward, moveDir.y , this->GetYaw());
				std::cout << moveDir.y << std::endl;
				
			}
		}
	}
	float lookPitch = atan2(-(m_FPScamP->GetView()(3, 1)), sqrtf(pow((m_FPScamP->GetView()(3, 2)), 2) + pow((m_FPScamP->GetView()(3, 3)), 2)));
	//std::cout << lookPitch *180 / (atan(1) * 4) << std::endl;
	//limit motion of the player
	//float length = m_pos.Length();
	//float maxLength = 500.0f;
	//if (length > maxLength)
	//{
	//	m_pos.Normalize();
	//	m_pos *= maxLength;
	//	m_vel *= -0.9; //VERY simple bounce back
	//}

	//apply my base behaviour
	CMOGO::Tick(_GD);
}

void Player::AddCameraChild(std::shared_ptr<FPSCamera> _child)
{
	m_FPScamP = _child;
}


void Player::AddCamTargetChild(std::shared_ptr<CamTarget> _child)
{
	m_CamTargetP = _child;
}