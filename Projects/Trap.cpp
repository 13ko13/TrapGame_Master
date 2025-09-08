#include "Trap.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"

namespace
{
	constexpr float kTrapSize = 16.0f; // �g���b�v�̃T�C�Y
	constexpr float kScale = 2.0f; // �g���b�v�̊g�嗦
	constexpr int kRightSpikeIndex = 1; // �E�����g���b�v�̃C���f�b�N�X
	constexpr int kUpSpikeIndex = 0; // ������g���b�v�̃C���f�b�N�X
	constexpr int kLeftSpikeIndex = 3; // �������g���b�v�̃C���f�b�N�X
	constexpr int kBottomSpikeIndex = 2; // �������g���b�v�̃C���f�b�N�X
}

Trap::Trap():
	m_RtrapPos({ 0.0f, 0.0f }),
	m_UtrapPos({ 0.0f, 0.0f }),
	m_LtrapPos({ 0.0f, 0.0f }),
	m_BtrapPos({ 0.0f, 0.0f }),
	m_velocity({ 0.0f, 0.0f }),
	m_Rhandle(-1),
	m_Uhandle(-1),
	m_Lhandle(-1),
	m_BtrapHandle(-1),
	m_isRtrapActive(false),
	m_isBtrapActive(false),
	m_isBtrapFire(false),
	m_pPlayer(nullptr)
{
}

Trap::~Trap()
{
	//�O���t�B�b�N�폜��TrapManager�ōs��
}

void Trap::Init(
	const Vec2& RtrapPos, const Vec2& UtrapPos,
	const Vec2& LtrapPos, const Vec2& BtrapPos,
	const Vec2& velocity,
	int RgraphHandle, int UgraphHandle, int LgraphHandle, int BgraphHandle)
{
	m_RtrapPos = RtrapPos;
	m_UtrapPos = UtrapPos;
	m_LtrapPos = LtrapPos;
	m_BtrapPos = BtrapPos;
	m_velocity = velocity;
	m_Rhandle = RgraphHandle;
	m_Uhandle = UgraphHandle;
	m_Lhandle = LgraphHandle;
	m_BtrapHandle = BgraphHandle;
	m_isRtrapActive = true;
	m_isBtrapActive = true;
	m_isBtrapFire = false;

	m_RtrapColRect.SetCenter(m_RtrapPos.x, m_RtrapPos.y, kTrapSize, kTrapSize);
	m_UtrapColRect.SetCenter(m_UtrapPos.x, m_UtrapPos.y, kTrapSize, kTrapSize);
	m_LtrapColRect.SetCenter(m_LtrapPos.x, m_LtrapPos.y, kTrapSize, kTrapSize);
	m_BtrapColRect.SetCenter(m_BtrapPos.x, m_BtrapPos.y, kTrapSize, kTrapSize);
}

void Trap::End()
{
	m_RtrapPos = { NULL,NULL };
	m_UtrapPos = { NULL,NULL };
	m_LtrapPos = { NULL,NULL };
	m_BtrapPos = { NULL,NULL };
}

void Trap::Update()
{
	if (m_isRtrapActive)
	{
		m_RtrapPos += m_velocity;

		//��ʊO�ɏo�����A�N�e�B�u�ɂ���
		if (m_RtrapPos.x < -kTrapSize || m_RtrapPos.x > Game::kScreenWidth ||
			m_RtrapPos.y < -kTrapSize || m_RtrapPos.y > Game::kScreenHeight)
		{
			m_isRtrapActive = false;
			return;
		}

		m_RtrapColRect.SetCenter(m_RtrapPos.x, m_RtrapPos.y, kTrapSize, kTrapSize);
	}

	if (m_isBtrapActive)
	{
		IsFireBtrap();

		if (m_isBtrapFire)
		{
			m_BtrapPos += m_velocity;
		}

		if (m_BtrapPos.y > Game::kScreenHeight)
		{
			m_isBtrapActive = false;
			return;
		}
		m_BtrapColRect.SetCenter(
			m_BtrapPos.x, m_BtrapPos.y,
			kTrapSize, kTrapSize);
	}
	
	m_UtrapColRect.SetCenter(
		m_UtrapPos.x, m_UtrapPos.y,
		kTrapSize, kTrapSize);

	m_LtrapColRect.SetCenter(
		m_LtrapPos.x, m_LtrapPos.y,
		kTrapSize, kTrapSize);
}

void Trap::Draw()
{
	if (m_isRtrapActive)
	{
		int RtrapSrcX = kTrapSize * kRightSpikeIndex; // �E�����g���b�v�̐؂���ʒu
		int RtrapSrcY = 0;

		DrawRectRotaGraph(
			m_RtrapPos.x, m_RtrapPos.y,
			RtrapSrcX, RtrapSrcY,
			kTrapSize, kTrapSize,
			kScale, 0.0f,
			m_Rhandle,
			true
		);
	}
	
	int UtrapSrcX = kTrapSize * kUpSpikeIndex; // ������g���b�v�̐؂���ʒu
	int UtrapSrcY = 0;

	DrawRectRotaGraph(
		m_UtrapPos.x, m_UtrapPos.y,
		UtrapSrcX, UtrapSrcY,
		kTrapSize, kTrapSize,
		kScale, 0.0f,
		m_Uhandle,
		true
	);

	int LtrapSrcX = kTrapSize * kLeftSpikeIndex; // �������g���b�v�̐؂���ʒu
	int LtrapSrcY = 0;

	DrawRectRotaGraph(
		m_LtrapPos.x, m_LtrapPos.y,
		LtrapSrcX, LtrapSrcY,
		kTrapSize, kTrapSize,
		kScale, 0.0f,
		m_Lhandle,
		true
	);


	if (m_isBtrapActive)
	{
		int BtrapSrcX = kTrapSize * kBottomSpikeIndex; // �������g���b�v�̐؂���ʒu
		int BtrapSrcY = 0;

		DrawRectRotaGraph(
			m_BtrapPos.x, m_BtrapPos.y,
			BtrapSrcX, BtrapSrcY,
			kTrapSize, kTrapSize,
			kScale, 0.0f,
			m_BtrapHandle,
			true
		);
	}
#ifdef _DEBUG
	//�����蔻���\��
	m_RtrapColRect.Draw(0x0000ff, false);
	m_LtrapColRect.Draw(0x0000ff, false);
	m_UtrapColRect.Draw(0x0000ff, false);
	m_BtrapColRect.Draw(0x0000ff, false);
#endif
}

void Trap::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

bool Trap::IsRtrapActive() const
{
	return m_isRtrapActive;
}

bool Trap::IsBtrapActive() const
{
	return m_isBtrapActive;
}

bool Trap::IsFireBtrap()
{
	if (m_pPlayer->GetPos().x < 255 && m_pPlayer->GetPos().y > 500)
	{
		m_isBtrapFire = true;
	}
	return m_isBtrapFire;
}

Rect Trap::GetRightRect() const
{
	return m_RtrapColRect;
}

Rect Trap::GetUpRect() const
{
	return m_UtrapColRect;
}

Rect Trap::GetLeftRect() const
{
	return m_LtrapColRect;
}

Rect Trap::GetBottomRect() const
{
	return m_BtrapColRect;
}