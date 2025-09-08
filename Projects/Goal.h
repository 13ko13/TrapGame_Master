#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "DxLib.h"
#include "Player.h"

class Goal
{
public:
	Goal();
	~Goal();

	void Init(int graphHandle);
	void Update();
	void Draw();

	bool IsClear() const;
	Rect GetColRect() const;
	void SetPlayer(Player* player);

private:
	//�n���h���֌W
	int m_handle;		// �S�[���̃O���t�B�b�N�n���h��

	//�A�j���[�V�����֌W
	int m_animFrame;	// �S�[���̃A�j���[�V�����t���[��

	//�ʒu�֌W
	Vec2 m_pos;			// �S�[���̈ʒu

	//�����蔻��֌W
	Rect m_colRect;		// �S�[�������蔻��p�̋�`

	//�v���C���[
	Player* m_pPlayer;	// �v���C���[�̃|�C���^

public:
	//�t���O�֌W
	bool m_isClear;
};