#pragma once

#include <GL/glew.h>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>
// sfml 2.6

#include <algorithm>
#include <memory>

#include "shaderProgramCreator.h"
#include "render/GlRenderer.h"
#include "EventBus/EventBus.h"
#include "EventBus/Event.h"
#include "Editor3D.h"
#include "Processes/ProcessManager.h"
#include "Processes/PrMain.h"

class Application
{
    sf::Window m_window;
    ShaderProgram m_shaderProgram;
    EventBus& m_eventBus; 
    ProcessManager m_prManager;
    Editor3D m_editor;
    std::shared_ptr<PrMain> m_mainProc;
    GlRenderer m_renderer;

public:
    Application(int argc, char ** argv);
    
    bool Init();
    int Run();
    void AppClose();

    ~Application();
    
private:
    void CreateWindowContext();
    void LoadShaders();
    void CollectEvents();
};