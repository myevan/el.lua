#include "Event.h"

namespace EL
{
    void Event::AddHandler(const std::shared_ptr<Handler>& handler)
    {
        m_handlers.push_back(handler);
    }

    void Event::RunHandlers(const Buffer& buf)
    {
        for (std::shared_ptr<Handler>& handler: m_handlers)
        {
            handler->Run(buf);
        }
    }
}