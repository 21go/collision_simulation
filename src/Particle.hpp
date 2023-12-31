#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>

class Particle
{
    sf::Vector2f acceleration;
    float strength;
    sf::CircleShape s;

public:
    int particle_id;
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f pos;
    float radius;
    Particle(int, float pos_x, float pos_y, float vel_x, float vel_y, float acceleration_x, float acceleration_y, float mass, float radius);

    void set_color(sf::Color color);

    sf::Vector2f get_position();

    void set_velocity(float vel_x, float vel_y);
    
    void render(sf::RenderWindow &window);

    void update(float screen_x, float screen_y);

    void apply_force(sf::Vector2f force);

    void enforce_screen_boundaries(float screen_x, float screen_y);
};

#endif // PARTICLE_HPP
