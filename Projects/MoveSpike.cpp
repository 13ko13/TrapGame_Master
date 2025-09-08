#include "MoveSpike.h"
#include "DxLib.h"
#include "Game.h"
#include "Vec2.h"
#include "Rect.h"

namespace
{
	constexpr float kSpikeWidth = 48.0f; // �g�Q�̕�
	constexpr float kSpikeHeight = 32.0f; // �g�Q�̍���
	constexpr float kSpikeSpeed = 2.0f; // �g�Q�̈ړ����x

	constexpr float kSpikeReturnY = 618.0f; // �g�Q���߂�ʒu��Y���W
	constexpr float kSpikeMaxY = 523.0f; // �g�Q���㏸����ő�Y���W
}

MoveSpike::MoveSpike() :
	m_handle(-1),
	m_isActive(false),
	m_isReturn(false),
	m_pos({ 0.0f, 0.0f }),
	m_velocity({ 0.0f, 0.0f })
{
}

MoveSpike::~MoveSpike()
{
}

void MoveSpike::Init(const Vec2& pos, const Vec2& velocity, int graphHandle)
{
	m_handle = graphHandle;
	m_isActive = true;
	m_isReturn = false;
	m_pos = pos;
	m_velocity = velocity;
	m_colRect.SetCenter(m_pos.x, m_pos.y, kSpikeWidth, kSpikeHeight); // �����蔻��p�̋�`��ݒ�
}

void MoveSpike::End()
{
	m_pos = { NULL, NULL };
}

void MoveSpike::Update()
{
	//printfDx("%d", m_isActive);
	if (!m_isActive) return; // �g�Q���A�N�e�B�u�łȂ��ꍇ�͍X�V���Ȃ�

	IsReturn();

	if (m_pos.y <= kSpikeReturnY && !m_isReturn) // �g�Q�̈ʒu���X�V
	{
		m_pos += m_velocity * kSpikeSpeed;
	}
	else if (m_pos.y >= kSpikeMaxY && m_isReturn)
	{
		m_pos.y -= m_velocity.y * kSpikeSpeed; // �ʒu��߂�
	}

	// �����蔻��p�̋�`���X�V
	m_colRect.SetCenter(m_pos.x, m_pos.y, kSpikeWidth, kSpikeHeight);
}

void MoveSpike::Draw()
{
	//if (!m_isActive) return; // �g�Q���A�N�e�B�u�łȂ��ꍇ�͕`�悵�Ȃ�
	// �g�Q�̕`��
	DrawRotaGraph(
		static_cast<int>(m_pos.x),
		static_cast<int>(m_pos.y),
		1.5f, 0.0f,
		m_handle, true);

#ifdef _DEBUG
	//�����蔻���\��
	m_colRect.Draw(0x0000ff, false);
#endif
}

bool MoveSpike::IsActive() const
{
	return m_isActive;
}

Rect MoveSpike::GetRect() const
{
	return m_colRect; // �����蔻��p�̋�`��Ԃ�
}

void MoveSpike::IsReturn()
{
	if(m_pos.y > kSpikeReturnY )
	{
		m_isReturn = true; 
	}
	else if (m_pos.y <= kSpikeMaxY)
	{
		m_isReturn = false; 
	}
}