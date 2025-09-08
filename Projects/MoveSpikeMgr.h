#pragma once

#include "Vec2.h"
#include "Player.h"
#include <vector>
#include "MoveSpike.h"

class MoveSpikeMgr
{
public:
	MoveSpikeMgr();
	~MoveSpikeMgr();

	void Init(int graphHandle);
	void Update();
	void Draw();
	void End();

	void SpawnSpike(const Vec2& pos, const Vec2& velocity); // �g�Q�𐶐�
	bool CheckCollision(const Rect& playerRect) const;	//�v���C���[�̓����蔻��

private:
	std::vector<MoveSpike> m_spikes; // �ړ��g�Q�̃��X�g
	int m_graphHandle; // �g�Q�̃O���t�B�b�N�n���h��
};

