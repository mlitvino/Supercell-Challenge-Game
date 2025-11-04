#include "PowerUp.h"
#include <SFML/Graphics/RenderTarget.hpp>

PowerUp::PowerUp(Type type)
    : m_type(type)
{
}

bool PowerUp::init()
{
    m_collisionRadius = radius;
    m_shape.setRadius(radius);
    m_shape.setOrigin({radius, radius});
    applyType();
    m_shape.setPosition(m_position);
    return true;
}

void PowerUp::update(float dt)
{
    m_position.x -= speed * dt;
    m_shape.setPosition(m_position);
}

void PowerUp::render(sf::RenderTarget& target) const
{
    target.draw(m_shape);
}

void PowerUp::setType(Type t)
{
    m_type = t;
    applyType();
}

PowerUp::Type PowerUp::getType() const
{
    return m_type;
}

sf::FloatRect PowerUp::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void PowerUp::applyType()
{
    switch (m_type)
    {
        case Type::Shield: m_shape.setFillColor(Color::Shield); break;
        case Type::Bomb:   m_shape.setFillColor(sf::Color(220, 20, 60)); break;
    }
}
