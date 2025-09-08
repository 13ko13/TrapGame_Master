#include "Goal.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	// ゴールのサイズ
	constexpr float kGoalWidth = 60.0f; // ゴールの幅
	constexpr float kGoalHeight = 60.0f; // ゴールの高さ

	// ゴールの位置
	constexpr float kGoalPosX = 1200; // ゴールのX座標
	constexpr float kGoalPosY = 687; // ゴールのY座標

	// ゴールの拡大率
	constexpr float kScale = 0.6f; // ゴールの拡大率

	//アニメーション情報
	constexpr int kAnimNum = 5; // ゴールのアニメーションコマ数
	constexpr int kAnimWaitFrame = 8; // アニメーション1コマ当たりのフレーム数
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
	m_colRect.SetCenter(m_pos.x, m_pos.y, kGoalWidth, kGoalHeight); // 当たり判定用の矩形を設定
}

void Goal::Update()
{
	// アニメーションのフレーム数を更新
	m_animFrame++;
	if (m_animFrame >= kAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0;
	}

	// プレイヤーとの当たり判定をチェック
	if(m_pPlayer && m_colRect.IsCollision(m_pPlayer->GetColRect()))
	{
		m_isClear = true; // プレイヤーがゴールに到達したらクリアフラグを立てる
	}
}

void Goal::Draw()
{
	// アニメーションのフレーム数から表示したいコマ番号を計算で求める
	int animNo = m_animFrame / kAnimWaitFrame;
	// アニメーションの進行に合わせてグラフィックの切り取り位置を変更する
	int srcX = kGoalWidth * animNo;
	int srcY = 0;
	// ゴールの描画
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
