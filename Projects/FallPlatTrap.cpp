#include "FallPlatTrap.h"
#include "DxLib.h"
#include "PlatformManager.h"
#include "MoveSpike.h"

namespace
{
	constexpr float kPlatformWidth = 128.0f; // プラットフォームの幅
	constexpr float kPlatformHeight = 16.0f; // プラットフォームの高さ
	//プラットフォームの拡大率
	constexpr float kChipScale = 1.0f;
	//チップ一つの大きさ
	constexpr int kChipWidth = 32;
	constexpr int kChipHeight = 32;
	constexpr int kChipSize = 32;

	//チップを何個置くかの情報
	constexpr int kChipNumX = 2; // 横に置くチップの数
	constexpr int kChipNumY = 1; // 縦に置くチップの数
}

FallPlatTrap::FallPlatTrap() :
	m_pos({ 0.0f, 0.0f }),
	m_delayFrames(0),
	m_currentFrames(0),
	m_handle(-1),
	m_isActive(false),
	m_playerOnPlatform(false)
{
}

FallPlatTrap::~FallPlatTrap()
{
	// グラフィック削除はDxLibで行うため、ここでは何もしない
}

void FallPlatTrap::Init(const Vec2& pos, int delayFrames, int graphHandle)
{
	m_pos = pos;
	m_delayFrames = delayFrames;
	m_currentFrames = 0;
	m_handle = graphHandle;
	m_isActive = true;

	// 当たり判定用の矩形を設定
	m_colRect.SetCenter(m_pos.x + 32.0f , m_pos.y, kPlatformWidth, kPlatformHeight);
}

void FallPlatTrap::Update(const Rect& playerRect)
{
	if (!m_isActive) return; // トラップがアクティブでない場合は更新しない

	// プレイヤーがプラットフォーム上にいるかどうかをチェック
	m_playerOnPlatform = m_colRect.IsCollision(playerRect);

	if (m_playerOnPlatform)
	{
		// プレイヤーがプラットフォーム上にいる場合、フレームをカウント
		m_currentFrames++;
		if (m_currentFrames >= m_delayFrames)
		{
			// 指定されたフレーム数経過後、トラップを非アクティブにする
			m_isActive = false;
		}
	}
	else
	{
		m_currentFrames = 0; // プレイヤーがいない場合はカウントをリセット
	}
	// 当たり判定用の矩形を更新
	m_colRect.SetCenter(m_pos.x + 32.0f , m_pos.y, kPlatformWidth, kPlatformHeight);
}

void FallPlatTrap::Draw()
{
	if (!m_isActive) return; // トラップがアクティブでない場合は描画しない

	int DrawPosX = 1000; // 描画位置X
	int DrawPosY = 300; // 描画位置Y

	// プラットフォームの描画
	//マップチップのとこを表示するか
	//描画
	DrawFallPlat(DrawPosX, DrawPosY, m_pos.x, m_pos.y);
	DrawFallPlat(DrawPosX + 32, DrawPosY, m_pos.x + 32, m_pos.y);
	DrawFallPlat(DrawPosX + 64, DrawPosY, m_pos.x + 64, m_pos.y);
	DrawFallPlat(DrawPosX + 96, DrawPosY, m_pos.x + 96, m_pos.y);
}

bool FallPlatTrap::IsActive() const
{
	return m_isActive;
}

Rect FallPlatTrap::GetRect() const
{
	return m_colRect;
}

void FallPlatTrap::DrawFallPlat(int DrawPosX, int DrawPosY, int posX, int posY)
{
	// プラットフォームの描画
	DrawRectRotaGraph(
		posX - kChipSize * 0.5f,
		posY + kPlatformHeight * 0.5f ,
		0, 0,
		kChipSize, kChipSize,
		kChipScale, 0.0f,
		m_handle, true);

#ifdef _DEBUG
	//当たり判定
	DrawBox(
		posX - kChipWidth ,
		posY - kPlatformHeight * 0.5f,
		posX ,
		posY + kPlatformHeight + 7,
		GetColor(255, 0, 0),
		false);
#endif // DEBUG
}