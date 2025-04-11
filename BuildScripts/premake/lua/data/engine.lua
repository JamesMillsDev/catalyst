require("utilities.string-utils")
module = require("data.module")
editor = require("data.editor")
dll = require("data.dll")
ids = require("utilities.ids")

local properties = 
{ 
    name = "", 
    modules = {}, 
    editor = {},
    short_name = "", 
    dependencies_dir = "", 
    deliminator = '.', 
    testbed_name = "",
    testbed_mode = "",
    language_version = "",
    disabled_warnings = {},
    dlls = {}
}

function properties:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function properties.read(filepath)
    local new_properties = properties:new
    { 
        name = "", 
        modules = {}, 
        editor = {},
        short_name = "", 
        dependencies_dir = "", 
        deliminator = '.', 
        testbed_name = "",
        testbed_mode = "",
        language_version = "",
        disabled_warnings = {},
        dlls = {}
    }

    local file = assert(io.open(filepath, "r"))

    new_properties:get_name(file)
    new_properties:get_deliminator(file)
    new_properties:get_dependencies_dir(file)
    new_properties:get_testbed_name(file)
    new_properties:get_testbed_mode(file)
    new_properties:get_language_version(file)
    new_properties:get_disabled_warnings(file)
    new_properties:get_dlls(file)

    new_properties.editor = editor.read(file)
    new_properties.modules = module.read_modules(file)
    new_properties.short_name = upper_partial(new_properties.name, 2)

    file:close()

    return new_properties
end

function properties:get_name(file)
    line = assert(file:read())

    if contains(line, ids.name) then
        self.name = value_from(line, ids.name)
    end
end

function properties:get_deliminator(file)
    line = assert(file:read())

    if contains(line, ids.deliminator) then
        self.deliminator = value_from(line, ids.deliminator)
    end
end

function properties:get_dependencies_dir(file)
    line = assert(file:read())

    if contains(line, ids.dependencies_dir) then
        self.dependencies_dir = value_from(line, ids.dependencies_dir)
    end
end

function properties:get_testbed_name(file)
    line = assert(file:read())

    if contains(line, ids.testbed_name) then
        self.testbed_name = value_from(line, ids.testbed_name)
    end
end

function properties:get_testbed_mode(file)
    line = assert(file:read())

    if contains(line, ids.testbed_mode) then
        self.testbed_mode = value_from(line, ids.testbed_mode)
    end
end

function properties:get_language_version(file)
    line = assert(file:read())

    if contains(line, ids.language_version) then
        self.language_version = value_from(line, ids.language_version)
    end
end

function properties:get_disabled_warnings(file)
    line = assert(file:read())

    if contains(line, ids.disabled_warnings) then
        line = value_from_len(line, ids.disabled_warnings, 3, 2)

        line = split(line, ',')

        for l, v in pairs(line) do
            self.disabled_warnings[l - 1] = no_quotes(v)
        end
    end
end

function properties:get_dlls(file)
    line = assert(file:read())

    if contains(line, ids.dll) then
        line = value_from_len(line, ids.dll, 3, 2)

        self.dll = dll.parse(line)
    end
end

return properties