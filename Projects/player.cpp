#include "Player.h"
#include "DxLib.h"
#include "Character.h"
#include "Shot.h"
#include "Bg.h"
#include "Trap.h"
#include "TrapManager.h"

//�萔��`
namespace
{
	const Vec2 kInitPos = { 110.0f,144.0f };	//�����ʒu

	//�O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 38;

	//�A�j���[�V�������
	constexpr int kIdleAnimNum = 6;
	constexpr int kAnimWaitFrame = 6; //�A�j��1�R�}������̃t���[����
	constexpr int kShotAnimFrame = 10;//�e�������Ă�Ƃ��̃O���t�B�b�N�\������
	constexpr int kJumpAnimNum = 2;

	constexpr float kSpeed = 4.0f;		//�ړ����x
	constexpr float kJumpPower = 10.0f;	//�W�����v��

	constexpr float kCharaSize = 32.0f;	//�L�����N�^�[�T�C�Y

	//�e�̏��
	constexpr int kShotCoolTime = 50;	//�e�̃N�[���^�C��

	//����
	constexpr int kJumpSeVolume = 70;	//�W�����v�̉���
	constexpr int kShotSeVolume = 70;	//�e���˂̉���
}

Player::Player() :
	m_jumpSeHandle(-1),
	m_shotSeHandle(-1),
	m_jumpSeVolume(0),
	m_shotSeVolume(0),
	m_prevInput(0),
	m_jumpNum(0),
	m_isInput(false),
	m_isShotInput(false),
	m_isDoubleJump(false),
	m_animFrame(0),
	m_shotAnimTime(0),
	m_time(0)
{
	m_pos = kInitPos;
}

Player::~Player()
{
}

void Player::Init(int handle, int handleIdle, int handleWalk, int handleShot,
				  int handleJump, int handleDJump)
{
	m_handle = handle;
	m_handleIdle = handleIdle;
	m_handleWalk = handleWalk;
	m_handleShot = handleShot;
	m_handleJump = handleJump;
	m_handleDJump = handleDJump;
	m_pos.x = kInitPos.x;
	m_pos.y = kInitPos.y;
	m_prevInput = 0;
	m_jumpNum = 0;
	m_isTurn = false;
	m_isInput = false;
	m_isDoubleJump = false;
	m_animFrame = 0;
	m_shotAnimTime = 0;
	m_time = 0;

	m_jumpSeHandle = LoadSoundMem("data/Jump.mp3");
	m_shotSeHandle = LoadSoundMem("data/ShotSe.mp3");
	m_trap.SetPlayer(this);
	m_pTrapManager = new TrapManager();
	m_pTrapManager->SetPlayer(this);
}

void Player::End()
{

}

void Player::Update()
{
	m_time++;
	m_shotAnimTime++;
	ShotCT();

	Character::Update();

	Move();
	Jump();

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_2) != 0)//&���Z:�r�b�g�P�ʂ̉��Z
	{
		if (!m_isShotInput)
		{
			m_isInput = true;
			m_state = PlayerState::Shot;
			m_animFrame = 0;

			//�e���ˉ�
			m_shotSeVolume = kShotSeVolume; //���ʂ�ݒ�
			PlaySoundMem(m_shotSeHandle, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(m_shotSeVolume, m_shotSeHandle);
		}
	}

	if (!m_isInput && m_isGround)
	{
		m_state = PlayerState::Idle;
	}
	else if (!m_isGround)
	{
		m_state = PlayerState::Jump;
	}

	m_animFrame++;

	int animMax = 0;
	switch (m_state)
	{
	case PlayerState::Idle:
		m_handle = m_handleIdle;
		animMax = 6;
		break;
	case PlayerState::Walk:
		m_handle = m_handleWalk;
		animMax = 6;
		break;
	case PlayerState::Shot:
		m_handle = m_handleShot;
		animMax = 3;
		break;
	case PlayerState::Jump:
		m_handle = m_handleJump;
		animMax = 2;
		break;
	case PlayerState::DoubleJump:
		m_handle = m_handleDJump;
		animMax = 2;
		break;
	}

	if (m_animFrame >= animMax * kAnimWaitFrame)
	{
		m_animFrame = 0;
	}

	if (m_isInput && m_shotAnimTime <= kShotAnimFrame)
	{
		return;
	}
	else
	{
		m_isInput = false;
		m_shotAnimTime = 0;
	}
}

void Player::Draw()
{
	Character::Draw();

#ifdef _DEBUG
	//�����蔻���\��
	m_colRect.Draw(0x0000ff, false);
#endif

	//�A�j���[�V�����̃t���[��������\���������R�}�ԍ����v�Z�ŋ��߂�
	int AnimNo = m_animFrame / kAnimWaitFrame;

	//�A�j���[�V�����̐i�s�ɍ��킹�ăO���t�B�b�N�̐؂���ʒu��ύX����
	int srcX = 0;
	int srcY = 0;

	if (m_state == PlayerState::Shot && m_shotAnimTime <= kShotAnimFrame)
	{
		srcX = kGraphWidth * 2;
	}
	else
	{
		srcX = kGraphWidth * AnimNo;
	}

	DrawRectGraph(static_cast<int>(m_pos.x) - kCharaSize * 0.5f,
		static_cast<int>(m_pos.y) - kCharaSize * 0.7f,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		m_handle, true, m_isTurn
	);
}

Shot* Player::CreateShot()
{
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_2) != 0)//&���Z:�r�b�g�P�ʂ̉��Z
	{
		if (!m_isShotInput)
		{
			m_isShotInput = true;
			Shot* pShot = new Shot();
			pShot->SetInfo(m_pos, !m_isTurn);
			return pShot;
		}
	}
	return nullptr;
}

void Player::Move()
{
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_RIGHT) != 0)	//&���Z:�r�b�g�P�ʂ̉��Z
	{
		m_move.x = kSpeed;
		m_isInput = true;
		m_state = PlayerState::Walk;
		m_isTurn = false;
	}
	else if ((pad & PAD_INPUT_LEFT) != 0)	//&���Z:�r�b�g�P�ʂ̉��Z
	{
		m_move.x = -kSpeed;
		m_isInput = true;
		m_state = PlayerState::Walk;
		m_isTurn = true;
	}
	else
	{
		m_move.x = 0.0f;
	}
}

void Player::Jump()
{
	//�����̓����蔻��Œn�ʂɂ��邩����
	Rect footRect;
	footRect.m_left = m_pos.x - kCharaSize * 0.4f;
	footRect.m_right = m_pos.x + kCharaSize * 0.4f;
	footRect.m_top = m_pos.y + kCharaSize * 0.5f;
	footRect.m_bottom = m_pos.y + kCharaSize * 0.5f + 2.0f;

	Rect chipRect;

	////�ʏ�n�ʂƂ̔���
	//bool onGround = m_pBg->IsCollision(footRect, chipRect);

	////������u���b�N�Ƃ̔���
	//if()

	//�v���C���[���n�ʂɂ������i�W�����v���\�ɂ���B
	//& �n�ʂɂ��Ă��锻���true�ɂ���
	if (m_pBg->IsCollision(footRect,chipRect))	//�����͓����蔻�肪�������ꂽ��ς���
	{
		m_isDoubleJump = false;
		m_isGround = true;
		m_jumpNum = 0; //�c��̃W�����v����0�ɂ���
	}

	//�W�����v���͔�΂�
	if (m_isDoubleJump) return;

	int currentPadInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((currentPadInput & PAD_INPUT_A) && !(m_prevInput & PAD_INPUT_A) && m_jumpNum <= 2)
	{
		m_state = PlayerState::Jump;
		m_jumpNum++;
		m_move.y = -kJumpPower;
		m_isGround = false;
		//�W�����v��
		m_jumpSeVolume = kJumpSeVolume; //���ʂ�ݒ�
		PlaySoundMem(m_jumpSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_jumpSeVolume, m_jumpSeHandle);
	}
	if (m_jumpNum == 2)
	{
		m_state = PlayerState::DoubleJump;
		m_isDoubleJump = true;
		m_jumpNum = 0;
	}

	//��Ԃ��X�V
	m_prevInput = currentPadInput;
}

void Player::ShotCT()
{
	if (m_time >= kShotCoolTime)
	{
		m_isShotInput = false;
		m_time = 0;
	}
}