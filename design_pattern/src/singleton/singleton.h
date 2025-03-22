#pragma once

#include <mutex>
#include <iostream>

class D
{
private:
    static D* m_instance;
    static D* getInstance();
private:
    D() = default;
    D(const D &d) = delete;
    D operator=(const D &d) = delete;
};