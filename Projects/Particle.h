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
	//情報
	int m_life;			//寿命

	//フラグ
	bool m_active;		//使用中フラグ

	//インスタンス
	Vec2 m_pos;			//位置
	Vec2 m_velocity;	//速度
};

