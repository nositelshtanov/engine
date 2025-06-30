#include "GlRenderer.h"

GlRenderer::GlRenderer()
    : m_objs()
    , m_scene(nullptr)
    , m_shaderProgram()
    , m_vertexesVAO(0)
    , m_vertexesVBO(0)
{
}

void GlRenderer::AddShaderProgram(const ShaderProgram &shaderProgram)
{
    m_shaderProgram = shaderProgram;
}

void GlRenderer::Init()
{
    InitVertexesVAO();
}

void GlRenderer::AddScene(IScene *scene)
{
    m_scene = scene;
}

void GlRenderer::Draw()
{
    if (!m_scene)
        return;

    auto &&drawableObjs = m_scene->GetAllDrawableObjs();
    m_objs.insert(m_objs.end(), drawableObjs.begin(), drawableObjs.end());

    auto normalizeDouble = [](double& num) {
        while (num > 1 || num < -1) {
            num /= 10;
        }
    };

    std::vector<float> vertexData;
    for (auto &&obj : m_objs)
    {
        auto &&vertexes = obj->GetVertexes();
        std::for_each(vertexes.begin(), vertexes.end(), [&vertexData, &normalizeDouble](const MVertex3D &vertex)
        {
            auto point = vertex.GetPoint(); 
            // придумать чо с потерей точности. Перевести тут в принципе надо в NDO
            normalizeDouble(point.x);
            normalizeDouble(point.y);
            normalizeDouble(point.z);
            vertexData.push_back(point.x);
            vertexData.push_back(point.y);
            //vertexData.push_back(point.z);
        });
    }

    glBindVertexArray(m_vertexesVAO);

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    m_shaderProgram.Use();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(3.5f);
    glDrawArrays(GL_POINTS, 0, vertexData.size() / 2);

    glBindVertexArray(0);
    m_objs.clear();
}

GlRenderer::~GlRenderer()
{
    glDeleteVertexArrays(1, &m_vertexesVAO);
    glDeleteBuffers(1, &m_vertexesVBO);
}

void GlRenderer::InitVertexesVAO()
{
    m_vertexesVAO;
    glGenVertexArrays(1, &m_vertexesVAO);
    glBindVertexArray(m_vertexesVAO);

    glGenBuffers(1, &m_vertexesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexesVBO);

    glBindVertexArray(0);
}