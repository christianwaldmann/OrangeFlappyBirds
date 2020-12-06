#include "ogpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"


namespace Orange {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		OG_CORE_ASSERT(windowHandle, "Window handle is null!");
	}


	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OG_CORE_ASSERT(status, "Failed to initialize Glad!");

		OG_CORE_INFO("OpenGL Info:");
		OG_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		OG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		OG_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
	}


	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}
