#include "ogpch.h"
#include "application.h"

#include "orange/Events/ApplicationEvent.h"
#include "orange/Log.h"


namespace Orange {

	application::application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}


	application::~application() {}


	void application::run() {
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}
}