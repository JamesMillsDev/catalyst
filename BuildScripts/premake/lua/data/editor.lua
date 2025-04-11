require("utilities.string-utils")
ids = require("utilities.ids")

local editor=
{
    name = "",
    config_name="",
    additional_files={}
}

function editor:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function editor.parse(lines)
    local new_editor = editor:new{name = "",config_name="",additional_files={}}

    new_editor:get_name(lines)
    table.remove(lines, 1)
    new_editor:get_config_name(lines)
    table.remove(lines, 1)
    new_editor:get_additional_files(lines)

    return new_editor
end

function editor:get_name(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.name) then
        self.name = value_from_len(line, ids.name, 3, 2)
    end
end

function editor:get_config_name(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.config_name) then
        self.config_name = value_from_len(line, ids.config_name, 3, 2)
    end
end

function editor:get_additional_files(lines)
    line = remove_tab(lines[1])

    if contains(line, ids.additional_files) then
        line = value_from_len(line, ids.additional_files, 3, 2)

        line = split(line, ',')

        for l, v in pairs(line) do
            self.additional_files[l - 1] = no_quotes(v)
        end
    end
end

function editor.get_remaining_data(file)
    local data = {}

    while true do
        local line = file:read()
        if contains(line, "modules") then 
            break
        end

        if not contains(line, "editor") or line[1] ~= '[' or line[1] ~= ']' or line[1] ~= '{' or line[1] ~= '}' then
            data[#data+1] = line
        end
    end

    return data
end

function editor.read(file)
    local lines = editor.get_remaining_data(file)

    table.remove(lines, 1)
    table.remove(lines, 1)

    return editor.parse(lines)
end

return editor