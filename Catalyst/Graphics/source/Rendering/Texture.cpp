#include "cagraphicspch.h"
#include "Rendering/Texture.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Catalyst
{
	Texture::Texture()
		: m_filename{ "none" }, m_width{ 0 }, m_height{ 0 },
		m_glHandle{ 0 }, m_format{ 0 }, m_loadedPixels{ nullptr }
	{
	}

	Texture::Texture(const char* _fileName)
		: m_filename{ "none" }, m_width{ 0 }, m_height{ 0 },
		m_glHandle{ 0 }, m_format{ 0 }, m_loadedPixels{ nullptr }
	{
		Load(_fileName);
	}

	Texture::Texture(const unsigned int _w, const unsigned int _h, const EFormat _format, const unsigned char* _pixels)
		: m_filename{ "none" }, m_width{ _w }, m_height{ _h },
		m_format{ _format }, m_loadedPixels{ nullptr }
	{
		Create(_w, _h, _format, _pixels);
	}

	Texture::~Texture()
	{
		if (m_glHandle != 0)
			glDeleteTextures(1, &m_glHandle);

		if (m_loadedPixels != nullptr)
		{
			stbi_image_free(m_loadedPixels);
			m_loadedPixels = nullptr;
		}
	}

	bool Texture::Load(const char* _fileName)
	{
		if (m_glHandle != 0)
		{
			glDeleteTextures(1, &m_glHandle);
			m_glHandle = 0;
			m_width = 0;
			m_height = 0;
			m_filename = "none";

			if (m_loadedPixels != nullptr)
			{
				stbi_image_free(m_loadedPixels);
				m_loadedPixels = nullptr;
			}
		}

		int x = 0, y = 0, comp = 0;
		m_loadedPixels = stbi_load(_fileName, &x, &y, &comp, STBI_default);

		if (m_loadedPixels != nullptr)
		{
			glGenTextures(1, &m_glHandle);
			glBindTexture(GL_TEXTURE_2D, m_glHandle);
			switch (comp)
			{
			case STBI_grey:
				m_format = Red;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y, 0, GL_RED, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;

			case STBI_grey_alpha:
				m_format = Rg;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, x, y, 0, GL_RG, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;

			case STBI_rgb:
				m_format = Rgb;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;

			case STBI_rgb_alpha:
				m_format = Rgba;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;
			default:
				break;
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_width = static_cast<unsigned int>(x);
			m_height = static_cast<unsigned int>(y);
			m_filename = _fileName;

			return true;
		}

		return false;
	}

	void Texture::Create(const unsigned int _w, const unsigned int _h, const EFormat _format, const unsigned char* _pixels)
	{
		if (m_glHandle != 0)
		{
			glDeleteTextures(1, &m_glHandle);
			m_glHandle = 0;
			m_filename = "none";
		}

		m_width = _w;
		m_height = _h;
		m_format = _format;

		glGenTextures(1, &m_glHandle);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		switch (m_format)
		{
		case Red:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(m_width), static_cast<int>(m_height), 0, GL_RED, GL_UNSIGNED_BYTE, _pixels);
			break;

		case Rg:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, static_cast<int>(m_width), static_cast<int>(m_height), 0, GL_RG, GL_UNSIGNED_BYTE, _pixels);
			break;

		case Rgb:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(m_width), static_cast<int>(m_height), 0, GL_RGB, GL_UNSIGNED_BYTE, _pixels);
			break;

		case Rgba:
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(m_width), static_cast<int>(m_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);
			break;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int Texture::Bind(const unsigned int _slot) const
	{
		glActiveTexture(GL_TEXTURE0 + _slot);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);

		return _slot;
	}
}
