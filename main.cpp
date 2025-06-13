#include <GL/glew.h>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>
// sfml 2.6

#include <algorithm>

#include "shaderProgramCreator.h"

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 6;
    contextSettings.attributeFlags = sf::ContextSettings::Attribute::Core;

    sf::Window window(sf::VideoMode::getDesktopMode(), "MyCAD", sf::Style::Default, contextSettings);
    window.setActive(true);

    glewInit();

    glViewport(0, 0, window.getSize().x, window.getSize().y);

    ShaderProgram shaderProgram;
    shaderProgram.AddVertexShader("vertexshader.vs");
    shaderProgram.AddFragmentShader("fragmentshader.fs");
    if (!shaderProgram.LinkProgram())
        std::cout << "link shader program error" << std::endl;
    shaderProgram.Use();

    std::vector<float> vertices = 
    {
        //-1.0f, 0.0f, 0.0f,
        //0.0f, 1.0f, 0.0f,
        //1.0f, 0.0f, 0.0f
        -0.5f, 0.0f,
        0.0f, 0.2f,
        0.0f, 0.2f,
        0.3f, 0.8f 
    };


    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Use();
        glBindVertexArray(vao);
        glLineWidth(5);
        glDrawArrays(GL_LINES, 0, vertices.size() / 4);
        glBindVertexArray(0);

        window.display();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return 0;
}