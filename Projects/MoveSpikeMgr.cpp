#include "MoveSpikeMgr.h"

MoveSpikeMgr::MoveSpikeMgr():
	m_graphHandle(-1)
{
}

MoveSpikeMgr::~MoveSpikeMgr()
{
	// m_spikesの要素は自動的に解放されるため、特に何もしない
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
	m_spikes.clear(); // トゲのリストをクリア
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
	m_spikes.push_back(newSpike); // トゲをリストに追加
}

bool MoveSpikeMgr::CheckCollision(const Rect& playerRect) const
{
	for (const auto& spike : m_spikes)
	{
		Rect spikeRect = spike.GetRect();
		if (playerRect.IsCollision(spikeRect))
		{
			return true; // プレイヤーとトゲが衝突した
		}
	}
	return false; // 衝突なし
}
