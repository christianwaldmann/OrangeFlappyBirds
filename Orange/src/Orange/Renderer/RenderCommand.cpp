#include "ogpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Orange {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}
