#pragma once

#include "Orange.h"

#include "Player.h"
#include <vector>


struct Pillar {
	glm::vec3 TopPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec2 TopScale = { 15.0f, 20.0f };

	glm::vec3 BottomPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec2 BottomScale = { 15.0f, 20.0f };
};


class Level {

public:
	Level();

	void OnUpdate(Orange::Timestep ts);
	void OnRender();

	Player GetPlayer() { return m_Player; }

	void CreatePillar(float offset);

	bool CollisionTest();

	void Reset();

	const bool IsGameOver() const { return m_GameOver; }

private:
	Player m_Player;

	glm::vec3 m_LevelEdgeTopPosition = { 0.0f, 34.0f, 0.0f };
	glm::vec3 m_LevelEdgeBottomPosition = { 0.0f, -34.0f, 0.0f };
	glm::vec2 m_LevelEdgeSize = { 100.0f, 2.0f };
	float m_LevelHeight = 68.0f;
	glm::vec2 m_CeilingFloorSize = { 50.0f, 50.0f };

	Orange::Ref<Orange::Texture2D> m_TriangleTexture;
	std::vector<Pillar> m_Pillars;
	unsigned int m_PillarIndex = 0;
	float m_PillarTarget = 50.0f;

	glm::vec3 m_PillarHSV = { 0.0f, 0.8f, 0.8f };

	bool m_GameOver = false;
};