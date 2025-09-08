#include "Particle.h"
#include "DxLib.h"

Particle::Particle() : 
	m_pos(0,0),
	m_velocity(0,0),
	m_life(0),
	m_active(false)
{
}

Particle::~Particle()
{
}


void Particle::Init(const Vec2& pos, const Vec2& velocity, int life)
{
	m_pos = pos;
	m_velocity = velocity;
	m_life = life;
	m_active = true;
}

void Particle::Update()
{
	if (!m_active) return;

	m_pos += m_velocity;
	m_velocity.y += 0.3f;	//èdóÕ
	m_life--;

	if (m_life <= 0)
	{
		m_active = false;
	}
}

void Particle::Draw() const
{
	if (!m_active) return;

	int color = GetColor(255, 0, 0);	//ê‘
	DrawBox(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_pos.x + 2), static_cast<int>(m_pos.y + 2),
			color, TRUE);
}

bool Particle::IsDead() const
{
	return !m_active;
}

