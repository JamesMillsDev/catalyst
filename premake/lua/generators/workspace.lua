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

	startproject (engine_props.testbed_name)
end

return ws