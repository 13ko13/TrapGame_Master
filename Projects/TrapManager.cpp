#include "TrapManager.h"
#include "PlatformManager.h"
#include "DxLib.h"

TrapManager::TrapManager() :
	m_RtrapGraphHandle(-1),
	m_UtrapGraphHandle(-1),
	m_LtrapGraphHandle(-1),
	m_BtrapGraphHandle(-1)
{
}

TrapManager::~TrapManager()
{
	//Trap�̃O���t�B�b�N��DxLib�ŊǗ�����Ă��邽�߁A�����ł͍폜���Ȃ�o
}

void TrapManager::Init(
	int RtrapGraphHandle, int UtrapGraphHandle,
	int LtrapGraphHandle, int BtrapGraphHandle)
{
	m_RtrapGraphHandle = RtrapGraphHandle;
	m_UtrapGraphHandle = UtrapGraphHandle;
	m_LtrapGraphHandle = LtrapGraphHandle;
	m_BtrapGraphHandle = BtrapGraphHandle;
}

void TrapManager::Update()
{
	for (auto& trap : m_Rtraps)
	{
		trap.Update();
	}

	for (auto& Rtrap : m_Utraps)
	{
		Rtrap.Update();
	}

	for (auto& Ltrap : m_Ltraps)
	{
		Ltrap.Update();
	}

	for (auto& Btrap : m_Btraps)
	{
		Btrap.Update();
	}

	//��A�N�e�B�u�ȃg�Q���폜
	m_Rtraps.erase(
		std::remove_if(m_Rtraps.begin(), m_Rtraps.end(),
			[](const Trap& Rtrap) { return !Rtrap.IsRtrapActive(); }),
		m_Rtraps.end());

	m_Btraps.erase(
		std::remove_if(m_Btraps.begin(), m_Btraps.end(),
			[](const Trap& Btrap) { return !Btrap.IsBtrapActive(); }),
		m_Btraps.end());
}

void TrapManager::Draw()
{
	for (auto& Rtrap : m_Rtraps)
	{
		Rtrap.Draw();
	}

	for (auto& Utrap : m_Utraps)
	{
		Utrap.Draw();
	}

	for (auto& Ltrap : m_Ltraps)
	{
		Ltrap.Draw();
	}

	for (auto& Btrap : m_Btraps)
	{
		Btrap.Draw();
	}
}

void TrapManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void TrapManager::SpawnTrap(
	const Vec2& RtrapPos, const Vec2& UtrapPos,
	const Vec2& LtrapPos, const Vec2& BtrapPos,
	const Vec2& velocity)
{
	Trap newTrap;
	newTrap.Init(
		RtrapPos, UtrapPos,
		LtrapPos, BtrapPos,
		velocity,
		m_RtrapGraphHandle,
		m_UtrapGraphHandle,
		m_LtrapGraphHandle,
		m_BtrapGraphHandle
	);
	newTrap.SetPlayer(m_pPlayer);
	m_Rtraps.push_back(newTrap); // �g���b�v�����X�g�ɒǉ�
	m_Utraps.push_back(newTrap); // ������g���b�v�����l�ɒǉ�
	m_Ltraps.push_back(newTrap); // �������g���b�v�����l�ɒǉ�
	m_Btraps.push_back(newTrap); // �������g���b�v�����l�ɒǉ�
}

bool TrapManager::CheckCollision(const Rect& playerRect)
{
	for (auto& trap : m_Rtraps)
	{
		if (trap.IsRtrapActive() && trap.GetRightRect().IsCollision(playerRect))
		{
			return true; // �v���C���[�ƃg���b�v���Փ�
		}
	}

	for (auto& trap : m_Utraps)
	{
		if (trap.IsRtrapActive() && trap.GetUpRect().IsCollision(playerRect))
		{
			return true; // �v���C���[�ƃg���b�v���Փ�
		}
	}

	for (auto& trap : m_Ltraps)
	{
		if (trap.IsRtrapActive() && trap.GetLeftRect().IsCollision(playerRect))
		{
			return true; // �v���C���[�ƃg���b�v���Փ�
		}
	}

	for (auto& trap : m_Btraps)
	{
		if (trap.IsBtrapActive() && trap.GetBottomRect().IsCollision(playerRect))
		{
			return true; // �v���C���[�ƃg���b�v���Փ�
		}
	}

	return false; // �Փ˂Ȃ�
}


