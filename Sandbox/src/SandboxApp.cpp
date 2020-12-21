#include <Orange.h>
#include <Orange/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "GameLayer.h"



class Sandbox : public Orange::Application {

public:
	Sandbox() {
		PushLayer(new GameLayer());
	}


	~Sandbox() {

	}
};


Orange::Application* Orange::CreateApplication() {
	return new Sandbox();
}