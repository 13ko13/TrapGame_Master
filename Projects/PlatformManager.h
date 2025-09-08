#pragma once
#include <vector>
#include "FallPlatTrap.h"
#include "Rect.h"

class PlatformManager
{
public:
	PlatformManager();
	~PlatformManager();

	void Init(int graphHandle);
	void Update(const Rect& plaerRect);
	void Draw();
	void SpawnPlatform(const Vec2& pos, int delayFrames);
	bool CheckCollision(const Rect& colRect, Rect& outPlatRect) const;
private:
	std::vector<FallPlatTrap> m_platforms;
	int m_graphHandle;
};

