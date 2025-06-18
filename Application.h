#pragma once

#include <GL/glew.h>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>
// sfml 2.6

#include <algorithm>

#include "shaderProgramCreator.h"
#include "EventBus/EventBus.h"
#include "EventBus/Event.h"
#include "Editor3D.h"
#include "Processes/ProcessManager.h"
#include "Processes/PrMain.h"

class Application
{
    sf::Window m_window;
    ShaderProgram m_shaderProgram;
    EventBus m_eventBus; 
    ProcessManager m_prManager;
    Editor3D m_editor;
    PrMain m_mainProc;

public:
    Application(int argc, char ** argv)
        : m_window()
        , m_shaderProgram()
        , m_eventBus()
        , m_prManager(m_eventBus)
        , m_editor(m_prManager)
        , m_mainProc(m_editor)
    {
    }
    bool Init()
    {
        CreateWindowContext(); 
        m_window.setActive(true);
        glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
        m_shaderProgram.InitProgram();
        LoadShaders();

        auto && reqEventTypes = m_mainProc.GetRequiredEventTypes();
        for (auto && eventType : reqEventTypes)
            m_eventBus.Subscribe(m_mainProc, eventType);

        return true;
    }
    int Run()
    {
        std::vector<float> vertices = 
        {
            -0.9, 0.9f,
            0.0f, 0.0f
        };

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        while (m_window.isOpen())
        {
            CollectEvents(); 
            m_eventBus.ProcessEvents();

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shaderProgram.Use();
            glBindVertexArray(vao);
            //glLineWidth(5);
            glPointSize(3.5f);
            glDrawArrays(GL_POINTS, 0, vertices.size() / 2);
            glBindVertexArray(0);

            m_window.display();
        }

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        return 0;
    }
private:
    void CreateWindowContext()
    {
        sf::ContextSettings contextSettings;
        contextSettings.majorVersion = 4;
        contextSettings.minorVersion = 6;
        contextSettings.attributeFlags = sf::ContextSettings::Attribute::Core;

        m_window.create(sf::VideoMode::getDesktopMode(), "MyCAD", sf::Style::Default, contextSettings);

        glewInit();
    }
    void LoadShaders()
    {
        m_shaderProgram.AddVertexShader("vertexshader.vs");
        m_shaderProgram.AddFragmentShader("fragmentshader.fs");
        if (!m_shaderProgram.LinkProgram())
            std::cout << "link shader program error" << std::endl;
        m_shaderProgram.Use();
    }
    void CollectEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
            {
                m_mainProc.Stop();
                m_window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code = sf::Keyboard::P)
                    m_eventBus.PostEvent(std::make_unique<KeyboardEvent>(KeyboardEvent::P));
                else if (event.key.code = sf::Keyboard::M)
                    m_eventBus.PostEvent(std::make_unique<KeyboardEvent>(KeyboardEvent::M));
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    m_eventBus.PostEvent(std::make_unique<MouseEvent>(event.mouseButton.x, event.mouseButton.y, MouseEvent::Button::Left, MouseEvent::Action::Press));
            }
            default:
                break;
            }
        }
    }
};