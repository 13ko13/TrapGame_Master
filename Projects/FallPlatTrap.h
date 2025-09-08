#pragma once

#include "Vec2.h"
#include "Rect.h"

class FallPlatTrap
{
public:
		FallPlatTrap();
	~FallPlatTrap();
	void Init(const Vec2& pos,int delayFrames, int graphHandle);
	void Update(const Rect& playerRect);
	void Draw();
	bool IsActive() const;
	Rect GetRect() const;

private:
	void DrawFallPlat(int DrawPosX, int DrawPosY, int posX, int posY);

private:
	Vec2 m_pos;        // �g���b�v�̈ʒu
	int m_delayFrames; // �g���b�v����������܂ł̃t���[����
	int m_currentFrames; // ���݂̃t���[����
	int m_handle;      // �g���b�v�̃O���t�B�b�N�n���h��
	bool m_isActive;   // �g���b�v���A�N�e�B�u���ǂ���
	bool m_playerOnPlatform; // �v���C���[���v���b�g�t�H�[����ɂ��邩�ǂ���
	bool m_isVisible;  // �g���b�v���\������Ă��邩�ǂ���
	Rect m_colRect;    // �����蔻��p�̋�`
};

