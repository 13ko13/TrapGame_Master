#pragma once

#include "Rect.h"
#include "Vec2.h"

class Player;

class Trap
{
public:
	Trap();
	~Trap();

	void Init(
		const Vec2& RtrapPos, const Vec2& UtrapPos,
		const Vec2& LtrapPos, const Vec2& BtrapPos,
		const Vec2& velocity, 
		int RgraphHandle, int UgraphHnadle, int LgraphHandle, int BtrapHandle);
	void End();
	void Update();
	void Draw();
	
	void SetPlayer(Player* pPlayer);

	bool IsRtrapActive() const;
	bool IsBtrapActive() const;
	bool IsFireBtrap() ;

	Rect GetRightRect() const;	// �E�����g���b�v�̓����蔻���`
	Rect GetUpRect() const;		// ������g���b�v�̓����蔻���`
	Rect GetLeftRect() const;	// �������g���b�v�̓����蔻���`
	Rect GetBottomRect() const;	// �������g���b�v�̓����蔻���`

private:
	//�n���h���֌W
	int m_Rhandle;			// �g���b�v�̉E�����O���t�B�b�N�n���h��
	int m_Uhandle;			// �g���b�v�̏�����O���t�B�b�N�n���h��
	int m_Lhandle;			// �g���b�v�̍������O���t�B�b�N�n���h��
	int m_BtrapHandle;		// �g���b�v�̉������O���t�B�b�N�n���h��

	//�t���O�֌W
	bool m_isRtrapActive;		// �E�����g���b�v���A�N�e�B�u���ǂ���
	bool m_isBtrapActive;		//�������g���b�v���A�N�e�B�u���ǂ���
	bool m_isBtrapFire;			//�������g���b�v�𔭎˂��邩�ǂ���

	Vec2 m_RtrapPos;        // �E�����g���b�v�̈ʒu
	Vec2 m_UtrapPos;        // ������g���b�v�̈ʒu
	Vec2 m_LtrapPos;        // �������g���b�v�̈ʒu
	Vec2 m_BtrapPos;        // �������g���b�v�̈ʒu
	Vec2 m_velocity;		// �g���b�v�̈ړ����x

	Rect m_RtrapColRect;	// �E�����g���b�v�����蔻��p�̋�`
	Rect m_UtrapColRect;	// ������g���b�v�����蔻��p�̋�`
	Rect m_LtrapColRect;	// �������g���b�v�����蔻��p�̋�`
	Rect m_BtrapColRect;	// �������g���b�v�����蔻��p�̋�`

	Player* m_pPlayer;	//Player�̃|�C���^
};

