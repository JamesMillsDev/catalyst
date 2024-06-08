local name_id = "name"
local modules_id = "modules"
local dependencies_dir_id = "dependencies_dir"
local deliminator_id = "deliminator"

function split(inputstr, sep)
    if sep == nil then
        sep = "%s"
    end
    local t = {}
    for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
        table.insert(t, str)
    end
    return t
end

local properties = {}

properties.name = ""
properties.modules = {}
properties.short_name = ""
properties.dependencies_dir = ""
properties.deliminator = '.'

function properties.read(filepath)
    local file = assert(io.open(filepath, "r"))

    properties.read_name(file)
    properties.read_modules(file)
    properties.read_deliminator(file)
    properties.read_dependencies_dir(file)
    properties.short_name = string.upper(string.sub(properties.name, 0, 2))

    file:close()
end

function properties.read_name(file)
    line = assert(file:read())

    if string.find(line, name_id) ~= nil then
        properties.name = string.sub(line, string.len(name_id) + 3, string.len(line) - 1)
    end

end

function properties.read_modules(file)
    line = assert(file:read())

    line = string.sub(line, 10, string.len(line) - 1)

    lines = split(line, ',')

    for l, v in pairs(lines) do
        properties.modules[l - 1] = string.sub(v, 2, string.len(v) - 1)
    end
end

function properties.read_deliminator(file)
    line = assert(file:read())

    if string.find(line, deliminator_id) ~= nil then
        properties.deliminator = string.sub(line, string.len(deliminator_id) + 3, string.len(line) - 1)
    end

end

function properties.read_dependencies_dir(file)
    line = assert(file:read())

    if string.find(line, dependencies_dir_id) ~= nil then
        properties.dependencies_dir = string.sub(line, string.len(dependencies_dir_id) + 3, string.len(line) - 1)
    end

end

return properties