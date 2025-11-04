#include "Material.h"
#include "utils/Colors.h"
#include <SFML/Graphics/RenderTarget.hpp>

Material::Material(Type type)
    : m_type(type)
{
}

bool Material::init()
{
    m_collisionRadius = radius;
    m_shape.setRadius(radius);
    m_shape.setOrigin({radius, radius});
    applyType();
    m_shape.setPosition(m_position);
    return true;
}

void Material::update(float dt)
{
    m_position.x -= speed * dt;
    m_shape.setPosition(m_position);
}

void Material::render(sf::RenderTarget& target) const
{
    target.draw(m_shape);
}

void Material::setType(Type t)
{
    m_type = t;
    applyType();
}

Material::Type Material::getType() const
{
    return m_type;
}

sf::FloatRect Material::getBounds() const
{
    return m_shape.getGlobalBounds();
}

bool Material::isOffscreen() const
{
    return m_position.x + radius * 2.f < -100.f;
}

void Material::applyType()
{
    switch (m_type)
    {
        case Type::Wood: m_shape.setFillColor(Color::MaterialWood); break;
        case Type::Iron: m_shape.setFillColor(Color::MaterialIron); break;
    }
}
