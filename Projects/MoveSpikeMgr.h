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

	void SpawnSpike(const Vec2& pos, const Vec2& velocity); // トゲを生成
	bool CheckCollision(const Rect& playerRect) const;	//プレイヤーの当たり判定

private:
	std::vector<MoveSpike> m_spikes; // 移動トゲのリスト
	int m_graphHandle; // トゲのグラフィックハンドル
};

