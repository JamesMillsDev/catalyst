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

function upper_partial(input, count)
    return string.upper(string.sub(input, 0, count))
end

function contains(input, val)
    return string.find(input, val) ~= nil
end

function no_quotes(line)
    return line:gsub("%\"", '')
end

function value_from(line, id)
    return value_from_len(line, id, 3, 1)
end

function value_from_tab(line, id)
    return value_from_len(line, id, 7, 1)
end

function remove_tab(line)
    return line:match("^%s*(.-)%s*$")
end

function value_from_len(line, id, front, back)
    return string.sub(line, string.len(id) + front, string.len(line) - back)
end