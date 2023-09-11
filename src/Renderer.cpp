#include "Renderer.h"

namespace OGLR
{
	Renderer::Renderer()
	{
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::render(const MeshComponent& mesh)
	{
		mesh.shader->bind();
		mesh.va.bind();
		mesh.ib.bind();

		if (mesh.texture)
			mesh.texture->bind();

		glDrawElements(GL_TRIANGLES, mesh.ib.getCount(), GL_UNSIGNED_INT, nullptr);
	}

}