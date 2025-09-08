#include "MoveSpikeMgr.h"

MoveSpikeMgr::MoveSpikeMgr():
	m_graphHandle(-1)
{
}

MoveSpikeMgr::~MoveSpikeMgr()
{
	// m_spikes�̗v�f�͎����I�ɉ������邽�߁A���ɉ������Ȃ�
}

void MoveSpikeMgr::Init(int graphHandle)
{
	m_graphHandle = graphHandle;
}

void MoveSpikeMgr::End()
{
	for (auto& spike : m_spikes)
	{
		spike.End();
	}
	m_spikes.clear(); // �g�Q�̃��X�g���N���A
}

void MoveSpikeMgr::Update()
{
	for (auto& spike : m_spikes)
	{
		spike.Update();
	}
}

void MoveSpikeMgr::Draw()
{
	for (auto& spike : m_spikes)
	{
		spike.Draw();
	}
}

void MoveSpikeMgr::SpawnSpike(const Vec2& pos, const Vec2& velocity)
{
	MoveSpike newSpike;
	newSpike.Init(pos, velocity, m_graphHandle);
	m_spikes.push_back(newSpike); // �g�Q�����X�g�ɒǉ�
}

bool MoveSpikeMgr::CheckCollision(const Rect& playerRect) const
{
	for (const auto& spike : m_spikes)
	{
		Rect spikeRect = spike.GetRect();
		if (playerRect.IsCollision(spikeRect))
		{
			return true; // �v���C���[�ƃg�Q���Փ˂���
		}
	}
	return false; // �Փ˂Ȃ�
}
