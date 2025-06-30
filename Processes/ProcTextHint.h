#pragma once

#include <string>

class ProcTextHint {
    std::string m_text;
public:
    ProcTextHint()
        : m_text()
    {}
    void SetText(const std::string& text) {
        m_text = text;
    }
    std::string GetText() const { return m_text; }
};