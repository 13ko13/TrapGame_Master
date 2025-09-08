#include "Character.h"
#include "Vec2.h"
#include "Player.h"
#include "DxLib.h"
#include "Bg.h"

namespace
{
	constexpr float kCharaSize = 32.0f;	//キャラクターサイズ
	constexpr float kGravity = 0.7f;	//重力
	constexpr float kGround = 144.0f;	//地面位置
}

Character::Character() :
	m_state(PlayerState::Idle),
	m_handle(-1),
	m_handleIdle(-1),
	m_handleWalk(-1),
	m_handleShot(-1),
	m_handleJump(-1),
	m_handleDJump(-1),
	m_isTurn(true),
	m_isGround(true),
	m_pBg(nullptr),
	m_pPlatformMgr(nullptr)
{
}

Character::~Character()
{
	DeleteGraph(m_handle);
}

void Character::Init()
{

}

void Character::Update()
{
	m_isGround = false;

	Gravity();

	Rect chipRect;	//当たったマップチップの矩形
	CheckHitMap(chipRect);

	if (m_isGround)
	{
		m_move.y = 0.0f;
	}
}

void Character::Draw()
{
	float drawX = m_pos.x - kCharaSize * 0.5f;
	float drawY = m_pos.y - kCharaSize * 0.5f;
}

void Character::Gravity()
{
	m_move.y += kGravity;
}

void Character::CheckHitMap(Rect& chipRect)
{
    // 横方向の衝突
    m_pos.x += m_move.x;
    m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize - 1, kCharaSize - 1);

    if (m_pBg->IsCollision(m_colRect, chipRect))
    {
        if (m_move.x > 0.0f)
            m_pos.x = chipRect.GetLeft() - kCharaSize * 0.5f;
        else if (m_move.x < 0.0f)
            m_pos.x = chipRect.GetRight() + kCharaSize * 0.5f;

        m_move.x = 0.0f;
    }

    // 縦方向の衝突
    m_pos.y += m_move.y;
    m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize - 1, kCharaSize - 1);

    bool hitBg = m_pBg->IsCollision(m_colRect, chipRect);
    Rect platRect;
    bool hitPlat = false;

    if (!hitBg && m_pPlatformMgr)
    {
        hitPlat = m_pPlatformMgr->CheckCollision(m_colRect, platRect);
    }

    if (hitBg || hitPlat)
    {
        const Rect& hitRect = hitBg ? chipRect : platRect;

        if (m_move.y > 0.0f) // 下方向（落下）
        {
            m_pos.y = hitRect.GetTop() - kCharaSize * 0.5f;
            m_move.y = 0.0f;
            m_isGround = true;
        }
        else if (m_move.y < 0.0f) // 上方向（ジャンプ中）
        {
            m_pos.y = hitRect.GetBottom() + kCharaSize * 0.5f;
            m_move.y *= -0.1f;
        }
    }

#ifdef _DEBUG
    m_colRect.SetCenter(m_pos.x, m_pos.y, kCharaSize, kCharaSize);
#endif
}
