#pragma once

#include "core.h"
#include "Events/Event.h"
#include "orange/Events/ApplicationEvent.h"
#include "Window.h"


namespace Orange {

	class ORANGE_API application {

	public:
		application();
		virtual ~application();

		void run();
		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	application* create_application();
}
