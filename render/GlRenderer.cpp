#include "GlRenderer.h"

#define GLT_IMPLEMENTATION

#define GLT_DEBUG
#define GLT_DEBUG_PRINT

#include "../gltext.h"

GlRenderer::GlRenderer()
    : m_objs()
    , m_scene(nullptr)
    , m_shaderProgram()
    , m_vertexesVAO(0)
    , m_vertexesVBO(0)
    , m_procHint()
    , m_procHintObj(nullptr)
    , m_viewportWidth(0.0f)
    , m_viewportHeight(0.0f)
{
}

void GlRenderer::AddShaderProgram(const ShaderProgram &shaderProgram)
{
    m_shaderProgram = shaderProgram;
}

bool GlRenderer::Init()
{
    bool res = true;
    res = InitGLText();
    InitVertexesVAO();
    return res;
}

void GlRenderer::AddScene(IScene *scene)
{
    m_scene = scene;
}

void GlRenderer::AddProcHintObj(std::shared_ptr<ProcTextHint> hintObj) {
    m_procHintObj = hintObj;
}

void GlRenderer::Draw()
{
    if (!m_scene)
        return;

    auto &&drawableObjs = m_scene->GetAllDrawableObjs();
    m_objs.insert(m_objs.end(), drawableObjs.begin(), drawableObjs.end());

    auto normalizePoint3D = [this](const MPoint3D& point) {
        MPoint3D newPoint = point;
        auto wHalf = m_viewportWidth / 2;
        auto hHalf = m_viewportHeight / 2;
        newPoint.x = (newPoint.x - wHalf) / wHalf;
        newPoint.y = (newPoint.y - hHalf) / hHalf;
        newPoint.y *= -1;
        return newPoint;
    };

    std::vector<float> vertexData;
    for (auto &&obj : m_objs)
    {
        auto &&vertexes = obj->GetVertexes();
        std::for_each(vertexes.begin(), vertexes.end(), [&vertexData, &normalizePoint3D](const MVertex3D &vertex)
        {
            auto point = normalizePoint3D(vertex.GetPoint());

            vertexData.push_back(point.x);
            vertexData.push_back(point.y);
            //vertexData.push_back(point.z);
        });
    }

    glBindVertexArray(m_vertexesVAO);
    // поч без этой строки не работает? VBO автоматически разве не биндится?
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexesVBO);

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    m_shaderProgram.Use();

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(3.5f);
    glDrawArrays(GL_POINTS, 0, vertexData.size() / 2);

    glBindVertexArray(0);
    m_objs.clear();

    DrawText();
    std::cout << "error: " << glGetError() << std::endl;
}

void GlRenderer::DrawText() {
    gltBeginDraw();

    auto && hint = m_procHintObj->GetText();
    gltSetText(m_procHint, hint.c_str());

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(m_procHint, 0.0f, 0.0f, 1.0f);

    gltEndDraw();
}


void GlRenderer::SetViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

GlRenderer::~GlRenderer()
{
    glDeleteVertexArrays(1, &m_vertexesVAO);
    glDeleteBuffers(1, &m_vertexesVBO);

    gltDeleteText(m_procHint);
    gltTerminate();
}


void GlRenderer::InitVertexesVAO()
{
    glGenVertexArrays(1, &m_vertexesVAO);
    glBindVertexArray(m_vertexesVAO);

    glGenBuffers(1, &m_vertexesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexesVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    std::cout << "init error: " << glGetError() << std::endl;
}

bool GlRenderer::InitGLText() {
    bool res = gltInit();
    if (!res)
        return false;
    m_procHint = gltCreateText();
    gltSetText(m_procHint, "");
    return true;
}
