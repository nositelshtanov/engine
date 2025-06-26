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
    ShaderLoader(std::string_view filePath);
    ShaderLoader();

    bool LoadShader();
    
    std::string_view GetShaderContent() const;
    std::string ReleaseShaderContent();
    void SetTarget(std::string_view shaderPath);
    void SetDebug(bool val);
};

class ShaderProgram
{
    unsigned int m_shaderProgram;
    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;

    static constexpr unsigned int s_invalidId = std::numeric_limits<unsigned int>::max();
public:
    ShaderProgram();

    void InitProgram();

    void AddVertexShader(std::string_view VSPath);
    void AddFragmentShader(std::string_view FSPath);
    bool LinkProgram();
    void Use() const;

    ~ShaderProgram();
};