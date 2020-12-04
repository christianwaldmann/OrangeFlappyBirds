workspace "Orange"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist",
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Orange/vendor/GLFW/include"
IncludeDir["Glad"] = "Orange/vendor/Glad/include"
IncludeDir["ImGui"] = "Orange/vendor/imgui"


include "Orange/vendor/GLFW"
include "Orange/vendor/Glad"
include "Orange/vendor/imgui"


project "Orange"
	location "Orange"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .."/%{prj.name}")
	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

	pchheader "ogpch.h"
	pchsource "Orange/src/ogpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"OG_PLATFORM_WINDOWS",
			"OG_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "OG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "OG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "OG_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .."/%{prj.name}")
	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Orange/vendor/spdlog/include",
		"Orange/src",
	}

	links
	{
		"Orange"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"OG_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "OG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "OG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "OG_DIST"
		runtime "Release"
		optimize "On"