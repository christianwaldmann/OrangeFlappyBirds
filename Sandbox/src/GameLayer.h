#pragma once

#include "Orange.h"

#include "Level.h"

#include <imgui/imgui.h>


class GameLayer : public Orange::Layer {

public:
	GameLayer();
	void OnUpdate(Orange::Timestep ts) override;
	void OnEvent(Orange::Event& e) override;
	void OnImGuiRender() override;

	bool OnWindowResize(Orange::WindowResizeEvent e);
	bool OnMouseButtonPressed(Orange::MouseButtonPressedEvent e);

private:
	void CreateCamera(uint32_t width, uint32_t height);

private:
	Orange::Ref<Orange::OrthographicCamera> m_Camera;
	Level m_Level;

	ImFont* m_Font;

	enum class GameState {
		Play = 0, MainMenu = 1, GameOver = 2
	};
	GameState m_State = GameState::MainMenu;

	unsigned int m_Score = 0;

	bool m_Blink = true;

	float m_Time = 0.0f;
};