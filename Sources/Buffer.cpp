#include "Buffer.h"

#include <memory.h>

namespace EL
{
    Buffer::Buffer(size_t capacity)
    : m_capacity(capacity), m_size(0)
    {
        m_bytes = (uint8_t *)malloc(capacity);
    }

    Buffer::~Buffer()
    {
        free(m_bytes);
    }

    bool Buffer::Write(int input)
    {
        return Write(sizeof(input), (const uint8_t *)&input);
    }

    bool Buffer::Read(size_t &offset, int &output) const
    {
        return Read(offset, sizeof(output), (uint8_t *)&output);
    }

    bool Buffer::Write(size_t size, const uint8_t* bytes)
    {
        if (m_capacity < m_size + size)
            return false;

        memcpy((void*)m_bytes, (const void*)bytes, size);
        m_size += size;
        return true;
    }

    bool Buffer::Read(size_t& offset, size_t size, uint8_t* bytes) const
    {
        if (m_size < offset + size)
            return false;

        memcpy((void*)bytes, (const void*)m_bytes, size);
        return true;
    }

}