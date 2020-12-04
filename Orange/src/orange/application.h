#pragma once

#include "core.h"
#include "Window.h"
#include "Events/Event.h"
#include "orange/Events/ApplicationEvent.h"
#include "orange/LayerStack.h"


namespace Orange {

	class ORANGE_API application {

	public:
		application();
		virtual ~application();

		void run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static application* s_Instance;
	};

	// To be defined in CLIENT
	application* create_application();
}
