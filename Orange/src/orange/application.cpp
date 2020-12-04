#include "ogpch.h"
#include "application.h"

#include "orange/Log.h"

#include "glad/glad.h"


namespace Orange {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


	application* application::s_Instance = nullptr;


	application::application() {
		OG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(application::OnEvent));
	}


	application::~application() {}


	void application::run() {
		while (m_Running) {

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}


	void application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}


	void application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}


	void application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}


	bool application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}