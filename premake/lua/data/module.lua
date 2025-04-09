require("utilities.string-utils")
ids = require("utilities.ids")

local module = {name="",prj_dependencies={},includes={},additional_files={},unified_links={},split_links={},link_dirs={}}

function module:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function module.parse(lines)
    local new_module = module:new{name="",prj_dependencies={},includes={},additional_files={},unified_links={},split_links={},link_dirs={}}

    new_module:get_name(lines)
    table.remove(lines, 1)
    new_module:get_prj_deps(lines)
    table.remove(lines, 1)
    new_module:get_additional_files(lines)
    table.remove(lines, 1)
    new_module:get_links(lines)
    table.remove(lines, 1)
    new_module:get_link_dirs(lines)
    table.remove(lines, 1)

    return new_module
end

function module:get_name(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.name) then
        self.name = value_from_len(line, ids.name, 3, 2)
    end
end

function module:get_prj_deps(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.prj_dependencies) then
        line = value_from_len(line, ids.prj_dependencies, 3, 2)

        line = split(line, ',')

        for l, v in pairs(line) do
            self.prj_dependencies[l - 1] = no_quotes(v)
        end
    end
end

function module:get_additional_files(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.additional_files) then
        line = value_from_len(line, ids.additional_files, 3, 2)

        line = split(line, ',')

        for l, v in pairs(line) do
            self.additional_files[l - 1] = no_quotes(v)
        end
    end
end

function module:get_links(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.links) then
        line = value_from_len(line, ids.links, 3, 2)

        line = split(line, ',')

        for l, v in pairs(line) do
            local item = split(v, ':')

            local lib = no_quotes(item[1])
            local mode = no_quotes(item[2])

            if mode == "single" then
                self.unified_links[l - 1] = lib
            else
                self.split_links[l - 1] = lib
            end
        end
    end
end

function module:get_link_dirs(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.link_dirs) then
        line = value_from_len(line, ids.link_dirs, 3, 2)

        line = split(line, ',')

        for l, v in pairs(line) do
            self.link_dirs[l - 1] = no_quotes(v)
        end
    end
end

function module.get_remaining_data(file)
    local data = {}

    while true do
        local line = file:read()
        if line == nil then 
            break
        end

        if not contains(line, "modules") or line[1] ~= '[' or line[1] ~= ']' or line[1] ~= '{' or line[1] ~= '}' then
            data[#data+1] = line
        end
    end

    return data
end

function module.read_modules(file)
    local lines = module.get_remaining_data(file)

    table.remove(lines, 1)
    table.remove(lines, 1)

    local m = {}

    while next(lines) ~= nil do
        m[#m+1] = module.parse(lines)

        table.remove(lines, 1)
        table.remove(lines, 1)
    end

    return m
end

return module