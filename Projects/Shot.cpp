#include "DxLib.h"
#include "Shot.h"
#include "Player.h"
#include "Character.h"

namespace
{
	constexpr float kSpeed = 5.0f;	//’e‚ÌˆÚ“®‘¬“x
}

Shot::Shot() :
	m_pos({ 0,0 }),
	m_handle(-1),
	m_move({ kSpeed,0 })
{
	m_handle = LoadGraph("data/Bullet.gif");
}

Shot::~Shot()
{
	DeleteGraph(m_handle);
}

void Shot::Init()
{
}

void Shot::Update()
{
	m_pos += m_move;
}

void Shot::Draw()
{
	if (m_move.x > 0)
	{
		//‰EŒü‚«:’Êí•`‰æ
		DrawGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y) - 7, m_handle, true);
	}
	else
	{
		//¶Œü‚«:¶‰E”½“]•`‰æ
		DrawTurnGraph(static_cast<int>(m_pos.x) - 14, static_cast<int>(m_pos.y) - 7, m_handle, true);
	}
}

void Shot::SetInfo(const Vec2& pos, bool isTurn)
{
	m_pos = pos;

	if (isTurn)
	{
		m_move.x = kSpeed;
	}
	else
	{
		m_move.x = -kSpeed;
	}
}