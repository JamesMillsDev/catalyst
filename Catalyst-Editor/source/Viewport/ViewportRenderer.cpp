#include "Viewport/ViewportRenderer.h"

#include <gl/glew.h>
#include <iostream>

namespace Catalyst
{
	ViewportRenderer::ViewportRenderer(int width, int height)
	{
        Resize(width, height);
	}
	
	ViewportRenderer::~ViewportRenderer()
	{
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteTextures(1, &m_texture);
        glDeleteRenderbuffers(1, &m_rbo);
	}

	void ViewportRenderer::Resize(int width, int height)
	{
        if (m_fbo)  // Delete existing buffers if resizing
        {
            glDeleteFramebuffers(1, &m_fbo);
            glDeleteTextures(1, &m_texture);
            glDeleteRenderbuffers(1, &m_rbo);
        }

        // Create framebuffer
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        // Create texture
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

        // Create renderbuffer (for depth & stencil)
        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ViewportRenderer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ViewportRenderer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}