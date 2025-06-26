#pragma once

#include <memory>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "IRenderer.h"
#include "IDrawable.h"
#include "../shaderProgramCreator.h"

class GlRenderer : public IRenderer
{
    std::vector<std::shared_ptr<IDrawable>> m_objs;
    IScene * m_scene;
    ShaderProgram m_shaderProgram;
    unsigned int m_vertexesVAO;
    unsigned int m_vertexesVBO;
public:
    GlRenderer();

    void AddShaderProgram(const ShaderProgram& shaderProgram);
    void Init();
    void AddScene(IScene* scene);
    void Draw();

    virtual ~GlRenderer() override;

private:
    void InitVertexesVAO();
};