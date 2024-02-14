#pragma once

#include "Buffer.h"
#include "../renderer/Vertex.h"

#include <type_traits>

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
         * Binds the attributes of the template parameter Vertex to this VertexArray
         * !!! This VAO and the associated VBO need to be bound before calling this method
         * @tparam V (Vertex<Attrib...>) The vertex template to bind
         */
        template<typename  V>
        void bindAttributes() const {
            [this]<typename...Ts>(std::__type_identity<Vertex<Ts...>>){
                uint32_t i = 0, offset = 0;
                (this->addAttrib<Ts>(i++, sizeof(Vertex<Ts ...>), offset), ...);
            }(std::__type_identity<V>());
        }

		void bind() const;
		static void unBind();

	private:
        template<typename A>
        void addAttrib(uint32_t idx, uint32_t stride, uint32_t& offset) const;

		GLuint m_RendererID;
	};

}