#pragma once
#include "Vec2.h"
#include "Player.h"

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update();
	void Draw();

	void SetInfo(const Vec2& pos, bool isTurn);

	Vec2 GetPos() const { return m_pos; };
	Rect GetColRect() const { return m_colRect; }
private:
	int m_handle;	//�O���t�B�b�N�n���h��

	Vec2 m_pos;		//�e�̈ʒu
	Vec2 m_move;	//�ړ���
	Rect m_colRect;	//�����蔻��p�̋�`
};

