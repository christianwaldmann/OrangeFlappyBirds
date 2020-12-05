#include <orange.h>

#include "imgui/imgui.h"


class ExampleLayer : public Orange::Layer {

public:
	ExampleLayer() : Layer("Example") {}


	void OnUpdate() override {
		if (Orange::Input::IsKeyPressed(OG_KEY_TAB))
			OG_TRACE("Tab key is pressed!");
	}


	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hallo Welt");
		ImGui::End();
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
	}


	~Sandbox() {

	}
};


Orange::application* Orange::create_application() {
	return new Sandbox();
}