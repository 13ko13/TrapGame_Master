#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "DxLib.h"

class MoveSpike
{
public:
	MoveSpike();
	~MoveSpike();

	void Init(const Vec2& pos, const Vec2& velocity, int graphHandle);
	void Update();
	void Draw();
	void End();

	void IsReturn(); // �g�Q���߂�^�C�~���O���ǂ����𔻒肷��
	bool IsActive() const;
	Rect GetRect() const;

private:
	//�n���h���֌W
	int m_handle;		// �g�Q�̃O���t�B�b�N�n���h��

	//�t���O�֌W
	bool m_isActive;	// �g�Q���A�N�e�B�u���ǂ���
	//�ړ��g�Q���㉺�ɓ����^�C�~���O�̃t���O
	bool m_isReturn; // �g�Q���߂�^�C�~���O���ǂ���

	Vec2 m_pos;			// �g�Q�̈ʒu
	Vec2 m_velocity;	// �g�Q�̈ړ����x

	Rect m_colRect;		// �g�Q�����蔻��p�̋�`
};

