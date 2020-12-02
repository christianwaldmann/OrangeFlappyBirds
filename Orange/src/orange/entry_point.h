#pragma once


#ifdef OG_PLATFORM_WINDOWS

extern Orange::application* Orange::create_application();

int main(int argc, char** argv) {
	Orange::Log::Init();
	OG_CORE_WARN("Initialized Log!");
	int a = 5;
	OG_INFO("Hallo! Var={0}", a);

	auto app = Orange::create_application();
	app->run();
	delete app;
}

#endif