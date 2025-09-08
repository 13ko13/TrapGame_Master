#pragma once
#include <vector>
#include "Particle.h"
#include "Vec2.h"

class ParticleMgr
{
public:
    ParticleMgr();
    ~ParticleMgr();

    void Spawn(const Vec2& pos, int count = 1000);
    void Update();
    void Draw() const;

private:
    std::vector<Particle> m_particles;
};
