#include "GameLayer.h"

#include <imgui/imgui.h>


GameLayer::GameLayer() : Layer("GameLayer") {
	auto& window = Orange::Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	ImGuiIO& io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0);
}


void GameLayer::OnUpdate(Orange::Timestep ts) {
	// Update
	m_Time += ts;

	if (m_Time - (float)int(m_Time) > 0.4)
		m_Blink = false;
	else
		m_Blink = true;

	m_Score = (int)(m_Level.GetPlayer().GetPosition().x + 20) / 15 - 1;

	if (m_Level.IsGameOver()) {
		m_State = GameState::GameOver;
	}

	if (m_State == GameState::Play) {
		m_Level.OnUpdate(ts);
	}
	const auto& playerPosition = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPosition.x, playerPosition.y, 0.0f });

	// Rendering
	Orange::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1 });
	Orange::RenderCommand::Clear();

	Orange::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Orange::Renderer2D::EndScene();
}


void GameLayer::OnEvent(Orange::Event& e) {
	Orange::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Orange::WindowResizeEvent>(OG_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<Orange::MouseButtonPressedEvent>(OG_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}


void GameLayer::OnImGuiRender() {
	switch (m_State) {
	case GameState::Play:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Orange::Application::Get().GetWindow().GetWidth();
		auto height = Orange::Application::Get().GetWindow().GetHeight();
		pos.x += width - 250.0f;
		pos.y += 0.0 * height;
		char scoreDisplayText[20];
		snprintf(scoreDisplayText, sizeof(scoreDisplayText), "Score: %d", m_Score);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 46.0f, pos, 0xffffffff, scoreDisplayText);
		break;
	}
	case GameState::MainMenu:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Orange::Application::Get().GetWindow().GetWidth();
		auto height = Orange::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
		break;
	}
	case GameState::GameOver:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Orange::Application::Get().GetWindow().GetWidth();
		auto height = Orange::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
		pos.x += 200.0f;
		pos.y += 150.0f;
		char scoreDisplayText[20];
		snprintf(scoreDisplayText, sizeof(scoreDisplayText), "Score: %d", m_Score);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 46.0f, pos, 0xffffffff, scoreDisplayText);
		break;
	}
	}
}


bool GameLayer::OnWindowResize(Orange::WindowResizeEvent e) {
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}


bool GameLayer::OnMouseButtonPressed(Orange::MouseButtonPressedEvent e) {
	if (m_State == GameState::GameOver)
		m_Level.Reset();

	m_State = GameState::Play;
	return false;
}


void GameLayer::CreateCamera(uint32_t width, uint32_t height) {
	float aspectRatio = (float)width / (float)height;

	float CamSize = 8.0f;

	float bottom = -CamSize;
	float top = CamSize;
	float left = (-CamSize) * aspectRatio;
	float right = CamSize * aspectRatio;

	m_Camera = Orange::CreateRef<Orange::OrthographicCamera>(left, right, bottom, top);
}
