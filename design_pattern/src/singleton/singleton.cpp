#include "singleton.h"

//C++ 11 版本之后的跨平台实现(volatile)
D* D::m_instance = nullptr;

std::mutex m_mutex;
D* D::getInstance()
{
    if (!m_instance) {
        std::lock_guard<std::mutex> lock(m_mutex);
        D* tmp = nullptr;
        if (!m_instance) {
            tmp = new D;
            std::atomic_thread_fence(std::memory_order_release); //释放内存fence
            m_instance = tmp;
        }
    }
    return m_instance;
};

