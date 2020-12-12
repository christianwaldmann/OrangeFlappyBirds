#pragma once


#ifdef OG_PLATFORM_WINDOWS

extern Orange::Application* Orange::CreateApplication();

int main(int argc, char** argv) {
	Orange::Log::Init();

	auto app = Orange::CreateApplication();
	app->run();
	delete app;
}

#endif