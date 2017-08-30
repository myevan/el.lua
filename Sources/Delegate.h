#pragma once

#include <vector>
#include <memory>

namespace EL
{
    template<typename ... ArgTypes>
    class Delegate
    {
    public:
        using Func = std::function<void(ArgTypes...)>;

    public:
        void Add(Func func)
        {
            m_funcs.push_back(func);
        }

        void Remove(Func func)
        {
            m_funcs.remove(func);
        }

        virtual void Run(ArgTypes... args)
        {
            for (const Func& func: m_funcs)
            {
                func(args...);
            }
        }

    public:
        void operator+=(Func func)
        {
            Add(func);
        }

        void operator-=(Func func)
        {
            Remove(func);
        }

        void operator()(ArgTypes... args)
        {
            Run(args...);
        }

    private:
        std::vector<Func> m_funcs;
    };
}
