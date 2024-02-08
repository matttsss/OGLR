#pragma once

#include <GL/glew.h>
#include <iostream>

namespace OGLR::Buffers {

        enum BufferType {
            Index = GL_ELEMENT_ARRAY_BUFFER, Vertex = GL_ARRAY_BUFFER, Uniform = GL_UNIFORM_BUFFER
        };

        enum UsageType {
            Static = GL_STATIC_DRAW, Dynamic = GL_DYNAMIC_DRAW
        };

        template <BufferType BT, UsageType UT = UsageType::Static>
        class Buffer {
        public:

            Buffer(const void* data, GLuint size):
                    m_RendererId(0), m_Count(size) {
                glCreateBuffers(1, &m_RendererId);

                bind();
                glBufferData(BT, size, data, UT);
                unBind();
            }

            ~Buffer(){
                glDeleteBuffers(1, &m_RendererId);
            }

            void bind() const {
                glBindBuffer(BT, m_RendererId);
            }

            void unBind() const {
                glBindBuffer(BT, m_RendererId);
            }

            void setData(const void* data, GLuint size) const {
                if (UT == UsageType::Static)
                    std::cout << "Warning, dynamically modifying a static buffer, reconsider type if necessary." << std::endl;

                bind();
                glBufferData(BT, size, data, UT);
                unBind();

            }

            GLsizei getCount() const {
                static_assert(BT ==  BufferType::Index, "Cannot get count on a non-index buffer");
                return m_Count / sizeof(GLuint);
            }


        private:
            GLuint m_RendererId;
            uint32_t m_Count;

        };


} // OGLR::Buffers

