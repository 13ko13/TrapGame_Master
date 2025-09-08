#include "ParticleMgr.h"
#include <cstdlib>
#include "DxLib.h"

ParticleMgr::ParticleMgr()
{

}
ParticleMgr::~ParticleMgr()
{

}

void ParticleMgr::Spawn(const Vec2& pos, int count)
{
    for (int i = 0; i < count; i++)
    {
        float vx = (rand() % 200 - 100) / 10.0f; // -5.0 ~ 5.0
        float vy = (rand() % 70 - 100) / 10.0f; // -10 ~ -3
        int life = rand() % 60 + 30;           // 30〜90フレーム

        Particle p;
        p.Init(pos, Vec2(vx, vy), life);
        m_particles.push_back(p);
    }
}

void ParticleMgr::Update()
{
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        it->Update();
        if (it->IsDead()) it = m_particles.erase(it);
        else ++it;
    }
}

void ParticleMgr::Draw() const
{
    for (const auto& p : m_particles) {
        p.Draw();
    }
}
