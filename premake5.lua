properties = require("data.engine")

local current_dir=io.popen"cd":read"*l"

local engine_props = properties.read(current_dir .. "\\engine.properties")
local outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

tb = require("generators.testbed")
ws = require("generators.workspace")
pj = require("generators.projects")

ws.generate(engine_props)
tb.generate(engine_props, outputdir)

group (engine_props.name)
pj.generate(engine_props, outputdir)