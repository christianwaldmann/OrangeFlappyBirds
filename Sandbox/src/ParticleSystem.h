#pragma once

#include "Orange.h"

#include "Orange.h"


struct ParticleProps {
	glm::vec3 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};


class ParticleSystem {

public:
	ParticleSystem();

	void Emit(const ParticleProps& particleProps);

	void OnUpdate(Orange::Timestep ts);
	void OnRender();

private:
	struct Particle {
		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;
		float SizeBegin, SizeEnd;
		glm::vec4 ColorBegin, ColorEnd;
		glm::vec3 Position;
		glm::vec2 Velocity;
		float Rotation;

		bool Active = false;
	};

	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;
};
