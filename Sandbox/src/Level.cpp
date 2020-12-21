#include "Level.h"

#include "Random.h"

#include <glm/gtc/matrix_transform.hpp>


static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}


static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
	int H = (int)(hsv.x * 360.0f);
	double S = hsv.y;
	double V = hsv.z;

	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	} else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	} else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	} else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	} else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	} else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return { (Rs + m), (Gs + m), (Bs + m), 1.0f };
}


Level::Level() {
	m_Player.LoadAssets();

	m_TriangleTexture = Orange::Texture2D::Create("assets/textures/Triangle.png");

	for (int i = 0; i < 5; i++) {
		CreatePillar(i * 15.0f + 10.0f);
	}
}


void Level::OnUpdate(Orange::Timestep ts) {
	m_Player.OnUpdate(ts);

	float playerXPos = m_Player.GetPosition().x;
	if (playerXPos > m_PillarTarget) {
		CreatePillar(m_PillarIndex * 15.0f + 10.0f);
		m_PillarTarget = m_Pillars[0].TopPosition.x + 50.0f;
		m_Pillars.erase(m_Pillars.begin());
	}

	if (CollisionTest())
		m_GameOver = true;

	m_PillarHSV += 0.05f * (float)ts;
}


void Level::OnRender() {
	// Floor and Ceiling
	glm::vec4 pillarColor = HSVtoRGB(m_PillarHSV);
	Orange::Renderer2D::DrawQuad({ m_Player.GetPosition().x, m_LevelHeight / 2.0f }, m_CeilingFloorSize, pillarColor);
	Orange::Renderer2D::DrawQuad({ m_Player.GetPosition().x, -m_LevelHeight / 2.0f }, m_CeilingFloorSize, pillarColor);

	// Pillars
	for (auto& pillar : m_Pillars) {
		Orange::Renderer2D::DrawQuadRotated(pillar.TopPosition, pillar.TopScale, glm::radians(180.0f), m_TriangleTexture, 1.0f, pillarColor);
		Orange::Renderer2D::DrawQuad(pillar.BottomPosition, pillar.BottomScale, m_TriangleTexture, 1.0f, pillarColor);
	}

	// Player
	m_Player.OnRender();
}


void Level::CreatePillar(float offset) {
	Pillar pillar;
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;

	float center = Random::Float() * 35.0f - 17.5f;
	float gap = 2.0f + Random::Float() * 5.0f;

	pillar.TopPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
	pillar.BottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;

	m_Pillars.push_back(pillar);

	m_PillarIndex++;
}


bool Level::CollisionTest() {
	// 1. Check floor and ceiling
	if (abs(m_Player.GetPosition().y) > m_LevelHeight / 8.0f)
		return true;

	// 2. Check triangles (pillars)
	// Calculate player boundaries
	glm::vec4 playerVertices[4] = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.5f,  0.5f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f, 1.0f}
	};

	glm::vec2 playerPos = m_Player.GetPosition();
	glm::vec4 playerTransformedVertices[4];
	for (int i = 0; i < 4; i++) {
		playerTransformedVertices[i] =
			glm::translate(glm::mat4(1.0f), { playerPos.x, playerPos.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.3f, 1.0f })
			* playerVertices[i];
	}

	for (auto& pillar : m_Pillars) {
		glm::vec2 m_TriangleTransformedVertices[3];

		// Triangle vertices
		glm::vec4 triangleVertices[3] = {
			{-0.4f, -0.4f, 0.0f, 1.0f},
			{ 0.4f, -0.4f, 0.0f, 1.0f},
			{ 0.0f,  0.4f, 0.0f, 1.0f}
		};

		// Calculate top pillar positions and then test for collision
		for (int i = 0; i < 3; i++) {
			m_TriangleTransformedVertices[i] =
				glm::translate(glm::mat4(1.0f), { pillar.TopPosition.x, pillar.TopPosition.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { pillar.TopScale.x, pillar.TopScale.y, 1.0f })
				* triangleVertices[i];
		}

		for (auto& vert_to_check : playerTransformedVertices) {
			if (PointInTri({ vert_to_check.x, vert_to_check.y }, m_TriangleTransformedVertices[0], m_TriangleTransformedVertices[1], m_TriangleTransformedVertices[2])) {
				return true;
			}
		}

		// Calculate bottom pillar positions and then test for collision
		for (int i = 0; i < 3; i++) {
			m_TriangleTransformedVertices[i] =
				glm::translate(glm::mat4(1.0f), { pillar.BottomPosition.x, pillar.BottomPosition.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { pillar.BottomScale.x, pillar.BottomScale.y, 1.0f })
				* triangleVertices[i];
		}

		for (auto& vert_to_check : playerTransformedVertices) {
			if (PointInTri({ vert_to_check.x, vert_to_check.y }, m_TriangleTransformedVertices[0], m_TriangleTransformedVertices[1], m_TriangleTransformedVertices[2])) {
				return true;
			}
		}
	}

	return false;
}


void Level::Reset() {
	m_GameOver = false;

	m_Player.Reset();

	m_PillarTarget = 50.0f;
	m_PillarIndex = 0;
	m_Pillars.clear();
	for (int i = 0; i < 5; i++) {
		CreatePillar(i * 15.0f + 10.0f);
	}
}