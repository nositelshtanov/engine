#pragma once

#include <memory>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
//#include "../gltext.h"

#include "IRenderer.h"
#include "IDrawable.h"
#include "../shaderProgramCreator.h"
#include "../Processes/ProcTextHint.h"

struct GLTtext;

class GlRenderer : public IRenderer
{
    std::vector<std::shared_ptr<IDrawable>> m_objs;
    IScene * m_scene;
    ShaderProgram m_shaderProgram;
    unsigned int m_vertexesVAO;
    unsigned int m_vertexesVBO;
    unsigned int m_edgesVAO;
    unsigned int m_edgesVBO;
    GLTtext* m_procHint;
    std::shared_ptr<ProcTextHint> m_procHintObj;
    float m_viewportWidth;
    float m_viewportHeight;
public:
    GlRenderer();

    void AddShaderProgram(const ShaderProgram& shaderProgram);
    bool Init();
    void AddScene(IScene* scene);
    void AddProcHintObj(std::shared_ptr<ProcTextHint> hintObj);
    void Draw();

    void SetViewportSize(float width, float height);

    virtual ~GlRenderer() override;

private:
    void DrawText();

    void InitVertexesVAO();
    void InitEdgesVAO();
    bool InitGLText();
};