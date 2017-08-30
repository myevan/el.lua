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

    template<typename ... ArgTypes>
        class Delegate
        {
            public:
                using Func = std::function<void(ArgTypes...)>;

            public:
                uint32_t Add(Func func)
                {
                    uint32_t handle = m_handleManager.AllocHandle();
                    uint32_t index = m_handleManager.GetHandleIndex(handle);
                    if (index < m_funcs.size())
                    {
                        m_funcs[index] = func;
                    }
                    else
                    {
                        m_funcs.push_back(func);
                    }

                    return handle;
                }

                void Remove(uint32_t handle)
                {
                    uint32_t index;
                    if (m_handleManager.TryFreeHandle(handle, index))
                    {
                        assert(index < m_funcs.size());
                        m_funcs[index] = nullptr;
                    }
                }

                virtual void Run(ArgTypes... args)
                {
                    for (const Func& func: m_funcs)
                    {
                        if (func != nullptr)
                        {
                            func(args...);
                        }
                    }
                }

            public:
                void operator()(ArgTypes... args)
                {
                    Run(args...);
                }

            private:
                HandleManager m_handleManager;

                std::vector<Func> m_funcs;

        };
}
