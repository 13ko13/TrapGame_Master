#pragma once
#include "Vec2.h"
#include "Player.h"
#include "Rect.h"

class Shuriken
{
public:
		Shuriken();
	~Shuriken();

	void Init(int graphHandle);
	void Update();
	void Draw();

	bool IsActive() const;
	bool CheckCollision(const Rect& playerRect) const;
	Rect GetRect() const;
	// �v���C���[�̃|�C���^���Z�b�g
	void SetPlayer(Player* player);


private:
	//�n���h���֌W
	int m_handle;		// �藠���̃O���t�B�b�N�n���h��
	int m_fireSeHandle;	//�藠������SE

	//����
	int m_fireSeVolume;	//�藠������SE�̉���

	//�t���O�֌W
	bool m_isActive;	// �藠�����A�N�e�B�u���ǂ���1
	bool m_isFired;	// �藠�������˂��ꂽ���ǂ���

	//�A�j���[�V�����֌W
	int m_animFrame;	// �藠���̃A�j���[�V�����t���[��

	Vec2 m_pos;			// �藠���̈ʒu
	Vec2 m_velocity;	// �藠���̈ړ����x

	Rect m_colRect;		// �藠�������蔻��p�̋�`

	//�v���C���[
	Player* m_pPlayer;	// �v���C���[�̃|�C���^
};

