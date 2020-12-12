#pragma once

#include "orange/Renderer/OrthographicCamera.h"
#include "orange/Core/Timestep.h"

#include "orange/Events/ApplicationEvent.h"
#include "orange/Events/MouseEvent.h"


namespace Orange {

	class OrthographicCameraController {

	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_EnableRotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
	};

}
