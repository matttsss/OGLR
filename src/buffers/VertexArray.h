#pragma once

#include "Buffer.h"
#include "VertexBufferLayout.h"

namespace OGLR
{

	class VertexArray
	{
	public:

		VertexArray();
        VertexArray(VertexArray&& other) noexcept;
        VertexArray(const VertexArray&) = delete;
		~VertexArray();

        /**
         * Links the Vertex buffer layout to this VAO.
         * !!! The corresponding VB and this VAO need to be bound before calling this method
         * @param bufferLayout (VertexBufferLayout) Layout of the VB
         */
		void bindAttributes(const VertexBufferLayout& bufferLayout) const;

		void bind() const;
		static void unBind();

	private:
		GLuint m_RendererID;
	};

}