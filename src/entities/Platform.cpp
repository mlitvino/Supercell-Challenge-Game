#include "entities/Platform.h"

bool Platform::init()
{
	m_speed = speed;
    m_body.setSize({PlatformWidth, PlatformHeight});
    m_body.setFillColor(Color::Platform);
    m_body.setPosition(m_position);
    return true;
}

void Platform::update(float dt)
{
    m_position.x -= m_speed * dt;
    m_body.setPosition(m_position);
}

void Platform::render(sf::RenderTarget& target) const
{
    target.draw(m_body);
}
