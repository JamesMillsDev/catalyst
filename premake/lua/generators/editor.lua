local editor = {}

function editor.generate(properties, outputdir)
    project(properties.name .. "-" .. properties.editor.name)
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

        for l, file in pairs(properties.editor.additional_files) do
            files{ ("%{prj.location}/" .. file) }
        end

        for l, module in pairs(properties.modules) do
            dependson{(properties.name .. properties.deliminator .. module.name)}
        end

        includedirs
        {
            "%{prj.location}\\include",
            ("$(SolutionDir)"..properties.dependencies_dir.."\\include\\")
        }

        for l, module in pairs(properties.modules) do
            includedirs{"$(SolutionDir)"..properties.name.."\\"..module.name.."\\include\\"}
        end
            
        for l, module in pairs(properties.modules) do
            for i, link in pairs(module.unified_links) do
                links{ link }
            end
        end
                
        for l, module in pairs(properties.modules) do
            for i, v in pairs(module.link_dirs) do
                libdirs{ ("$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..v) }
            end
        end

        for l, dll in pairs(properties.dll) do
            if dll.mode == "single" then
                postbuildcommands
                {
                    ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name..".dll\" \"$(OutDir)"..dll.name..".dll\"")
                }
            end
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
            
            for l, module in pairs(properties.modules) do
                for i, link in pairs(module.split_links) do
                    links{ (link.."_d") }
                end
            end
            
            for l, dependency in pairs(properties.modules) do
                links{(string.lower(properties.name) .. properties.deliminator .. string.lower(dependency.name).."_d.lib")}
            end
            
            for l, dependency in pairs(properties.modules) do
                postbuildcommands
                {
                    ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency.name).."_d.dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency.name).."_d.dll\"")
                }
            end

            for l, dll in pairs(properties.dll) do
                if dll.mode == "split" then
                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name.."_d.dll\" \"$(OutDir)"..dll.name.."_d.dll\"")
                    }
                end
            end

        filter "configurations:Release-Editor"
            defines
            { 
                (properties.short_name.."_RELEASE"),
                "IS_EDITOR",
                "DLL_EXPORT"
            }
            symbols "On"
            
            for l, module in pairs(properties.modules) do
                for i, link in pairs(module.split_links) do
                    links{ (link) }
                end
            end
            
            for l, dependency in pairs(properties.modules) do
                links{(string.lower(properties.name) .. properties.deliminator .. string.lower(dependency.name)..".lib")}
            end
            
            for l, dependency in pairs(properties.modules) do
                postbuildcommands
                {
                    ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency.name)..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency.name)..".dll\"")
                }
            end

            for l, dll in pairs(properties.dll) do
                if dll.mode == "split" then
                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name..".dll\" \"$(OutDir)"..dll.name..".dll\"")
                    }
                end
            end

        filter "configurations:Debug"
            defines
            {
                (properties.short_name.."_DEBUG"),
                "DLL_EXPORT"
            }
            symbols "On"
            targetsuffix "_d"
            
            for l, module in pairs(properties.modules) do
                for i, link in pairs(module.split_links) do
                    links{ (link.."_d") }
                end
            end
            
            for l, dependency in pairs(properties.modules) do
                links{(string.lower(properties.name) .. properties.deliminator .. string.lower(dependency.name).."_d.lib")}
            end
            
            for l, dependency in pairs(properties.modules) do
                postbuildcommands
                {
                    ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency.name).."_d.dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency.name).."_d.dll\"")
                }
            end

            for l, dll in pairs(properties.dll) do
                if dll.mode == "split" then
                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name.."_d.dll\" \"$(OutDir)"..dll.name.."_d.dll\"")
                    }
                end
            end

        filter "configurations:Release"
            defines 
            {
                (properties.short_name.."_RELEASE"),
                "DLL_EXPORT",
                "pugixml"
            }
            optimize "On"
            
            for l, module in pairs(properties.modules) do
                for i, link in pairs(module.split_links) do
                    links{ (link) }
                end
            end
            
            for l, dependency in pairs(properties.modules) do
                links{(string.lower(properties.name) .. properties.deliminator .. string.lower(dependency.name)..".lib")}
            end
            
            for l, dependency in pairs(properties.modules) do
                postbuildcommands
                {
                    ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency.name)..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency.name)..".dll\"")
                }
            end

            for l, dll in pairs(properties.dll) do
                if dll.mode == "split" then
                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name..".dll\" \"$(OutDir)"..dll.name..".dll\"")
                    }
                end
            end

        filter "configurations:Shipping"
            defines 
            {
                (properties.short_name.."_SHIPPING"),
                "DLL_EXPORT"
            }
            optimize "On"
            
            for l, module in pairs(properties.modules) do
                for i, link in pairs(module.split_links) do
                    links{ link }
                end
            end
            
            for l, dependency in pairs(properties.modules) do
                links{(string.lower(properties.name) .. properties.deliminator .. string.lower(dependency.name)..".lib")}
            end
            
            for l, dependency in pairs(properties.modules) do
                postbuildcommands
                {
                    ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency.name)..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency.name)..".dll\"")
                }
            end

            for l, dll in pairs(properties.dll) do
                if dll.mode == "split" then
                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name..".dll\" \"$(OutDir)"..dll.name..".dll\"")
                    }
                end
            end
end

return editor