#pragma once
#include "Rect.h"
#include "Vec2.h"

class Rect;
class Bg;
class PlatformManager;

class Character
{
public:
	Character();
	virtual ~Character();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	Vec2 GetPos() const { return m_pos; }
	Rect GetColRect() const { return m_colRect; }

	//PlatformManager���O������Z�b�g�ł���悤�ɂ���
	void SetPlatformManager(PlatformManager* pPlatformManager) 
	{ 
		m_pPlatformMgr = pPlatformManager; 
	}

public:
	//�v���C���[�̌��݂̍s��
	enum class PlayerState
	{
		Idle,
		Walk,
		Shot,
		Jump,
		DoubleJump,
		Fall
	};

	PlayerState m_state;
	Bg* m_pBg;

protected:	
	void Gravity();
	//�}�b�v�`�b�v�Ƃ̓����蔻�菈��
	void CheckHitMap(Rect& chipRect);

protected:
	int m_handle;
	//�v���C���[�̓��͑ҋ@���̃O���t�B�b�N�n���h��
	int m_handleIdle;
	//�v���C���[�̕������͒��̃O���t�B�b�N�n���h��
	int m_handleWalk;
	//�v���C���[�̒e�������͒��̃O���t�B�b�N�n���h��
	int m_handleShot;
	//�v���C���[�̃W�����v���̃O���t�B�b�N�n���h��
	int m_handleJump;
	//�v���C���[�̓�i�W�����v���̃O���t�B�b�N�n���h��
	int m_handleDJump;

	//���E���]���邩
	bool m_isTurn;
	//�n�ʂɒ��n���Ă��邩
	bool m_isGround;

	Vec2 m_pos;		//���W
	Vec2 m_move;	//�ړ�
	Rect m_colRect;	//�����蔻��p�̋�`

	PlatformManager* m_pPlatformMgr; // �v���b�g�t�H�[���̏�ԊǗ�
};