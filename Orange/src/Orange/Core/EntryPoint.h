#pragma once


#ifdef OG_PLATFORM_WINDOWS

extern Orange::Application* Orange::CreateApplication();

int main(int argc, char** argv) {
	Orange::Log::Init();

	OG_PROFILE_BEGIN_SESSION("Startup", "OrangeProfile-Startup.json");
	auto app = Orange::CreateApplication();
	OG_PROFILE_END_SESSION();

	OG_PROFILE_BEGIN_SESSION("Runtime", "OrangeProfile-Runtime.json");
	app->run();
	OG_PROFILE_END_SESSION();

	OG_PROFILE_BEGIN_SESSION("Shutdown", "OrangeProfile-Shutdown.json");
	delete app;
	OG_PROFILE_END_SESSION();
}

#endif
