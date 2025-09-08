#pragma once

#include <vector>
#include "Trap.h"
#include "Player.h"

class Player;

class TrapManager
{
public:
	TrapManager();
	~TrapManager();

	void Init(
		int RtrapGraphHandle,
		int UtrapGraphHandle,
		int LtrapGraphHandle,
		int BtrapGraphHandle);
	void Update();
	void Draw();

	void SetPlayer(Player* pPlayer);
	void SpawnTrap(
		const Vec2& RtrapPos,const Vec2& UtrapPos,
		const Vec2& LtrapPos,const Vec2& BtrapPos,
		const Vec2& velocity);	// �g���b�v�𐶐�
	bool CheckCollision(const Rect& playerRect);			// �v���C���[�Ƃ̏Փ˔���

private:
	std::vector<Trap> m_Rtraps;	// �E�����g���b�v�̃��X�g
	std::vector<Trap> m_Utraps;	// ������g���b�v�̃��X�g
	std::vector<Trap> m_Ltraps;	// �������g���b�v�̃��X�g
	std::vector<Trap> m_Btraps;	// �������g���b�v�̃��X�g
	int m_RtrapGraphHandle;	// �E�����g���b�v�̃O���t�B�b�N�n���h��
	int m_UtrapGraphHandle;	// ������g���b�v�̃O���t�B�b�N�n���h��
	int m_LtrapGraphHandle;	// �������g���b�v�̃O���t�B�b�N�n���h��
	int m_BtrapGraphHandle;	// �������g���b�v�̃O���t�B�b�N�n���h��

	//�v���C���[�̃|�C���^
	Player* m_pPlayer;
};

