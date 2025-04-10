import sys
import os

def embed_file(input_file, output_header):
    # Read the file as binary
    with open(input_file, "rb") as f:
        binary_data = f.read()

    # Convert binary to a C++ byte array
    hex_array = ", ".join(f"0x{byte:02x}" for byte in binary_data)
    array_size = len(binary_data)

    # Generate C++ header content
    header_content = f"""#pragma once

#include <cstddef>

constexpr unsigned char embedded_file[] = {{ {hex_array} }};
constexpr std::size_t embedded_file_size = {array_size};
"""

    # Write to output header
    with open(output_header, "w") as f:
        f.write(header_content)

    print(f"Embedded {input_file} -> {output_header}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: embed_file.py <input_file> <output_header>")
        sys.exit(1)

    embed_file(sys.argv[1], sys.argv[2])
