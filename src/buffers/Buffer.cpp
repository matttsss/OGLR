#include "Buffer.h"


namespace OGLR
{

    Buffer::Buffer(Buffer &&other) noexcept
        : m_RendererID(other.m_RendererID), m_Count(other.m_Count),
          m_BT(other.m_BT),                 m_UT(other.m_UT) {
        other.m_RendererID = 0;
    }

    Buffer::Buffer(BufType BT, const void *data, GLuint size, UsageType UT)
        : m_RendererID(0), m_Count(size), m_BT(BT), m_UT(UT) {
        glCreateBuffers(1, &m_RendererID);

        bind();
        glBufferData(m_BT, size, data, m_UT);
        unBind();
    }

    Buffer::~Buffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void Buffer::bind() const {
        glBindBuffer(m_BT, m_RendererID);
    }

    void Buffer::unBind() const {
        glBindBuffer(m_BT, 0);
    }

    void Buffer::setData(const void *data, GLuint size) const {
        if (m_UT == UsageType::Static)
            std::cout << "Warning, dynamically modifying a static buffer, reconsider type if necessary." << std::endl;
        glBufferData(m_BT, size, data, m_UT);
    }

    void Buffer::castTo(BufType BT, UsageType UT) {
        m_BT = BT;
        m_UT = UT;
    }


}