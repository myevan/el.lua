#pragma once

#include <stdlib.h>
#include <stdint.h>

namespace EL
{
    class Buffer
    {
    public:
        Buffer(size_t capacity);
        ~Buffer();

    public:
        bool Write(int input);
        bool Read(size_t &offset, int &output) const;

    private:
        bool Write(size_t size, const uint8_t *bytes);
        bool Read(size_t &offset, size_t size, uint8_t *bytes) const;

    public:
        template <typename HeadType, typename... TailTypes>
        bool Write(HeadType head, TailTypes... tails)
        {
            Write(head);
            Write(tails...);
            return true;
        }

    public:
        template <typename RetType, typename = void>
        struct OffsetReader
        {
            static RetType Read(const Buffer &buf, size_t &offset)
            {
            }
        };

        template <typename Dummy>
        struct OffsetReader<int, Dummy>
        {
            static int Read(const Buffer &buf, size_t &offset)
            {
                int output;
                buf.Read(offset, output);
                return output;
            }
        };

    private:
        size_t m_capacity;
        size_t m_size;

    private:
        uint8_t *m_bytes;
    };
}