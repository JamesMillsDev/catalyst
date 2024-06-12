require("utilities.string-utils")

local dll = {name="",mode="",dir=""}

function dll:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function dll.parse(line)
    if contains(line, ids.dll) then
        line = value_from_len(line, ids.dll, 3, 2)

        line = split(line, ',')

        local d = {}

        for l, v in pairs(line) do
            d[#d+1] = dll.build(v)
        end
    end
end

function dll.build(text)
    local new_dll = dll:new{name="",mode="",dir=""}

    local item = split(v, ':')

    local lib = no_quotes(item[1])
    local mode = no_quotes(item[2])
    local path = no_quotes(item[3])

    new_dll.name = lib
    new_dll.mode = mode
    new_dll.dir = path

    return new_dll
end

return dll