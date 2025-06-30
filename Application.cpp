#include "Application.h"

Application::Application(int argc, char **argv)
    : m_window()
    , m_shaderProgram()
    , m_eventBus(GetEventBus())
    , m_prManager(m_eventBus)
    , m_editor(m_prManager)
    , m_mainProc(std::make_shared<PrMain>(m_editor))
    , m_renderer()
{
}

bool Application::Init()
{
    CreateWindowContext();
    m_window.setActive(true);
    glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
    m_shaderProgram.InitProgram();
    LoadShaders();

    m_renderer.AddShaderProgram(m_shaderProgram);
    bool res = m_renderer.Init();
    if (!res)
        std::cout << "renderer Init FAILED" << std::endl;
    m_renderer.AddProcHintObj(m_editor.GetProcTextHint());

    return true;
}

int Application::Run()
{
    m_renderer.AddScene(&m_editor.GetCurScene());
    m_mainProc->Run();

    while (m_window.isOpen())
    {
        CollectEvents();
        m_eventBus.ProcessEvents();

        m_renderer.Draw();

        m_window.display();
    }

    return 0;
}

void Application::AppClose()
{
    m_mainProc->Stop();
    m_prManager.FinishWork();
    m_editor.FinishWork();
    m_window.close();
}

Application::~Application()
{
    delete &m_eventBus;
}

void Application::CreateWindowContext()
{
    sf::ContextSettings contextSettings;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 6;
    contextSettings.attributeFlags = sf::ContextSettings::Attribute::Core;

    m_window.create(sf::VideoMode::getDesktopMode(), "MyCAD", sf::Style::Default, contextSettings);

    glewInit();
}

void Application::LoadShaders()
{
    m_shaderProgram.AddVertexShader("vertexshader.vs");
    m_shaderProgram.AddFragmentShader("fragmentshader.fs");
    if (!m_shaderProgram.LinkProgram())
        std::cout << "link shader program error" << std::endl;
    m_shaderProgram.Use();
}

void Application::CollectEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
        {
            AppClose();
            break;
        }
        case sf::Event::KeyPressed:
        {
            std::cout << "keyPressed " << static_cast<int>(event.key.code) << std::endl;
            if (event.key.code == sf::Keyboard::P) // 15
            {
                m_eventBus.PostEvent(std::make_unique<KeyboardEvent>(KeyboardEvent::P));
            }
            else if (event.key.code == sf::Keyboard::M) // 12
            {
                m_eventBus.PostEvent(std::make_unique<KeyboardEvent>(KeyboardEvent::M));
            }
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            if (event.mouseButton.button == sf::Mouse::Left)
                m_eventBus.PostEvent(std::make_unique<MouseEvent>(event.mouseButton.x, event.mouseButton.y, MouseEvent::Button::Left, MouseEvent::Action::Press));
            break;
        }
        case sf::Event::Resized:
        {
            m_renderer.SetViewportSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            break;
        }
        default:
            break;
        }
    }
}