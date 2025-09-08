#include "Goal.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	// �S�[���̃T�C�Y
	constexpr float kGoalWidth = 60.0f; // �S�[���̕�
	constexpr float kGoalHeight = 60.0f; // �S�[���̍���

	// �S�[���̈ʒu
	constexpr float kGoalPosX = 1200; // �S�[����X���W
	constexpr float kGoalPosY = 687; // �S�[����Y���W

	// �S�[���̊g�嗦
	constexpr float kScale = 0.6f; // �S�[���̊g�嗦

	//�A�j���[�V�������
	constexpr int kAnimNum = 5; // �S�[���̃A�j���[�V�����R�}��
	constexpr int kAnimWaitFrame = 8; // �A�j���[�V����1�R�}������̃t���[����
}

Goal::Goal() : 
	m_handle(-1),
	m_isClear(false),
	m_animFrame(0),
	m_pos({ kGoalPosX, kGoalPosY }),
	m_pPlayer(nullptr)

{
}

Goal::~Goal()
{
}

void Goal::Init(int graphHandle)
{
	m_pos.x = kGoalPosX;
	m_pos.y = kGoalPosY;
	m_handle = graphHandle;
	m_isClear = false;
	m_animFrame = 0;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kGoalWidth, kGoalHeight); // �����蔻��p�̋�`��ݒ�
}

void Goal::Update()
{
	// �A�j���[�V�����̃t���[�������X�V
	m_animFrame++;
	if (m_animFrame >= kAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0;
	}

	// �v���C���[�Ƃ̓����蔻����`�F�b�N
	if(m_pPlayer && m_colRect.IsCollision(m_pPlayer->GetColRect()))
	{
		m_isClear = true; // �v���C���[���S�[���ɓ��B������N���A�t���O�𗧂Ă�
	}
}

void Goal::Draw()
{
	// �A�j���[�V�����̃t���[��������\���������R�}�ԍ����v�Z�ŋ��߂�
	int animNo = m_animFrame / kAnimWaitFrame;
	// �A�j���[�V�����̐i�s�ɍ��킹�ăO���t�B�b�N�̐؂���ʒu��ύX����
	int srcX = kGoalWidth * animNo;
	int srcY = 0;
	// �S�[���̕`��
	DrawRectRotaGraph(
		m_pos.x, m_pos.y,
		srcX, srcY,
		kGoalWidth, kGoalHeight,
		kScale, 0.0f,
		m_handle,
		true
	);
}

bool Goal::IsClear() const
{
	return m_isClear;
}

Rect Goal::GetColRect() const
{
	return m_colRect;
}

void Goal::SetPlayer(Player* player)
{
	m_pPlayer = player;
}
