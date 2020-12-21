#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>


Player::Player() {
	// Smoke Particles
	m_SmokeParticle.Position = { 0.0f, 0.0f, 0.5f };
	m_SmokeParticle.Velocity = { -2.0f, 0.0f };
	m_SmokeParticle.VelocityVariation = { 4.0f, 2.0f };
	m_SmokeParticle.SizeBegin = 0.35f;
	m_SmokeParticle.SizeEnd = 0.0f;
	m_SmokeParticle.SizeVariation = 0.15f;
	m_SmokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };
	m_SmokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_SmokeParticle.LifeTime = 4.0f;

	// Smoke Particles
	m_FlameParticle.Position = { 0.0f, 0.0f, 0.5f };
	m_FlameParticle.Velocity = { -2.0f, 0.0f };
	m_FlameParticle.VelocityVariation = { 3.0f, 1.0f };
	m_FlameParticle.SizeBegin = 0.5f;
	m_FlameParticle.SizeEnd = 0.0f;
	m_FlameParticle.SizeVariation = 0.3f;
	m_FlameParticle.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_FlameParticle.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f , 1.0f };
	m_FlameParticle.LifeTime = 1.0f;
}


void Player::LoadAssets() {
	m_ShipTexture = Orange::Texture2D::Create("assets/textures/Ship.png");
}


void Player::OnUpdate(Orange::Timestep ts) {
	m_Time += ts;

	if (Orange::Input::IsKeyPressed(OG_KEY_SPACE)) {
		m_Velocity.y += m_EnginePower;
		// Increase y velocity if going downwards
		if (m_Velocity.y < 0.0f)
			m_Velocity.y += m_EnginePower * 1.2f;

		// Flames
		glm::vec2 emissionPoint = { 0.0f, -0.6f };
		float rotation = glm::radians(GetRotation());
		glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::vec4(emissionPoint, 0.0f, 1.0f);
		m_FlameParticle.Position = m_Position + glm::vec3(rotated.x, rotated.y, 0.0f);
		m_FlameParticle.Velocity.y = -m_Velocity.y * 0.2f - 0.2f;
		m_ParticleSystem.Emit(m_FlameParticle);
	} else {
		m_Velocity.y -= m_Gravity;
	}

	m_Velocity.y = glm::clamp(m_Velocity.y, -20.0f, 20.0f);
	m_Position += m_Velocity * (float)ts;

	m_Rotation = m_Velocity.y * 4.0f - 90.0f;

	// Smoke
	if (m_Time > m_SmokeNextEmitTime) {
		m_SmokeParticle.Position = m_Position;
		m_ParticleSystem.Emit(m_SmokeParticle);

		m_SmokeNextEmitTime += m_SmokeEmitInterval;
	}

	m_ParticleSystem.OnUpdate(ts);
}


void Player::OnRender() {
	m_ParticleSystem.OnRender();
	Orange::Renderer2D::DrawQuadRotated(m_Position, { 1.0f, 1.3f }, glm::radians(m_Rotation), m_ShipTexture);
}


void Player::Reset() {
	m_Position = { 0.0f, 0.0f, 0.9f };
	m_Velocity = { 5.0f, -m_Gravity, 0.0f };
	m_Rotation = -90.0f;
}