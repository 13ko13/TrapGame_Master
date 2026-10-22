#include "MoveSpike.h"
#include "DxLib.h"
#include "Game.h"
#include "Vec2.h"
#include "Rect.h"

namespace
{
	constexpr float kSpikeWidth = 48.0f; // トゲの幅
	constexpr float kSpikeHeight = 32.0f; // トゲの高さ
	constexpr float kSpikeSpeed = 2.0f; // トゲの移動速度

	constexpr float kSpikeReturnY = 618.0f; // トゲが戻る位置のY座標
	constexpr float kSpikeMaxY = 523.0f; // トゲが上昇する最大Y座標
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
	m_colRect.SetCenter(m_pos.x, m_pos.y, kSpikeWidth, kSpikeHeight); // 当たり判定用の矩形を設定
}

void MoveSpike::End()
{
	m_pos = { NULL, NULL };
}

void MoveSpike::Update()
{
	//printfDx("%d", m_isActive);
	if (!m_isActive) return; // トゲがアクティブでない場合は更新しない

	IsReturn();

	if (m_pos.y <= kSpikeReturnY && !m_isReturn) // トゲの位置を更新
	{
		m_pos += m_velocity * kSpikeSpeed;
	}
	else if (m_pos.y >= kSpikeMaxY && m_isReturn)
	{
		m_pos.y -= m_velocity.y * kSpikeSpeed; // 位置を戻す
	}

	// 当たり判定用の矩形を更新
	m_colRect.SetCenter(m_pos.x, m_pos.y, kSpikeWidth, kSpikeHeight);
}

void MoveSpike::Draw()
{
	//if (!m_isActive) return; // トゲがアクティブでない場合は描画しない
	// トゲの描画
	DrawRotaGraph(
		static_cast<int>(m_pos.x),
		static_cast<int>(m_pos.y),
		1.5f, 0.0f,
		m_handle, true);

#ifdef _DEBUG
	//当たり判定を表示
	m_colRect.Draw(0x0000ff, false);
#endif
}

bool MoveSpike::IsActive() const
{
	return m_isActive;
}

Rect MoveSpike::GetRect() const
{
	return m_colRect; // 当たり判定用の矩形を返す
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