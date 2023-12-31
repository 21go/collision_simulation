#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <uuid/uuid.h>
#include "Particle.hpp"

int main()
{
    float screen_x = 800, screen_y = 600;

    sf::RenderWindow window(sf::VideoMode(screen_x, screen_y), "Collisions Simulation");
    window.setFramerateLimit(60);
    
    bool isPaused = false;

    float mouse_offset_x = -10;
    float mouse_offset_y = -10;

    std::vector<Particle> particles;
    // particles.push_back(Particle(-1, 200, 400, 4, 0, 0, 0, 1, 8));
    // particles.push_back(Particle(-2, 500, 500, -2, 2, 0, 0, 1, 8));
    
    float boundary_x = screen_x - 100, boundary_y = screen_y - 100;
    sf::RectangleShape boundaries(sf::Vector2f(boundary_x, boundary_y));
    boundaries.setPosition(50, 50);

    boundaries.setOutlineThickness(2);
    boundaries.setOutlineColor(sf::Color::Red);
    boundaries.setFillColor(sf::Color::Black);

    int id = 0;

    while (window.isOpen())
    {
        float mouse_x = sf::Mouse::getPosition(window).x + mouse_offset_x;
        float mouse_y = sf::Mouse::getPosition(window).y + mouse_offset_y;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    float random_x = static_cast<float>(rand()) / RAND_MAX * 10.0 - 5.0;
                    float random_y = static_cast<float>(rand()) / RAND_MAX * 10.0 - 5.0;
                    float magnitude = std::sqrt(random_x * random_x + random_y * random_y);
                    random_x /= magnitude;
                    random_y /= magnitude;

                    particles.push_back(Particle(id++, mouse_x, mouse_y, random_x * 4, random_y * 4, 0, 0, 1, 8));
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                isPaused = !isPaused;
            }
        }

        if (isPaused) continue;

        window.clear();
        window.draw(boundaries);

        for (int i = 0; i < particles.size(); i++)
        {
            // sf::Vector2f gravity_vector(0.0f, 0.5f);
            // particles[i].apply_force(gravity_vector);
            particles[i].update(boundary_x, boundary_y, particles);
            particles[i].render(window);
        }

        window.display();
    }

    return 0;
}