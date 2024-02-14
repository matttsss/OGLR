#pragma once

#include "Buffer.h"
#include "VertexBufferLayout.h"
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
         * Links the Vertex buffer layout to this VAO.
         * !!! The corresponding VB and this VAO need to be bound before calling this method
         * @param bufferLayout (VertexBufferLayout) Layout of the VB
         */
		void bindAttributes(const VertexBufferLayout& bufferLayout) const;

        template<typename A>
        void addAttrib(uint32_t idx, uint32_t stride, uint32_t& offset) const;

        //template<typename  V>
        //void bindAttributes() const {
        //    [this]<typename...Ts>(std::__type_identity<Vertex<Ts...>>){
        //        uint32_t i, offset = 0;
        //        (addAttrib<Ts>(i++, sizeof(Vertex<Ts ...>), offset), ...);
        //    }(std::__type_identity<V>());
        //}

		void bind() const;
		static void unBind();

	private:
		GLuint m_RendererID;
	};

}