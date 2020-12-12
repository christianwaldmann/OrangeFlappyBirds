#include "ogpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"


namespace Orange {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		OG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
