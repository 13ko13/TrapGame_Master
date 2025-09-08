#include "Rock.h"
#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"

namespace
{
	//�O���t�B�b�N���
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;
	//���
	constexpr float kSpeed = 3.0f;
	constexpr float kFirstPosX = 20.0f;
	constexpr float kFirstPosY = 680.0f;
	//�A�j���[�V�������
	constexpr int kAnimNum = 3;
	constexpr int kAnimWaitFrame = 3;
}

Rock::Rock() :
	m_graphHandle(-1),
	m_animFrame(0),
	m_isActive(true),
	m_pPlayer(nullptr)
{

}

Rock::~Rock()
{

}

void Rock::Init(int graph)
{
	m_graphHandle = graph;
	m_pos = { kFirstPosX,kFirstPosY };
	m_vel = { kSpeed,0 };

	//�����蔻��p�̋�`��������
	m_colRect.SetCenter(m_pos.x, m_pos.y, kGraphWidth, kGraphHeight);
}
void Rock::End()
{

}

void Rock::Update()
{
	//if (m_pPlayer->GetPos().x > 20 &&
	//	m_pPlayer->GetPos().y > 1000 &&
	//	!m_isActive)
	//{
	//	//�v���C���[���w��|�W�V�����ɓ��B�����
	//	//�A�N�e�B�u������
	//	m_isActive = true;
	//}

	if (!m_isActive) { return; }//��A�N�e�B�u�Ȃ�N�����Ȃ�

	m_animFrame++;

	//��̈ʒu���X�V
	m_pos += m_vel;

	if (m_animFrame >= kAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0; //�A�j���[�V�����̃t���[���������Z�b�g
	}

	//��ʊO�o�����A�N�e�B�u
	if (m_pos.x > Game::kScreenWidth)
	{
		m_isActive = false;
		return;
	}

	//�����蔻��p�̋�`���X�V
	m_colRect.SetCenter(m_pos.x, m_pos.y, kGraphWidth, kGraphHeight);
}

void Rock::Draw()
{
	if (!m_isActive) return; //��A�N�e�B�u���͕`�悵�Ȃ�

	//�\���������R�}�ԍ����v�Z
	int animNo = m_animFrame / kAnimWaitFrame;

	//�O���t�B�b�N�̐؂���ʒu��ύX
	int srcX = kGraphWidth * animNo;
	int srcY = 0;

	//��̕`��
	DrawRectGraph(static_cast<int>(m_pos.x),
		static_cast<int>(m_pos.y),
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		m_graphHandle, true, false);

#ifdef _DEBUG
	//�����蔻���\��
	m_colRect.Draw(0x0000ff, false);
#endif
}

void Rock::SetPlayer(Player* player)
{
	m_pPlayer = player;
}

bool Rock::IsActive() const
{
	return m_isActive;
}

bool Rock::CheckCollision(const Rect& playerRect) const
{
	if (m_isActive && this->GetRect().IsCollision(playerRect))
	{
		return true; //�v���C���[�Ɗ₪�Փ�
	}

	return false; //�Փ˂Ȃ�
}

Rect Rock::GetRect() const
{
	return m_colRect; //�����蔻��p�̋�`��Ԃ�
}