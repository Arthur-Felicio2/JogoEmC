#pragma once
#include "raylib.h"
#include <vector>

namespace Starlight {

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float rotation;
    float angularVelocity;
    Color color;
    float life;
    float maxLife;
};

class ParticleSystem {
public:
    ParticleSystem(int maxParticles = 300);

    void spawnConfetti(float x, float y, int count = 50);
    void spawnConfettiShower(int count = 100);
    void update(float dt);
    void draw() const;
    void clear();

    bool hasActiveParticles() const;

private:
    std::vector<Particle> m_particles;
    int m_maxParticles;
};

} // namespace Starlight
