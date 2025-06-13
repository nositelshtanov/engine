#include <GL/glew.h>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>
// sfml 2.6

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 6;
    contextSettings.attributeFlags = sf::ContextSettings::Attribute::Core;

    sf::Window window(sf::VideoMode::getDesktopMode(), "MyCAD", sf::Style::Default, contextSettings);
    window.setActive(true);

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

        unsigned int vbo;
        glGenBuffers(1, &vbo);

        window.display();
    }

    return 0;
}