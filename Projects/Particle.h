#pragma once
#include "Vec2.h"

class Particle
{
public:
	Particle();
	~Particle();

	void Init(const Vec2& pos, const Vec2& velocity, int life);
	void Update();
	void Draw() const;
	bool IsDead() const;


private:
	//���
	int m_life;			//����

	//�t���O
	bool m_active;		//�g�p���t���O

	//�C���X�^���X
	Vec2 m_pos;			//�ʒu
	Vec2 m_velocity;	//���x
};

