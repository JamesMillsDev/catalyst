properties = require("premake/scripts/properties_file")
module_gen = require("premake/scripts/module_gen")

local current_dir=io.popen"cd":read'*l'
properties.read(current_dir .. "\\engine.properties")

local testbedname = "TestBed"
local enginename = properties.name
local deliminator = properties.deliminator

workspace (enginename)
    architecture "x64"

    configurations
	{
		"Debug-Editor",
		"Release-Editor",
		"Debug",
		"Release",
		"Shipping"
	}

	startproject (testbedname)

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\GLFW"
IncludeDir["glew"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\GL"
IncludeDir["ImGui"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\ImGui"
IncludeDir["glm"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\glm"
IncludeDir["stbi"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\stbi"
IncludeDir["rapidyaml"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\rapidyaml"
IncludeDir["react"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\reactphysics3d"
IncludeDir["assimp"] = "$(SolutionDir)%{properties.dependencies_dir}\\include\\assimp"
	
-- Include directories relative to root folder (solution directory)
LibDir = {}
LibDir["glfw"] = "$(SolutionDir)%{properties.dependencies_dir}\\lib\\GLFW"
LibDir["glew"] = "$(SolutionDir)%{properties.dependencies_dir}\\lib\\GL"
LibDir["ImGui"] = "$(SolutionDir)%{properties.dependencies_dir}\\lib\\ImGui"
LibDir["react"] = "$(SolutionDir)%{properties.dependencies_dir}\\lib\\reactphysics3d"
LibDir["assimp"] = "$(SolutionDir)%{properties.dependencies_dir}\\lib\\assimp"
LibDir[enginename] = ("$(SolutionDir)%{properties.dependencies_dir}\\lib\\" .. enginename)

engineshort = properties.short_name

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

DebugLibs = {}
for l, module_name in pairs(properties.modules) do
	DebugLibs[#DebugLibs+1] = (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name) .. "_d") 
end

ReleaseLibs = {}
for l, module_name in pairs(properties.modules) do
	ReleaseLibs[#ReleaseLibs+1] = (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name) .. ".lib")
end

DebugPostBuild = {}
for l, module_name in pairs(properties.modules) do
	DebugPostBuild[#DebugPostBuild+1] = ("copy /Y \"%{LibDir."..enginename.."}\\" .. string.lower(properties.name) .. properties.deliminator .. string.lower(module_name) .. "_d.dll\" \"%{wks.location}Builds\\%{prj.name}\\" .. outputdir .. "\\" .. string.lower(enginename) .. deliminator .. string.lower(module_name) .. "_d.dll\"")
end

ReleasePostBuild = {}
for l, module_name in pairs(properties.modules) do
	ReleasePostBuild[#ReleasePostBuild+1] = ("copy /Y \"%{LibDir."..enginename.."}\\" .. string.lower(properties.name) .. properties.deliminator .. string.lower(module_name) .. ".dll\" \"%{wks.location}Builds\\%{prj.name}\\" .. outputdir .. "\\" .. string.lower(enginename) .. deliminator .. string.lower(module_name) .. ".dll\"")
end

ModuleIncludes = {}
for l, module_name in pairs(properties.modules) do
	ModuleIncludes[#ModuleIncludes+1] = ("%{wks.location}\\" .. enginename .. "\\" .. module_name .. "\\include")
end

project (testbedname)
	location "%{prj.name}"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++20"

	targetdir ("$(SolutionDir)Builds\\%{prj.name}\\" .. outputdir)
	objdir ("$(SolutionDir)\\Intermediate\\%{prj.name}\\" .. outputdir)

	files
	{
		"%{prj.location}\\**.h",
		"%{prj.location}\\**.cpp"
	}

	for l, module_name in pairs(properties.modules) do
		dependson{(properties.name .. properties.deliminator .. module_name)}
	end

	includedirs
	{
		"%{prj.location}\\include",
		"$(SolutionDir)%{properties.dependencies_dir}\\include\\",
		ModuleIncludes
	}

	libdirs
	{
		LibDir
	}

	links
	{
		"glfw3dll",
		"glew32",
		"assimp-vc143-mt",
		"opengl32.lib"
	}

	postbuildcommands
	{
		("copy /Y \"%{LibDir.glfw}\\glfw3.dll\" \"$(OutDir)\\glfw3.dll\""),
		("copy /Y \"%{LibDir.glew}\\glew32.dll\" \"$(OutDir)\\glew32.dll\""),
		("copy /Y \"%{LibDir.assimp}\\assimp-vc143-mt.dll\" \"$(OutDir)\\assimp-vc143-mt.dll\"")
	}

	filter "configurations:Debug-Editor"
		symbols "On"
		
		links 
		{ 
			"reactphysics3d_d",
			"ImGui_d",
			DebugLibs
		}
		
		postbuildcommands
		{
			("copy /Y \"%{LibDir.ImGui}\\ImGui_d.dll\" \"$(OutDir)ImGui_d.dll\""),
			DebugPostBuild
		}

	filter "configurations:Release-Editor"
		symbols "On"
		
		links 
		{ 
			"reactphysics3d",
			"ImGui",
			ReleaseLibs
		}
		
		postbuildcommands
		{
			("copy /Y \"%{LibDir.ImGui}\\ImGui.dll\" \"$(OutDir)ImGui.dll\""),
			ReleasePostBuild
		}

	filter "configurations:Debug"
		symbols "On"

		links 
		{ 
			"reactphysics3d_d",
			"ImGui_d",
			DebugLibs
		}
		
		postbuildcommands
		{
			("copy /Y \"%{LibDir.ImGui}\\ImGui_d.dll\" \"$(OutDir)ImGui_d.dll\""),
			DebugPostBuild
		}

	filter "configurations:Release"
		optimize "On"

		links 
		{ 
			"reactphysics3d",
			"ImGui",
			ReleaseLibs
		}
		
		postbuildcommands
		{
			("copy /Y \"%{LibDir.ImGui}\\ImGui.dll\" \"$(OutDir)ImGui.dll\""),
			ReleasePostBuild
		}

	filter "configurations:Shipping"
		optimize "On"

		links 
		{ 
			"reactphysics3d",
			"ImGui",
			ReleaseLibs
		}
		
		postbuildcommands
		{
			("copy /Y \"%{LibDir.ImGui}\\ImGui.dll\" \"$(OutDir)ImGui.dll\""),
			ReleasePostBuild
		}

group (properties.name)

table.remove(LibDir, 1)

module_gen.generate(properties, wks, outputdir, ModuleIncludes, LibDir, ReleaseLibs, DebugLibs)