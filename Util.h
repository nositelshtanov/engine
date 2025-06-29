#pragma once

#include <functional>

class RaiiWrapper {
    using Callback = std::function<void()>;
    Callback m_onDestruct;
public:
    RaiiWrapper(Callback onCreate, Callback onDestruct)
        : m_onDestruct(onDestruct)
    {
        onCreate();
    }
    ~RaiiWrapper() {
        m_onDestruct();
    }
};
