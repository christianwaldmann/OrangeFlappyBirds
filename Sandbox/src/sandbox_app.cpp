#include <orange.h>


class ExampleLayer : public Orange::Layer {

public:
	ExampleLayer() : Layer("Example") {}


	void OnUpdate() override {
		OG_INFO("ExampleLayer::Update");
	}


	void OnEvent(Orange::Event& event) override {
		OG_TRACE("{0}", event);
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