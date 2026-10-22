#include "Shuriken.h"
#include "DxLib.h"
#include "Game.h"

namespace
	{
	// Vec2関係の定数定義
	const Vec2 kInitPos = { 1280.0f, 460.0f }; // 手裏剣の初期位置
	constexpr  float kSpeed = 25.0f; // 手裏剣の移動速度

	// 手裏剣の定数定義
	constexpr float kShurikenSize = 32.0f; // 手裏剣のサイズ
	constexpr float kScale = 1.0f; // 手裏剣の拡大率

	// アニメーション情報
	constexpr int kAnimNum = 8; // 手裏剣のアニメーションフレーム数
	constexpr int kAnimWaitFrame = 2; // 手裏剣のアニメーション1フレームあたりの待機時間

	//音量
	constexpr int kFireSeVolume = 80;	//手裏剣発射時SE音量
}

Shuriken::Shuriken() :
	m_pos({ 0.0f, 0.0f }),
	m_velocity({ 0.0f, 0.0f }),
	m_handle(-1),
	m_isActive(false),
	m_isFired(false),
	m_animFrame(0),
	m_pPlayer(nullptr) 
{
}

Shuriken::~Shuriken()
{
	// グラフィックハンドルの削除はDxLibで行うため、ここでは行わない
}

void Shuriken::Init(int graphHandle)
{
	m_pos.x = kInitPos.x; 
	m_pos.y = kInitPos.y; 
	m_velocity = { kSpeed, 0.0f };
	m_handle = graphHandle;
	m_isActive = false;
	m_isFired = false; 
	m_animFrame = 0;

	//音をロード
	m_fireSeHandle = LoadSoundMem("data/SpikeFireSe.mp3");

	// 当たり判定用の矩形を初期化
	m_colRect.SetCenter(m_pos.x, m_pos.y, kShurikenSize, kShurikenSize);
}

void Shuriken::Update()
{
	if (m_pPlayer->GetPos().x > 400.0f &&
		m_pPlayer->GetPos().y > 360.0f &&
		!m_isFired)
	{
		m_isActive = true; // プレイヤーが特定の位置に到達したら手裏剣をアクティブにする
		m_isFired = true; // 手裏剣が発射されたフラグを立てる

		//手裏剣発射音
		m_fireSeVolume = kFireSeVolume; //音量を設定
		PlaySoundMem(m_fireSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_fireSeVolume, m_fireSeHandle);
	}

	if (!m_isActive) return; // 手裏剣がアクティブでない場合は更新しない

	m_animFrame++;

	// 手裏剣の位置を更新
	m_pos -= m_velocity;

	if(m_animFrame >= kAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0; // アニメーションのフレーム数をリセット
	}

	// 画面外に出たら非アクティブにする
	if (m_pos.x < -kShurikenSize ||
		m_pos.y < -kShurikenSize ||
		m_pos.y > Game::kScreenHeight)
	{
		m_isActive = false;
		return;
	}

	// 当たり判定用の矩形を更新
	m_colRect.SetCenter(m_pos.x, m_pos.y, kShurikenSize, kShurikenSize);
}

void Shuriken::Draw()
{
	if (!m_isActive) return; // 手裏剣がアクティブでない場合は描画しない

	// アニメーションのフレーム数から表示したいコマ番号を計算
	int animNo = m_animFrame / kAnimWaitFrame;

	// アニメーションの進行に合わせてグラフィックの切り取り位置を変更
	int srcX = kShurikenSize * animNo;
	int srcY = 0;

	// 手裏剣の描画
	DrawRectGraph(static_cast<int>(m_pos.x) - kShurikenSize * 0.5f,
		static_cast<int>(m_pos.y) - kShurikenSize * 0.5f,
		srcX, srcY, 
		kShurikenSize, kShurikenSize,
		m_handle, true, false);

#ifdef _DEBUG
	//当たり判定を表示
	m_colRect.Draw(0x0000ff, false);
#endif
}

bool Shuriken::IsActive() const
{
	return m_isActive; // 手裏剣がアクティブかどうかを返す
}

bool Shuriken::CheckCollision(const Rect& playerRect) const
{
	if (m_isActive && this->GetRect().IsCollision(playerRect))
	{
		return true; // プレイヤーと手裏剣が衝突
	}

	return false; // 衝突なし
}

Rect Shuriken::GetRect() const
{
	return m_colRect; // 手裏剣の当たり判定用の矩形を返す
}

void Shuriken::SetPlayer(Player* player)
{
	m_pPlayer = player; // プレイヤーのポインタをセット
}

