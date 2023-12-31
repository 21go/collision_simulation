#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>

class Particle
{
    int particle_id;
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float strength;
    float mass;
    sf::CircleShape s;
    float radius;

public:
    Particle(int, float pos_x, float pos_y, float vel_x, float vel_y, float acceleration_x, float acceleration_y, float mass, float radius);

    void set_color(sf::Color color);
    
    void render(sf::RenderWindow &window);

    void update(float screen_x, float screen_y, std::vector<Particle> particles);

    void apply_force(sf::Vector2f force);

    void particle_collisions(std::vector<Particle> particles);

    void enforce_screen_boundaries(float screen_x, float screen_y);
};

#endif // PARTICLE_HPP
