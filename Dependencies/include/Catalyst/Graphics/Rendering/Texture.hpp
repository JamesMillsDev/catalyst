#pragma once

#include <Catalyst/Catalyst.hpp>
#include <string>

using std::string;

namespace Catalyst
{
	// a class for wrapping up an OpenGL texture image
	class Texture
	{
	public:
		enum EFormat : unsigned int
		{
			Red = 1,
			Rg,
			Rgb,
			Rgba
		};

	public:
		DLL Texture();
		DLL Texture(const char* _fileName);
		DLL Texture(unsigned int _w, unsigned int _h, EFormat _format, const unsigned char* _pixels = nullptr);
		DLL virtual ~Texture();

	public:
		// load a jpg, bmp, png or tga
		DLL bool Load(const char* _fileName);

		// creates a texture that can be filled in with pixels
		DLL void Create(unsigned int _w, unsigned int _h, EFormat _format, const unsigned char* _pixels = nullptr);

		// returns the filename or "none" if not loaded from a file
		[[nodiscard]] const string& GetFilename() const { return m_filename; }

		// binds the texture to the specified slot
		DLL void Bind(unsigned int _slot) const;

		// returns the opengl texture handle
		[[nodiscard]] uint32 GetHandle() const { return m_glHandle; }

		[[nodiscard]] uint32 GetWidth() const { return m_width; }
		[[nodiscard]] uint32 GetHeight() const { return m_height; }
		[[nodiscard]] uint32 GetFormat() const { return m_format; }
		[[nodiscard]] const unsigned char* GetPixels() const { return m_loadedPixels; }

	protected:
		string m_filename;
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_glHandle;
		unsigned int m_format;
		unsigned char* m_loadedPixels;
	};
}
