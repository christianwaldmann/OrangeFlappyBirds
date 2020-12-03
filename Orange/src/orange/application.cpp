#include "ogpch.h"
#include "application.h"

#include "orange/Log.h"


namespace Orange {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


	application::application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(application::OnEvent));
	}


	application::~application() {}


	void application::run() {
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}


	void application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(application::OnWindowClose));

		OG_CORE_TRACE("{0}", e);
	}


	bool application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}