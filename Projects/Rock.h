#pragma once
#include "Vec2.h"
#include "Player.h"
#include "Rect.h"

class Rock
{
public:
	Rock();
	~Rock();

	void Init(int graph);
	void End();
	void Update();
	void Draw();
	
	void SetPlayer(Player* player);
	bool IsActive() const;
	bool CheckCollision(const Rect& playerRect) const;
	Rect GetRect() const;


public:
	//�n���h���֌W
	int m_graphHandle;

	//�A�j���[�V�����֌W
	int m_animFrame;

	//�t���O�֌W
	bool m_isActive;

	//�C���X�^���X
	Vec2 m_pos;
	Vec2 m_vel;
	Rect m_colRect;
	Player* m_pPlayer;
};

