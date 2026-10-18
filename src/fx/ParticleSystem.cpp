#include "ParticleSystem.hpp"
#include <cstdlib>
#include <cmath>

namespace Starlight {

static Color CONFETTI_COLORS[] = {
    Color{244, 185, 66, 255},  // Gold
    Color{228, 87, 46, 255},   // Coral/Red
    Color{52, 211, 153, 255},  // Green
    Color{96, 165, 250, 255},  // Blue
    Color{244, 114, 182, 255}, // Pink
    Color{167, 139, 250, 255}, // Purple
    Color{255, 255, 255, 255}  // White sparkle
};

ParticleSystem::ParticleSystem(int maxParticles)
    : m_maxParticles(maxParticles) {
    m_particles.reserve(maxParticles);
}

void ParticleSystem::spawnConfetti(float x, float y, int count) {
    for (int i = 0; i < count; ++i) {
        if (static_cast<int>(m_particles.size()) >= m_maxParticles) break;

        Particle p;
        p.position = Vector2{x, y};

        float angle = (float)(rand() % 360) * (3.14159f / 180.0f);
        float speed = 150.0f + (float)(rand() % 350);
        p.velocity = Vector2{cosf(angle) * speed, sinf(angle) * speed - 200.0f};

        p.size = Vector2{8.0f + (float)(rand() % 8), 12.0f + (float)(rand() % 10)};
        p.rotation = (float)(rand() % 360);
        p.angularVelocity = -360.0f + (float)(rand() % 720);

        int colorIdx = rand() % 7;
        p.color = CONFETTI_COLORS[colorIdx];

        p.maxLife = 2.0f + (float)(rand() % 200) / 100.0f;
        p.life = p.maxLife;

        m_particles.push_back(p);
    }
}

void ParticleSystem::spawnConfettiShower(int count) {
    for (int i = 0; i < count; ++i) {
        if (static_cast<int>(m_particles.size()) >= m_maxParticles) break;

        Particle p;
        p.position = Vector2{(float)(rand() % 1280), (float)(-20 - (rand() % 100))};
        p.velocity = Vector2{-40.0f + (float)(rand() % 80), 120.0f + (float)(rand() % 160)};
        p.size = Vector2{8.0f + (float)(rand() % 8), 14.0f + (float)(rand() % 8)};
        p.rotation = (float)(rand() % 360);
        p.angularVelocity = -180.0f + (float)(rand() % 360);

        int colorIdx = rand() % 7;
        p.color = CONFETTI_COLORS[colorIdx];

        p.maxLife = 3.5f + (float)(rand() % 200) / 100.0f;
        p.life = p.maxLife;

        m_particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    const float gravity = 450.0f;
    for (size_t i = 0; i < m_particles.size();) {
        auto& p = m_particles[i];
        p.life -= dt;
        if (p.life <= 0.0f) {
            m_particles.erase(m_particles.begin() + i);
            continue;
        }

        p.velocity.y += gravity * dt;
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.rotation += p.angularVelocity * dt;

        // Slight air wobble
        p.velocity.x *= 0.99f;

        ++i;
    }
}

void ParticleSystem::draw() const {
    for (const auto& p : m_particles) {
        float alpha = p.life / p.maxLife;
        if (alpha > 1.0f) alpha = 1.0f;
        if (alpha < 0.0f) alpha = 0.0f;

        Color c = p.color;
        c.a = (unsigned char)(255.0f * alpha);

        Rectangle dest = {p.position.x, p.position.y, p.size.x, p.size.y};
        Vector2 origin = {p.size.x * 0.5f, p.size.y * 0.5f};
        DrawRectanglePro(dest, origin, p.rotation, c);
    }
}

void ParticleSystem::clear() {
    m_particles.clear();
}

bool ParticleSystem::hasActiveParticles() const {
    return !m_particles.empty();
}

} // namespace Starlight
