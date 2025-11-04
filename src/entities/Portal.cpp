#include "Portal.h"

bool Portal::init()
{
    m_body.setRadius(Radius);
    m_body.setOrigin({Radius, Radius});
    m_body.setFillColor(Color::Purple);
    m_body.setPosition(m_position);
    m_body.scale({0.7f, 1.2f});
    return true;
}

void Portal::update(float dt)
{
    m_position.x -= m_speed * dt;
    m_body.setPosition(m_position);
}

void Portal::render(sf::RenderTarget& target) const
{
    target.draw(m_body);
}

void Portal::setDestination(Biome::Type dest)
{
    m_destination = dest;
    if (dest == Biome::Type::Abyss)
        m_body.setFillColor(Color::Purple);
    else
         m_body.setFillColor(Color::Green);
}
