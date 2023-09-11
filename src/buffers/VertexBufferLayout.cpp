#include "VertexBufferLayout.h"

namespace OGLR::Buffers
{
	void VertexBufferLayout::addFloat(GLsizei count)
	{
		VertexAttribute attrib = { count, GL_FLOAT, GL_TRUE };
		m_Attributes.push_back(attrib);
		m_Stride += count * VertexAttribute::getSizeOfType(GL_FLOAT);
	}
	
}