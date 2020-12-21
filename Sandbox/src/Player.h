#pragma once

#include "Orange.h"

#include "ParticleSystem.h"


class Player {

public:
	Player();

	void LoadAssets();

	void OnUpdate(Orange::Timestep ts);
	void OnRender();

	glm::vec3& GetPosition() { return m_Position; }
	float GetRotation() { return m_Rotation; }

	void Reset();

private:
	Orange::Ref<Orange::Texture2D> m_ShipTexture;

	float m_Gravity = 0.15f;
	float m_EnginePower = 0.3f;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.5f };
	glm::vec3 m_Velocity = { 5.0f, -m_Gravity, 0.0f };
	float m_Rotation = -90.0f;

	ParticleProps m_SmokeParticle;
	ParticleProps m_FlameParticle;
	ParticleSystem m_ParticleSystem;

	float m_Time = 0.0f;
	float m_SmokeEmitInterval = 0.4f;
	float m_SmokeNextEmitTime = m_SmokeEmitInterval;
};