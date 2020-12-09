#include <orange.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>


class ExampleLayer : public Orange::Layer {

public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
		m_VertexArray.reset(Orange::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Orange::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Orange::VertexBuffer::Create(vertices, sizeof(vertices)));

		Orange::BufferLayout layout = {
			{ Orange::ShaderDataType::Float3, "a_Position"},
			{ Orange::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Orange::IndexBuffer> indexBuffer;
		indexBuffer.reset(Orange::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Square
		m_SquareVA.reset(Orange::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Orange::VertexBuffer> squareVB;
		squareVB.reset(Orange::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Orange::ShaderDataType::Float3, "a_Position"},
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Orange::IndexBuffer> squareIB;
		squareIB.reset(Orange::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = v_Color;
			}

		)";

		m_Shader.reset(new Orange::Shader(vertexSrc, fragmentSrc));

		// Square shader
		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main() {
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		m_BlueShader.reset(new Orange::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}


	void OnUpdate(Orange::Timestep ts) override {
		OG_TRACE("Delta time: {0}s {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		if (Orange::Input::IsKeyPressed(OG_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Orange::Input::IsKeyPressed(OG_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Orange::Input::IsKeyPressed(OG_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Orange::Input::IsKeyPressed(OG_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Orange::Input::IsKeyPressed(OG_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Orange::Input::IsKeyPressed(OG_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Orange::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Orange::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Orange::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Orange::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
			}
		}
		Orange::Renderer::Submit(m_Shader, m_VertexArray);

		Orange::Renderer::EndScene();
	}


	void OnImGuiRender() override {

	}


	void OnEvent(Orange::Event& event) override {

	}

private:
	std::shared_ptr<Orange::Shader> m_Shader;
	std::shared_ptr<Orange::VertexArray> m_VertexArray;

	std::shared_ptr<Orange::Shader> m_BlueShader;
	std::shared_ptr<Orange::VertexArray> m_SquareVA;

	Orange::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
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