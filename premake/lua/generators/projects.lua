local projects = {}

function projects.generate(properties, outputdir)
    local lower_short = string.lower(properties.short_name)
    local lower_name = string.lower(properties.name)
    
    for l, module in pairs(properties.modules) do
        local lower_module = string.lower(module.name)

        project(properties.name .. properties.deliminator .. module.name)
            location (properties.name .. "/" .. module.name)
            filename (lower_name .. properties.deliminator .. lower_module)

            kind "SharedLib"
            staticruntime "Off"
    
            language "C++"
            cppdialect ("C++"..properties.language_version)
    
            targetdir ("%{wks.location}/"..properties.dependencies_dir.."/lib/"..properties.name)
            objdir ("Intermediate/%{prj.name}/" .. outputdir)

            targetname(lower_name .. properties.deliminator .. lower_module)
    
            pchheader (lower_short .. lower_module .. "pch.h")
            pchsource (properties.name .. "/" .. module.name .. "/source/".. lower_short .. lower_module .. "pch.cpp")

            for l, dependency in pairs(module.prj_dependencies) do
                dependson{(lower_name .. properties.deliminator .. string.lower(dependency))}
            end

            files
            {
                "%{prj.location}/**.h",
                "%{prj.location}/**.cpp"
            }

            for l, file in pairs(module.additional_files) do
                files{ ("%{prj.location}/" .. file) }
            end

            includedirs
            {
                "%{prj.location}\\include\\",
                ("$(SolutionDir)"..properties.dependencies_dir.."\\include\\")
            }

            for l, included in pairs(module.prj_dependencies) do
                includedirs{ ("%{wks.location}\\" .. properties.name .. "\\" .. included .. "\\include") }
            end
                
            for l,v in pairs(module.unified_links) do
                links{ v }
            end
                
            for l,v in pairs(module.link_dirs) do
                libdirs{ ("$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..v) }
            end
            
            libdirs{ ("$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name) }
                
            for l,v in pairs(properties.disabled_warnings) do
                disablewarnings{ v }
            end

            filter "configurations:Debug-Editor"
                defines
                { 
                    (properties.short_name.."_DEBUG"),
                    "IS_EDITOR",
                    "DLL_EXPORT"
                }
                symbols "On"
                targetsuffix "_d"
                
                for l,v in pairs(module.split_links) do
                    links{ (v.."_d") }
                end
                
                for l, dependency in pairs(module.prj_dependencies) do
                    links{(lower_name .. properties.deliminator .. string.lower(dependency).."_d.lib")}
                end

            filter "configurations:Release-Editor"
                defines
                { 
                    (properties.short_name.."_RELEASE"),
                    "IS_EDITOR",
                    "DLL_EXPORT"
                }
                symbols "On"
                
                for l,v in pairs(module.split_links) do
                    links{ v }
                end
                
                for l, dependency in pairs(module.prj_dependencies) do
                    links{(lower_name .. properties.deliminator .. string.lower(dependency)..".lib")}
                end

            filter "configurations:Debug"
                defines
                {
                    (properties.short_name.."_DEBUG"),
                    "DLL_EXPORT"
                }
                symbols "On"
                targetsuffix "_d"
                
                for l,v in pairs(module.split_links) do
                    links{ (v.."_d") }
                end
                
                for l, dependency in pairs(module.prj_dependencies) do
                    links{(lower_name .. properties.deliminator .. string.lower(dependency).."_d.lib")}
                end

            filter "configurations:Release"
                defines 
                {
                    (properties.short_name.."_RELEASE"),
                    "DLL_EXPORT",
                    "pugixml"
                }
                optimize "On"
                
                for l,v in pairs(module.split_links) do
                    links{ v }
                end
                
                for l, dependency in pairs(module.prj_dependencies) do
                    links{(lower_name .. properties.deliminator .. string.lower(dependency)..".lib")}
                end

            filter "configurations:Shipping"
                defines 
                {
                    (properties.short_name.."_SHIPPING"),
                    "DLL_EXPORT"
                }
                optimize "On"
                
                for l,v in pairs(module.split_links) do
                    links{ v }
                end
                
                for l, dependency in pairs(module.prj_dependencies) do
                    links{(lower_name .. properties.deliminator .. string.lower(dependency)..".lib")}
                end

    end
end

return projects