#pragma once
#include "Vec2.h"
#include "Character.h"
#include "Shot.h"
#include "Bg.h"
#include "PlatformManager.h"
#include "Trap.h"

class Shot;
class TrapManager;

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void Init(int handle,int handleRun, int handleWalk,int handleShot,
					  int handleJump, int handleDJump) ;
	void End();
	virtual void Update() override;
	virtual void Draw()override;
	Shot* CreateShot();

private:
	//�ړ�
	void Move();
	//�W�����v
	void Jump();
	//�e�̃N�[���^�C��
	void ShotCT(); 

private:
	//�n���h��
	int m_jumpSeHandle;		//�W�����v��
	int m_shotSeHandle;		//�e���ˉ�

	//����
	int m_jumpSeVolume;		//�W�����v����
	int m_shotSeVolume;		//�e���ˉ���

	//�����ꂽ�u�Ԃ������Ƃ邽�߂̑O��̓��͏��
	int m_prevInput = 0;
	//���݂̃W�����v������(��i�W�����v�p)
	int m_jumpNum;

	//�v���C���[�����͏�Ԃ��ǂ����̃t���O
	bool m_isInput;
	//�v���C���[���e�𔭎˂��Ă����Ԃ��ǂ����̃t���O
	bool m_isShotInput;
	//�v���C���[����i�W�����v�����Ă��邩�̃t���O
	bool m_isDoubleJump;

	//�A�j���[�V�����֘A
	int m_animFrame;	//�A�j���[�V�����̃t���[����
	int m_shotAnimTime; //Shot��Ԃ��ێ����邽�߂̎��ԃJ�E���g

	//���Ԋ֘A
	int m_time;	//���Ԃ��v��

	//�g���b�v
	Trap m_trap;
	//�g���b�v�}�l�[�W���[
	TrapManager* m_pTrapManager;
};