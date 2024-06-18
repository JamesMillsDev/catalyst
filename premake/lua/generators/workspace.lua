local ws = {}

function ws.generate(engine_props)
	workspace (engine_props.name)
    architecture "x64"

    configurations
	{
		"Debug-Editor",
		"Release-Editor",
		"Debug",
		"Release",
		"Shipping"
	}

	if engine_props.testbed_mode == true then
		startproject (engine_props.testbed_name)
	else
		startproject (engine_props.name .. "-Editor")
	end
end

return ws