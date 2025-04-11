import re
import sys
from pathlib import Path

output_path = sys.argv[1]

source_root = Path(__file__).resolve().parents[1]
source_dirs = [source_root / "Catalyst", source_root / "Catalyst-Editor"]

REGISTER_COMPONENT_PATTERN = re.compile(r"REGISTER_COMPONENT\s*\(\s*(\w+)\s*\)")

headers = []
anchors = []

for src_dir in source_dirs:
	for path in src_dir.rglob("*.h"):
		with open(path, "r", encoding="utf-8") as f:
			lines = f.readlines()

			for i, line in enumerate(lines):
				# Skip macro definitions and comments
				if "#define REGISTER_COMPONENT" in line or line.strip().startswith("//"):
					continue

				matches = REGISTER_COMPONENT_PATTERN.findall(line)
				if matches:
					headers.append(f'#include "{path.as_posix()}"')
					for typename in matches:
						pass
						anchors.append(f"\tCatalyst::{typename}::s_ComponentAnchor,")

output = "// Auto-generated. Do not modify manually.\n\n"
output += "\n".join(sorted(headers)) + "\n\n"
output += "const void* g_ComponentAnchors[] =\n"
output += "{\n"
output += "\n".join(sorted(anchors)) + "\n};\n"

with open(output_path, "w", encoding="utf-8") as f:
	f.write(output)