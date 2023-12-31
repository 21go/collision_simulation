#include "Particle.hpp"
#include <cmath>
#include <iostream>

Particle::Particle(int id, float pos_x, float pos_y, float vel_x, float vel_y, float acceleration_x, float acceleration_y, float mass, float radius)
{
    particle_id = id;
    pos.x = pos_x;
    pos.y = pos_y;
    velocity.x = vel_x;
    velocity.y = vel_y;
    acceleration.x = acceleration_x;
    acceleration.y = acceleration_y;
    this->mass = mass;
    this->radius = radius;

    s.setPosition(pos);
    sf::Color randomColor(std::rand() % 256, std::rand() % 256, std::rand() % 256);
    s.setFillColor(randomColor);
    s.setRadius(radius);
}

void Particle::set_color(sf::Color color)
{
    s.setFillColor(color);
}

void Particle::render(sf::RenderWindow &window)
{
    s.setPosition(pos);
    window.draw(s);
}

void Particle::update(float screen_x, float screen_y)
{
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    pos.x += velocity.x;
    pos.y += velocity.y;
    enforce_screen_boundaries(screen_x, screen_y);
}

void Particle::set_velocity(float vel_x, float vel_y)
{
    velocity.x = vel_x;
    velocity.y = vel_y;
}

void Particle::enforce_screen_boundaries(float boundary_x, float boundary_y)
{
    float offset = 42;
    if ((pos.x - radius <= offset) || (pos.x + radius >= boundary_x + offset))
    {
        velocity.x = -velocity.x;
    }

    if ((pos.y - radius <= offset) || (pos.y + radius >= boundary_y + offset))
    {
        velocity.y = -velocity.y;
    }
}

void Particle::apply_force(sf::Vector2f force)
{
    float inverse_mass = 1.0f / mass;
    acceleration.x += force.x * inverse_mass;
    acceleration.y += force.y * inverse_mass;
}
