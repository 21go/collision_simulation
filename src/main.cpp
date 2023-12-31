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

    int id = 0;
    std::vector<Particle> particles;
    // for (int i = 0; i < 1000; i++)
    // {
    //     float random_x = static_cast<float>(rand()) / RAND_MAX * 10.0 - 5.0;
    //     float random_y = static_cast<float>(rand()) / RAND_MAX * 10.0 - 5.0;
    //     float magnitude = std::sqrt(random_x * random_x + random_y * random_y);
    //     random_x /= magnitude;
    //     random_y /= magnitude;

    //     // float random_size = static_cast<float>(std::rand()) / RAND_MAX * 28.0 + 4.0;

    //     particles.push_back(Particle(id++, i + 100, 200, random_x * 4, random_y * 4, 0, 0, 4, 4));
    // }
    // std::cout << particles.size() << std::endl;
    
    float boundary_x = screen_x - 100, boundary_y = screen_y - 100;
    sf::RectangleShape boundaries(sf::Vector2f(boundary_x, boundary_y));
    boundaries.setPosition(50, 50);

    boundaries.setOutlineThickness(2);
    boundaries.setOutlineColor(sf::Color::Red);
    boundaries.setFillColor(sf::Color::Black);

    sf::Clock spawnTimer;
    float spawnDelay = 0.1;

    while (window.isOpen())
    {
        float mouse_x = sf::Mouse::getPosition(window).x + mouse_offset_x;
        float mouse_y = sf::Mouse::getPosition(window).y + mouse_offset_y;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
            {
                isPaused = !isPaused;
            }
        }

        if (isPaused)
        {
            continue;
        }

        // Check if the mouse button is held down and enough time has passed since the last spawn
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && spawnTimer.getElapsedTime().asSeconds() > spawnDelay)
        {
            float random_x = static_cast<float>(rand()) / RAND_MAX * 10.0 - 5.0;
            float random_y = static_cast<float>(rand()) / RAND_MAX * 10.0 - 5.0;
            float magnitude = std::sqrt(random_x * random_x + random_y * random_y);
            random_x /= magnitude;
            random_y /= magnitude;

            float random_size = static_cast<float>(std::rand()) / RAND_MAX * 28.0 + 4.0;

            particles.push_back(Particle(id++, mouse_x, mouse_y, random_x * 4, random_y * 4, 0, 0, random_size, random_size));

            // Restart the timer
            spawnTimer.restart();
        }

        window.clear();
        window.draw(boundaries);


		auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
		{
			return fabs((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) <= (r1 + r2)*(r1 + r2);
		};
        
		std::vector<std::pair<Particle*, Particle*>> vecCollidingPairs;
        
		for (auto &ball : particles)
		{
			for (auto &target : particles)
			{
				if (ball.particle_id != target.particle_id)
				{
					if (DoCirclesOverlap(ball.pos.x, ball.pos.y, ball.radius, target.pos.x, target.pos.y, target.radius))
					{
						// Collision has occured
						vecCollidingPairs.push_back({ &ball, &target });

						// Distance between ball centers
						float fDistance = sqrtf((ball.pos.x - target.pos.x)*(ball.pos.x - target.pos.x) + (ball.pos.y - target.pos.y)*(ball.pos.y - target.pos.y));

						// Calculate displacement required
						float fOverlap = 0.5f * (fDistance - ball.radius - target.radius);

						// Displace Current Ball away from collision
						ball.pos.x -= fOverlap * (ball.pos.x - target.pos.x) / fDistance;
						ball.pos.y -= fOverlap * (ball.pos.y - target.pos.y) / fDistance;

						// Displace Target Ball away from collision
						target.pos.x += fOverlap * (ball.pos.x - target.pos.x) / fDistance;
						target.pos.y += fOverlap * (ball.pos.y - target.pos.y) / fDistance;
					}
				}
			}
		}

		// Now work out dynamic collisions
		for (auto c : vecCollidingPairs)
		{
			Particle *b1 = c.first;
			Particle *b2 = c.second;

			// Distance between balls
			float fDistance = sqrtf((b1->pos.x - b2->pos.x)*(b1->pos.x - b2->pos.x) + (b1->pos.y - b2->pos.y)*(b1->pos.y - b2->pos.y));

			// Normal
			float nx = (b2->pos.x - b1->pos.x) / fDistance;
			float ny = (b2->pos.y - b1->pos.y) / fDistance;

			// Tangent
			float tx = -ny;
			float ty = nx;

			// Dot Product Tangent
			float dpTan1 = b1->velocity.x * tx + b1->velocity.y * ty;
			float dpTan2 = b2->velocity.x * tx + b2->velocity.y * ty;

			// Dot Product Normal
			float dpNorm1 = b1->velocity.x * nx + b1->velocity.y * ny;
			float dpNorm2 = b2->velocity.x * nx + b2->velocity.y * ny;

			// Conservation of momentum in 1D
			float m1 = (dpNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
			float m2 = (dpNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dpNorm1) / (b1->mass + b2->mass);

			// Update ball velocities
			b1->velocity.x = tx * dpTan1 + nx * m1;
			b1->velocity.y = ty * dpTan1 + ny * m1;
			b2->velocity.x = tx * dpTan2 + nx * m2;
			b2->velocity.y = ty * dpTan2 + ny * m2;
		}


        for (int i = 0; i < particles.size(); i++)
        {
            // sf::Vector2f gravity_vector(0.0f, 0.5f);
            // particles[i].apply_force(gravity_vector);
            particles[i].update(boundary_x, boundary_y);
            particles[i].render(window);
        }

        window.display();
    }

    return 0;
}