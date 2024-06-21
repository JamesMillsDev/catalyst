local tests = {}

function tests.generate(properties, outputdir)
    local lower_short = string.lower(properties.short_name)
    local lower_name = string.lower(properties.name)

    for l, module in pairs(properties.modules) do
        if module.testing_enabled == "true" then
            local lower_module = string.lower(module.name)

            project(properties.name..properties.deliminator..module.name..properties.deliminator.."Tests")
                location ("Tests/%{prj.name}")
                filename (lower_name..properties.deliminator..lower_module..properties.deliminator.."tests")

                kind "SharedLib"
                staticruntime "Off"
        
                language "C++"
                cppdialect ("C++"..properties.language_version)
        
                targetdir ("$(SolutionDir)Builds\\%{prj.name}\\" .. outputdir)
                objdir ("Intermediate/%{prj.name}/" .. outputdir)
    
                targetname(lower_name .. properties.deliminator .. lower_module..properties.deliminator .. "tests")
    
                pchheader (lower_short .. lower_module .. "pch.h")
                pchsource ("Tests/%{prj.name}/source/".. lower_short .. lower_module .. "pch.cpp")

                for l, dependency in pairs(module.prj_dependencies) do
                    dependson{(lower_name .. properties.deliminator .. string.lower(dependency))}
                end

                dependson{(lower_name .. properties.deliminator .. lower_module)}

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
                    ("%{wks.location}\\"..properties.name.."\\"..module.name.."\\include"),
                    "$(VCInstallDir)UnitTest\\include",
                    ("$(SolutionDir)"..properties.dependencies_dir.."\\include\\")
                }

                for l, included in pairs(module.prj_dependencies) do
                    includedirs{ ("%{wks.location}\\"..properties.name.."\\"..included.."\\include") }
                end

                for l,v in pairs(module.unified_links) do
                    links{ v }
                end
                    
                for l,v in pairs(module.link_dirs) do
                    libdirs{ ("$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..v) }
                end

                libdirs
                {
                    "$(VCInstallDir)UnitTest\\lib",
                    ("$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name)
                }

                for l,v in pairs(properties.disabled_warnings) do
                    disablewarnings{ v }
                end

                for l, dll in pairs(properties.dll) do
                    if dll.mode == "single" then
                        postbuildcommands
                        {
                            ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..dll.dir.."\\"..dll.name..".dll\" \"$(OutDir)"..dll.name..".dll\"")
                        }
                    end
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

                    links{ (lower_name .. properties.deliminator .. lower_module.."_d.lib") }

                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..lower_module.."_d.dll\" \"$(OutDir)"..string.lower(properties.name).."."..lower_module.."_d.dll\"")
                    }
            
                    for l, dependency in pairs(module.prj_dependencies) do
                        postbuildcommands
                        {
                            ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency).."_d.dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency).."_d.dll\"")
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
                    
                    for l,v in pairs(module.split_links) do
                        links{ v }
                    end
                    
                    for l, dependency in pairs(module.prj_dependencies) do
                        links{(lower_name .. properties.deliminator .. string.lower(dependency)..".lib")}
                    end
                    
                    links{ (lower_name .. properties.deliminator .. lower_module..".lib") }

                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..lower_module..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..lower_module..".dll\"")
                    }
            
                    for l, dependency in pairs(module.prj_dependencies) do
                        postbuildcommands
                        {
                            ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency)..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency)..".dll\"")
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
                    
                    for l,v in pairs(module.split_links) do
                        links{ (v.."_d") }
                    end
                    
                    for l, dependency in pairs(module.prj_dependencies) do
                        links{(lower_name .. properties.deliminator .. string.lower(dependency).."_d.lib")}
                    end
                    
                    links{ (lower_name .. properties.deliminator .. lower_module.."_d.lib") }

                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..lower_module.."_d.dll\" \"$(OutDir)"..string.lower(properties.name).."."..lower_module.."_d.dll\"")
                    }
            
                    for l, dependency in pairs(module.prj_dependencies) do
                        postbuildcommands
                        {
                            ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency).."_d.dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency).."_d.dll\"")
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
                    
                    for l,v in pairs(module.split_links) do
                        links{ v }
                    end
                    
                    for l, dependency in pairs(module.prj_dependencies) do
                        links{(lower_name .. properties.deliminator .. string.lower(dependency)..".lib")}
                    end
                    
                    links{ (lower_name .. properties.deliminator .. lower_module..".lib") }

                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..lower_module..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..lower_module..".dll\"")
                    }
            
                    for l, dependency in pairs(module.prj_dependencies) do
                        postbuildcommands
                        {
                            ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency)..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency)..".dll\"")
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
                    
                    for l,v in pairs(module.split_links) do
                        links{ v }
                    end
                    
                    for l, dependency in pairs(module.prj_dependencies) do
                        links{(lower_name .. properties.deliminator .. string.lower(dependency)..".lib")}
                    end
                    
                    links{ (lower_name .. properties.deliminator .. lower_module..".lib") }
                    
                    postbuildcommands
                    {
                        ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..lower_module..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..lower_module..".dll\"")
                    }
            
                    for l, dependency in pairs(module.prj_dependencies) do
                        postbuildcommands
                        {
                            ("copy /Y \"$(SolutionDir)"..properties.dependencies_dir.."\\lib\\"..properties.name.."\\"..string.lower(properties.name).."."..string.lower(dependency)..".dll\" \"$(OutDir)"..string.lower(properties.name).."."..string.lower(dependency)..".dll\"")
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
    end
end

return tests