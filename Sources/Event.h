#pragma once

#include "Buffer.h"

#include <vector>
#include <memory>

namespace EL
{
    class Event
    {
    public:
        class Handler
        {
        public:
            virtual ~Handler() = default;

        public:
            virtual void Run(const Buffer& buf) = 0;

        };

        template<typename ... ArgTypes>
        class FunctionHandler : public Handler
        {
        public:
            FunctionHandler(void (*func)(ArgTypes...))
            {
                m_func = func;
            }

            virtual void Run(const Buffer& buf) override
            {
                size_t offset = 0;
                m_func(Buffer::OffsetReader<ArgTypes>::Read(buf, offset)...);
            }

        private:
            void (*m_func)(ArgTypes...);
        };

    public:
        template<typename ... ArgTypes>
        std::shared_ptr<Handler> Add(void (*func)(ArgTypes...))
        {
            auto handler = std::make_shared<FunctionHandler<ArgTypes...>>(func);
            AddHandler(handler);
            return handler;
        }

        template<typename ... ArgTypes>
        void Run(ArgTypes ... args)
        {
            Buffer buf(128);
            buf.Write(args...);

            RunHandlers(buf);
        }

    protected:
        void AddHandler(const std::shared_ptr<Handler>& handler);
        void RunHandlers(const Buffer& buf);

    private:
        std::vector<std::shared_ptr<Handler>> m_handlers;
    };
}