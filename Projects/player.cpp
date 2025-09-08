#include "Player.h"
#include "DxLib.h"
#include "Character.h"
#include "Shot.h"
#include "Bg.h"
#include "Trap.h"
#include "TrapManager.h"

//定数定義
namespace
{
	const Vec2 kInitPos = { 110.0f,144.0f };	//初期位置

	//グラフィックのサイズ
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 38;

	//アニメーション情報
	constexpr int kIdleAnimNum = 6;
	constexpr int kAnimWaitFrame = 6; //アニメ1コマ当たりのフレーム数
	constexpr int kShotAnimFrame = 10;//弾を撃ってるときのグラフィック表示時間
	constexpr int kJumpAnimNum = 2;

	constexpr float kSpeed = 4.0f;		//移動速度
	constexpr float kJumpPower = 10.0f;	//ジャンプ力

	constexpr float kCharaSize = 32.0f;	//キャラクターサイズ

	//弾の情報
	constexpr int kShotCoolTime = 50;	//弾のクールタイム

	//音量
	constexpr int kJumpSeVolume = 70;	//ジャンプの音量
	constexpr int kShotSeVolume = 70;	//弾発射の音量
}

Player::Player() :
	m_jumpSeHandle(-1),
	m_shotSeHandle(-1),
	m_jumpSeVolume(0),
	m_shotSeVolume(0),
	m_prevInput(0),
	m_jumpNum(0),
	m_isInput(false),
	m_isShotInput(false),
	m_isDoubleJump(false),
	m_animFrame(0),
	m_shotAnimTime(0),
	m_time(0)
{
	m_pos = kInitPos;
}

Player::~Player()
{
}

void Player::Init(int handle, int handleIdle, int handleWalk, int handleShot,
				  int handleJump, int handleDJump)
{
	m_handle = handle;
	m_handleIdle = handleIdle;
	m_handleWalk = handleWalk;
	m_handleShot = handleShot;
	m_handleJump = handleJump;
	m_handleDJump = handleDJump;
	m_pos.x = kInitPos.x;
	m_pos.y = kInitPos.y;
	m_prevInput = 0;
	m_jumpNum = 0;
	m_isTurn = false;
	m_isInput = false;
	m_isDoubleJump = false;
	m_animFrame = 0;
	m_shotAnimTime = 0;
	m_time = 0;

	m_jumpSeHandle = LoadSoundMem("data/Jump.mp3");
	m_shotSeHandle = LoadSoundMem("data/ShotSe.mp3");
	m_trap.SetPlayer(this);
	m_pTrapManager = new TrapManager();
	m_pTrapManager->SetPlayer(this);
}

void Player::End()
{

}

void Player::Update()
{
	m_time++;
	m_shotAnimTime++;
	ShotCT();

	Character::Update();

	Move();
	Jump();

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_2) != 0)//&演算:ビット単位の演算
	{
		if (!m_isShotInput)
		{
			m_isInput = true;
			m_state = PlayerState::Shot;
			m_animFrame = 0;

			//弾発射音
			m_shotSeVolume = kShotSeVolume; //音量を設定
			PlaySoundMem(m_shotSeHandle, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(m_shotSeVolume, m_shotSeHandle);
		}
	}

	if (!m_isInput && m_isGround)
	{
		m_state = PlayerState::Idle;
	}
	else if (!m_isGround)
	{
		m_state = PlayerState::Jump;
	}

	m_animFrame++;

	int animMax = 0;
	switch (m_state)
	{
	case PlayerState::Idle:
		m_handle = m_handleIdle;
		animMax = 6;
		break;
	case PlayerState::Walk:
		m_handle = m_handleWalk;
		animMax = 6;
		break;
	case PlayerState::Shot:
		m_handle = m_handleShot;
		animMax = 3;
		break;
	case PlayerState::Jump:
		m_handle = m_handleJump;
		animMax = 2;
		break;
	case PlayerState::DoubleJump:
		m_handle = m_handleDJump;
		animMax = 2;
		break;
	}

	if (m_animFrame >= animMax * kAnimWaitFrame)
	{
		m_animFrame = 0;
	}

	if (m_isInput && m_shotAnimTime <= kShotAnimFrame)
	{
		return;
	}
	else
	{
		m_isInput = false;
		m_shotAnimTime = 0;
	}
}

void Player::Draw()
{
	Character::Draw();

#ifdef _DEBUG
	//当たり判定を表示
	m_colRect.Draw(0x0000ff, false);
#endif

	//アニメーションのフレーム数から表示したいコマ番号を計算で求める
	int AnimNo = m_animFrame / kAnimWaitFrame;

	//アニメーションの進行に合わせてグラフィックの切り取り位置を変更する
	int srcX = 0;
	int srcY = 0;

	if (m_state == PlayerState::Shot && m_shotAnimTime <= kShotAnimFrame)
	{
		srcX = kGraphWidth * 2;
	}
	else
	{
		srcX = kGraphWidth * AnimNo;
	}

	DrawRectGraph(static_cast<int>(m_pos.x) - kCharaSize * 0.5f,
		static_cast<int>(m_pos.y) - kCharaSize * 0.7f,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		m_handle, true, m_isTurn
	);
}

Shot* Player::CreateShot()
{
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_2) != 0)//&演算:ビット単位の演算
	{
		if (!m_isShotInput)
		{
			m_isShotInput = true;
			Shot* pShot = new Shot();
			pShot->SetInfo(m_pos, !m_isTurn);
			return pShot;
		}
	}
	return nullptr;
}

void Player::Move()
{
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_RIGHT) != 0)	//&演算:ビット単位の演算
	{
		m_move.x = kSpeed;
		m_isInput = true;
		m_state = PlayerState::Walk;
		m_isTurn = false;
	}
	else if ((pad & PAD_INPUT_LEFT) != 0)	//&演算:ビット単位の演算
	{
		m_move.x = -kSpeed;
		m_isInput = true;
		m_state = PlayerState::Walk;
		m_isTurn = true;
	}
	else
	{
		m_move.x = 0.0f;
	}
}

void Player::Jump()
{
	//足元の当たり判定で地面にいるか判定
	Rect footRect;
	footRect.m_left = m_pos.x - kCharaSize * 0.4f;
	footRect.m_right = m_pos.x + kCharaSize * 0.4f;
	footRect.m_top = m_pos.y + kCharaSize * 0.5f;
	footRect.m_bottom = m_pos.y + kCharaSize * 0.5f + 2.0f;

	Rect chipRect;

	////通常地面との判定
	//bool onGround = m_pBg->IsCollision(footRect, chipRect);

	////消えるブロックとの判定
	//if()

	//プレイヤーが地面についたら二段ジャンプを可能にする。
	//& 地面についている判定をtrueにする
	if (m_pBg->IsCollision(footRect,chipRect))	//ここは当たり判定が実装されたら変える
	{
		m_isDoubleJump = false;
		m_isGround = true;
		m_jumpNum = 0; //残りのジャンプ数を0にする
	}

	//ジャンプ中は飛ばす
	if (m_isDoubleJump) return;

	int currentPadInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((currentPadInput & PAD_INPUT_A) && !(m_prevInput & PAD_INPUT_A) && m_jumpNum <= 2)
	{
		m_state = PlayerState::Jump;
		m_jumpNum++;
		m_move.y = -kJumpPower;
		m_isGround = false;
		//ジャンプ音
		m_jumpSeVolume = kJumpSeVolume; //音量を設定
		PlaySoundMem(m_jumpSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_jumpSeVolume, m_jumpSeHandle);
	}
	if (m_jumpNum == 2)
	{
		m_state = PlayerState::DoubleJump;
		m_isDoubleJump = true;
		m_jumpNum = 0;
	}

	//状態を更新
	m_prevInput = currentPadInput;
}

void Player::ShotCT()
{
	if (m_time >= kShotCoolTime)
	{
		m_isShotInput = false;
		m_time = 0;
	}
}