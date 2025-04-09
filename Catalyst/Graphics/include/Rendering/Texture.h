//  ***************************************************************
//  Texture - Creation date: 6/8/2024 3:39:06 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <string>

using std::string;

namespace Catalyst
{
	class DLL Texture
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
		Texture();
		explicit Texture(const char* _fileName);
		explicit Texture(unsigned int _w, unsigned int _h, EFormat _format, const unsigned char* _pixels = nullptr);
		virtual ~Texture();

		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;

	public:
		// load a jpg, bmp, png or tga
		bool Load(const char* _fileName);

		// creates a texture that can be filled in with pixels
		void Create(unsigned int _w, unsigned int _h, EFormat _format, const unsigned char* _pixels = nullptr);

		// returns the filename or "none" if not loaded from a file
		[[nodiscard]] const string& GetFilename() const { return m_filename; }

		// binds the texture to the specified slot
		int Bind(unsigned int _slot) const;

		// returns the opengl texture handle
		[[nodiscard]] uint GetHandle() const { return m_glHandle; }

		[[nodiscard]] uint GetWidth() const { return m_width; }
		[[nodiscard]] uint GetHeight() const { return m_height; }
		[[nodiscard]] uint GetFormat() const { return m_format; }
		[[nodiscard]] const unsigned char* GetPixels() const { return m_loadedPixels; }

	public:
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;

	protected:
		string m_filename;
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_glHandle;
		unsigned int m_format;
		unsigned char* m_loadedPixels;


	};
}