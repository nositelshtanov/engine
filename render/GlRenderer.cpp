#include "GlRenderer.h"

#define GLT_IMPLEMENTATION

#define GLT_DEBUG
#define GLT_DEBUG_PRINT
#define GLT_MANUAL_VIEWPORT

#include "../gltext.h"

GlRenderer::GlRenderer()
    : m_objs()
    , m_scene(nullptr)
    , m_shaderProgram()
    , m_vertexesVAO(0)
    , m_vertexesVBO(0)
    , m_edgesVAO(0)
    , m_edgesVBO(0)
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
    InitEdgesVAO();
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

    m_shaderProgram.Use();

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawVertices(m_objs);
    DrawEdges(m_objs);

    DrawText();
    m_objs.clear();
    //std::cout << "error: " << glGetError() << std::endl;
}

void GlRenderer::DrawEdges(const std::vector<std::shared_ptr<IDrawable>>& objs) {
    std::vector<float> edgesData;
    for (auto && obj : objs) {
        auto && edges = obj->GetEdges();
        for (auto && edge : edges) {
            auto begPoint = NormalizePoint(edge.GetGegVertex().GetPoint());
            auto endPoint = NormalizePoint(edge.GetEndVertex().GetPoint());

            edgesData.push_back(begPoint.x);
            edgesData.push_back(begPoint.y);
            edgesData.push_back(endPoint.x);
            edgesData.push_back(endPoint.y);
        }
    }

    glBindVertexArray(m_edgesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_edgesVBO);

    glBufferData(GL_ARRAY_BUFFER, edgesData.size() * sizeof(float), edgesData.data(), GL_STATIC_DRAW);

    glDrawArrays(GL_LINES, 0,  edgesData.size() / 2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlRenderer::DrawVertices(const std::vector<std::shared_ptr<IDrawable>>& objs) {
    std::vector<float> vertexData;
    for (auto && obj : objs) {
        auto &&vertexes = obj->GetVertexes();
        for (auto && vertex : vertexes) {
            auto point = NormalizePoint(vertex.GetPoint());

            vertexData.push_back(point.x);
            vertexData.push_back(point.y);
            //vertexData.push_back(point.z);
        }
    }

    glBindVertexArray(m_vertexesVAO);
    // поч без этой строки не работает? VBO автоматически разве не биндится?
    // UDP: да, только отрисовка достает из VAO VBO, а для настройки (в т.ч. буфер поменять) нужно биндить
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexesVBO);

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glPointSize(3.5f);
    glDrawArrays(GL_POINTS, 0, vertexData.size() / 2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


MPoint3D GlRenderer::NormalizePoint(const MPoint3D& point) const {
    MPoint3D newPoint = point;
    auto wHalf = m_viewportWidth / 2;
    auto hHalf = m_viewportHeight / 2;
    newPoint.x = (newPoint.x - wHalf) / wHalf;
    newPoint.y = (newPoint.y - hHalf) / hHalf;
    newPoint.y *= -1;
    return newPoint;
}

void GlRenderer::DrawText() {
    gltViewport(m_viewportWidth, m_viewportHeight);
    gltBeginDraw();

    auto && hint = m_procHintObj->GetText();
    gltSetText(m_procHint, hint.c_str());

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(m_procHint, 0.0f, 0.0f, 1.0f);

    gltEndDraw();
}


void GlRenderer::SetViewportSize(float width, float height) {
    if (width && height) {
        m_viewportWidth = width;
        m_viewportHeight = height;
    }
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
    std::cout << "init vertexes error: " << glGetError() << std::endl;
}

void GlRenderer::InitEdgesVAO() {
    glGenVertexArrays(1, &m_edgesVAO);
    glBindVertexArray(m_edgesVAO);

    glGenBuffers(1, &m_edgesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_edgesVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    std::cout << "init edges error: " << glGetError() << std::endl;
}

bool GlRenderer::InitGLText() {
    bool res = gltInit();
    if (!res)
        return false;
    m_procHint = gltCreateText();
    gltSetText(m_procHint, "");
    return true;
}
