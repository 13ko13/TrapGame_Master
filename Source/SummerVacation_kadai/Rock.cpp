#include "Rock.h"
#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"

namespace
{
	//グラフィック情報
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;
	//情報
	constexpr float kSpeed = 3.0f;
	constexpr float kFirstPosX = 20.0f;
	constexpr float kFirstPosY = 680.0f;
	//アニメーション情報
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

	//当たり判定用の矩形を初期化
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
	//	//プレイヤーが指定ポジションに到達すると
	//	//アクティブ化する
	//	m_isActive = true;
	//}

	if (!m_isActive) { return; }//非アクティブなら起動しない

	m_animFrame++;

	//岩の位置を更新
	m_pos += m_vel;

	if (m_animFrame >= kAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0; //アニメーションのフレーム数をリセット
	}

	//画面外出たら非アクティブ
	if (m_pos.x > Game::kScreenWidth)
	{
		m_isActive = false;
		return;
	}

	//当たり判定用の矩形を更新
	m_colRect.SetCenter(m_pos.x, m_pos.y, kGraphWidth, kGraphHeight);
}

void Rock::Draw()
{
	if (!m_isActive) return; //非アクティブ時は描画しない

	//表示したいコマ番号を計算
	int animNo = m_animFrame / kAnimWaitFrame;

	//グラフィックの切り取り位置を変更
	int srcX = kGraphWidth * animNo;
	int srcY = 0;

	//岩の描画
	DrawRectGraph(static_cast<int>(m_pos.x),
		static_cast<int>(m_pos.y),
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		m_graphHandle, true, false);

#ifdef _DEBUG
	//当たり判定を表示
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
		return true; //プレイヤーと岩が衝突
	}

	return false; //衝突なし
}

Rect Rock::GetRect() const
{
	return m_colRect; //当たり判定用の矩形を返す
}