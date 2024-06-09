local module_gen = {}

function module_gen.generate(properties, wks, outputdir, ModuleIncludes, LibDir, ReleaseLibs, DebugLibs)
    local release_lib = ReleaseLibs[#ReleaseLibs]
    local debug_lib = DebugLibs[#ReleaseLibs]

    for l, module_name in pairs(properties.modules) do
        project (properties.name .. properties.deliminator .. module_name)
            location (properties.name .. "/" .. module_name)
            filename (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name))

            kind "SharedLib"
            staticruntime "Off"
    
            language "C++"
            cppdialect "C++20"
    
            targetdir ("%{wks.location}/%{properties.dependencies_dir}/lib/%{properties.name}")
            objdir ("Intermediate/%{prj.name}/" .. outputdir)
    
            pchheader (string.lower(properties.short_name) .. string.lower(module_name) .. "pch.h")
            pchsource (properties.name .. "/" .. module_name .. "/source/".. string.lower(properties.short_name) .. string.lower(module_name) .. "pch.cpp")

            for l, module_name in pairs(properties.modules) do
                if string.find(module_name, properties.modules[0]) == nil then
                    dependson{(properties.name .. properties.deliminator .. properties.modules[0])}
                end
            end

            files
            {
                "%{prj.location}/**.h",
                "%{prj.location}/**.cpp"
            }

            if string.find(module_name, properties.modules[0]) ~= nil then
               files
               {
                    "%{prj.location}/resource.h",
                    "%{prj.location}/Catalyst.Core.rc"
               } 
            end

            includedirs
            {
                "%{prj.location}\\include\\",
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

            filter "configurations:Debug-Editor"
                defines
                { 
                    "%{properties.short_name}_DEBUG",
                    "IS_EDITOR",
                    "DLL_EXPORT"
                }
                symbols "On"
                targetname (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name) .. "_d")
                disablewarnings { "4251" }
                links 
                { 
                    "reactphysics3d_d",
                    "ImGui_d",
                    "pugixml_d"
                }

                if string.find(module_name, properties.modules[0]) == nil then
                    links{ debug_lib }
                end

            filter "configurations:Release-Editor"
                defines
                { 
                    "%{properties.short_name}_RELEASE",
                    "IS_EDITOR",
                    "DLL_EXPORT"
                }
                symbols "On"
                targetname (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name))
                disablewarnings { "4251" }
                links 
                { 
                    "reactphysics3d",
                    "ImGui",
                    "pugixml"
                }

                if string.find(module_name, properties.modules[0]) == nil then
                    links{ release_lib }
                end

            filter "configurations:Debug"
                defines
                {
                    "%{properties.short_name}_DEBUG",
                    "DLL_EXPORT"
                }
                symbols "On"
                targetname (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name) .. "_d")
                disablewarnings { "4251" }
                links 
                { 
                    "reactphysics3d_d",
                    "ImGui_d",
                    "pugixml_d"
                }

                if string.find(module_name, properties.modules[0]) == nil then
                    links{ debug_lib }
                end

            filter "configurations:Release"
                defines 
                {
                    "%{properties.short_name}_RELEASE",
                    "DLL_EXPORT",
                    "pugixml"
                }
                optimize "On"
                targetname (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name))
                disablewarnings { "4251" }
                links 
                { 
                    "reactphysics3d",
                    "ImGui"
                }

                if string.find(module_name, properties.modules[0]) == nil then
                    links{ release_lib }
                end

            filter "configurations:Shipping"
                defines 
                {
                    "%{properties.short_name}_SHIPPING",
                    "DLL_EXPORT"
                }
                optimize "On"
                targetname (string.lower(properties.name) .. properties.deliminator .. string.lower(module_name))
                disablewarnings { "4251" }
                links 
                { 
                    "reactphysics3d",
                    "ImGui",
                    "pugixml"
                }

                if string.find(module_name, properties.modules[0]) == nil then
                    links{ release_lib }
                end
    
    end
end

return module_gen