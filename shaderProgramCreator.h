#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <limits>
#include <algorithm>

class ShaderLoader
{
    std::string m_filePath;
    std::string m_shaderContent;
    bool m_debug;
public:
    ShaderLoader(std::string_view filePath)
        : m_filePath(filePath)
        , m_shaderContent()
        , m_debug(false)
    {}
    ShaderLoader()
        : m_filePath()
        , m_shaderContent()
        , m_debug(false)
    {}

    bool LoadShader()
    {
        bool res = false;
        if (m_filePath.empty())
            return res;

        std::ifstream in(m_filePath);
        if (!in.is_open())
            return res;

        std::string line;
        while (std::getline(in, line))
        {
            m_shaderContent += line + "\n";
            res = true;
            if (m_debug)
                std::cout << line << "\n";
        }

        in.close();
        if (m_debug)
            std::cout << std::endl; 

        return res;
    }
    std::string_view GetShaderContent() const 
    {
        return m_shaderContent;
    }
    std::string ReleaseShaderContent() 
    {
        return std::move(m_shaderContent); 
    }
    void SetTarget(std::string_view shaderPath)
    {
        m_filePath = shaderPath;
        m_shaderContent.clear();
    }
    void SetDebug(bool val)
    {
        m_debug = val;
    }
};

class ShaderProgram
{
    unsigned int m_shaderProgram;
    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;

    static constexpr unsigned int s_invalidId = std::numeric_limits<unsigned int>::max();
public:
    ShaderProgram()
        : m_shaderProgram(s_invalidId)
        , m_vertexShader(s_invalidId)
        , m_fragmentShader(s_invalidId)
    {
    }

    void InitProgram()
    {
        m_shaderProgram = glCreateProgram();
    }

    void AddVertexShader(std::string_view VSPath)
    {
        if (m_shaderProgram == s_invalidId)
            return;
        ShaderLoader shaderLoader(VSPath);
        shaderLoader.LoadShader();
        auto && VSContent = shaderLoader.ReleaseShaderContent();

        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const auto * VSContentPtr = VSContent.c_str();
        glShaderSource(m_vertexShader, /*std::count(VSContent.cbegin(), VSContent.cend(), '\n')*/ 1, &VSContentPtr, nullptr);
        glCompileShader(m_vertexShader);
        
        glAttachShader(m_shaderProgram, m_vertexShader);
    } 

    void AddFragmentShader(std::string_view FSPath)
    {
        if (m_shaderProgram == s_invalidId)
            return;
        ShaderLoader shaderLoader(FSPath);
        shaderLoader.LoadShader();
        auto && FSContent = shaderLoader.ReleaseShaderContent();

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const auto * FSContentPtr = FSContent.c_str();
        glShaderSource(m_fragmentShader, /*std::count(FSContent.cbegin(), FSContent.cend(), '\n')*/1, &FSContentPtr, nullptr);
        glCompileShader(m_fragmentShader);
        
        glAttachShader(m_shaderProgram, m_fragmentShader);
    }

    bool LinkProgram()
    {
        if (m_shaderProgram == s_invalidId)
            return false;
        glLinkProgram(m_shaderProgram);
        int success;
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        char infoLog[512] = {0};
        if (!success)
        {
            glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
            std::cout << infoLog << std::endl;
        }
        return success;
    }

    void Use() const 
    {
        if (m_shaderProgram == s_invalidId)
            return;
        glUseProgram(m_shaderProgram);
    } 

    ~ShaderProgram()
    {
        if (m_vertexShader != s_invalidId)
            glDeleteShader(m_vertexShader);
        
        if (m_fragmentShader != s_invalidId)
            glDeleteShader(m_fragmentShader);
    }
};