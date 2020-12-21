#include "ParticleSystem.h"

#include "Random.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>


ParticleSystem::ParticleSystem() {
	m_ParticlePool.resize(1000);
}


void ParticleSystem::Emit(const ParticleProps& particleProps) {
	Particle& particle = m_ParticlePool[m_PoolIndex];

	particle.Active = true;

	// Position
	particle.Position = particleProps.Position;

	// Rotation
	particle.Rotation = Random::Float() * 2.0f * 3.14f;

	// Velocity
	particle.Velocity = particleProps.Velocity + Random::Float() * (particleProps.VelocityVariation - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	// Size
	particle.SizeBegin = particleProps.SizeBegin + Random::Float() * (particleProps.SizeVariation - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	// Time
	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}


void ParticleSystem::OnUpdate(Orange::Timestep ts) {
	for (auto& particle : m_ParticlePool) {
		if (!particle.Active) {
			continue;
		}
		if (particle.LifeRemaining <= 0.0f) {
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		glm::vec2 posBecauseOfVelocity = particle.Velocity * (float)ts;
		particle.Position += glm::vec3(posBecauseOfVelocity.x, posBecauseOfVelocity.y, 0.0f);
		particle.Rotation += 0.01f * ts;
	}
}


void ParticleSystem::OnRender() {
	for (auto& particle : m_ParticlePool) {
		if (!particle.Active) {
			continue;
		}

		float percentage_factor = (1.0f - particle.LifeRemaining) / particle.LifeTime;
		float size = particle.SizeBegin + (particle.SizeEnd - particle.SizeBegin) * percentage_factor;
		glm::vec4 color = particle.ColorBegin + (particle.ColorEnd - particle.ColorBegin) * percentage_factor;
		Orange::Renderer2D::DrawQuadRotated(particle.Position, { size, size }, particle.Rotation, color);
	}
}
