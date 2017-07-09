#pragma once

namespace EL
{
    class Logger
    {
    public:
        static Logger& Get();
        static void Set(Logger* inst);

    private:
        static Logger* _inst;

    public:
        virtual ~Logger() = default;
        virtual void Info(const char* text) = 0;
    };

}
