#pragma once

#include "Orange.h"


class Sandbox2D : public Orange::Layer {

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Orange::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Orange::Event& event) override;

private:
	Orange::OrthographicCameraController m_CameraController;

	// TEMP
	Orange::Ref<Orange::VertexArray> m_SquareVA;
	Orange::Ref<Orange::Shader> m_FlatColorShader;

	Orange::Ref<Orange::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
