#include <orange.h>


class ExampleLayer : public Orange::Layer {

public:
	ExampleLayer() : Layer("Example") {}


	void OnUpdate() override {
		if (Orange::Input::IsKeyPressed(OG_KEY_TAB))
			OG_TRACE("Tab key is pressed!");
	}


	void OnEvent(Orange::Event& event) override {
		if (event.GetEventType() == Orange::EventType::KeyPressed) {
			Orange::KeyPressedEvent& e = (Orange::KeyPressedEvent&)event;
			OG_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};



class Sandbox : public Orange::application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Orange::ImGuiLayer());
	}


	~Sandbox() {

	}
};


Orange::application* Orange::create_application() {
	return new Sandbox();
}