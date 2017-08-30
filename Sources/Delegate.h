#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <cassert>

namespace EL
{
    class HandleManager
    {
    public:
        uint32_t AllocHandle()
        {
            uint32_t handle;
            if (m_freeIndices.empty())
            {
                handle = GenerateHandle(m_totalHandles.size());
                m_totalHandles.push_back(handle);
            }
            else
            {
                handle = GenerateHandle(m_freeIndices.back());
                m_freeIndices.pop_back();
            }
            return handle;
        }

        bool TryFreeHandle(uint32_t handle, uint32_t& outIndex)
        {
            outIndex = GetHandleIndex(handle);
            if (m_totalHandles[outIndex] == handle)
            {
                m_totalHandles[outIndex] = 0;
                m_freeIndices.push_back(outIndex);
                return true;
            }
            else
            {
                return false;
            }
        }

        uint32_t GetHandleIndex(uint32_t handle)
        {
            return handle & 0xffffff;
        }

    private:
        uint32_t GenerateHandle(size_t index)
        {
            static uint32_t s_seq = 1;
            uint32_t handle = (s_seq << 24) | (index & 0xffffff);
            s_seq += 2;
            s_seq &= 0xff;
            return handle;
        }

    private:
        std::vector<uint32_t> m_totalHandles;
        std::vector<uint32_t> m_freeIndices;
    };

    class DelegateHelper
    {
    public:
        template<typename Func>
        static uint32_t Add(HandleManager& handleManager, std::vector<Func>& funcs, Func func)
        {
            uint32_t handle = handleManager.AllocHandle();
            uint32_t index = handleManager.GetHandleIndex(handle);
            if (index < funcs.size())
            {
                funcs[index] = func;
            }
            else
            {
                funcs.push_back(func);
            }
            return handle;
        }

        template<typename Func>
        static bool Remove(HandleManager& handleManager, std::vector<Func>& funcs, uint32_t handle)
        {
            uint32_t index;
            if (handleManager.TryFreeHandle(handle, index))
            {
                assert(index < funcs.size());
                funcs[index] = nullptr;
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    template<typename ... ArgTypes>
    class Delegate
    {
    public:
        using Func = std::function<void(ArgTypes...)>;

    public:
        uint32_t Add(Func func)
        {
            return DelegateHelper::Add(m_handleManager, m_funcs, func);
        }

        void Remove(uint32_t handle)
        {
            return DelegateHelper::Remove(m_handleManager, m_funcs, handle);
        }

        void Run(ArgTypes... args)
        {
            for (const Func& func : m_funcs)
            {
                if (func != nullptr)
                {
                    func(args...);
                }
            }
        }

    public:
        template<typename T>
        void operator()(ArgTypes... args)
        {
            Run(args...);
        }

    private:
        HandleManager m_handleManager;

    private:
        std::vector<Func> m_funcs;
    };

    template<>
    class Delegate<void>
    {
    public:
        using Func = std::function<void(void)>;

    public:
        uint32_t Add(Func func)
        {
            return DelegateHelper::Add(m_handleManager, m_funcs, func);
        }

        bool Remove(uint32_t handle)
        {
            return DelegateHelper::Remove(m_handleManager, m_funcs, handle);
        }

        void Run()
        {
            for (const Func& func : m_funcs)
            {
                if (func != nullptr)
                {
                    func();
                }
            }
        }

    public:
        void operator()()
        {
            Run();
        }

    private:
        HandleManager m_handleManager;

    private:
        std::vector<Func> m_funcs;
    };
}
