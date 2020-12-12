#include <Orange.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Orange::Layer {

public:
	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f) {
		m_VertexArray.reset(Orange::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		Orange::Ref<Orange::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Orange::VertexBuffer::Create(vertices, sizeof(vertices)));

		Orange::BufferLayout layout = {
			{ Orange::ShaderDataType::Float3, "a_Position"},
			{ Orange::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Orange::Ref<Orange::IndexBuffer> indexBuffer;
		indexBuffer.reset(Orange::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Square
		m_SquareVA.reset(Orange::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Orange::Ref<Orange::VertexBuffer> squareVB;
		squareVB.reset(Orange::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Orange::ShaderDataType::Float3, "a_Position"},
			{ Orange::ShaderDataType::Float2, "a_TexCoord"},
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Orange::Ref<Orange::IndexBuffer> squareIB;
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

		m_Shader = Orange::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		// Square shader
		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}

		)";

		m_FlatColorShader = Orange::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Orange::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Orange::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Orange::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Orange::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}


	void OnUpdate(Orange::Timestep ts) override {
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Orange::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Orange::RenderCommand::Clear();

		Orange::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Orange::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Orange::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Orange::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Orange::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Orange::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Orange::Renderer::Submit(m_Shader, m_VertexArray);

		Orange::Renderer::EndScene();
	}


	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}


	void OnEvent(Orange::Event& event) override {
		m_CameraController.OnEvent(event);
	}

private:
	Orange::ShaderLibrary m_ShaderLibrary;

	Orange::Ref<Orange::Shader> m_Shader;
	Orange::Ref<Orange::VertexArray> m_VertexArray;

	Orange::Ref<Orange::Shader> m_FlatColorShader;
	Orange::Ref<Orange::VertexArray> m_SquareVA;

	Orange::Ref<Orange::Texture2D> m_Texture;
	Orange::Ref<Orange::Texture2D> m_ChernoLogoTexture;

	Orange::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};



class Sandbox : public Orange::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}


	~Sandbox() {

	}
};


Orange::Application* Orange::CreateApplication() {
	return new Sandbox();
}