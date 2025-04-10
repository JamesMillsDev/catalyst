require("utilities.string-utils")

local dll = {name="",mode="",dir=""}

function dll:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function dll.parse(line)
    local l = split(line, ',')

    local d = {}

    for l, v in pairs(l) do
        d[#d+1] = dll.build(v)
    end

    return d
end

function dll.build(text)
    local new_dll = dll:new{name="",mode="",dir=""}

    local item = split(no_quotes(text), ':')

    local lib = item[1]
    local mode = item[2]
    local path = item[3]

    new_dll.name = lib
    new_dll.mode = mode
    new_dll.dir = path

    return new_dll
end

return dll