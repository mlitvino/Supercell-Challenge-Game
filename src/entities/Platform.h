#pragma once

#include "Entity.h"
#include "utils/Colors.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Platform final : public Entity
{
public:
	static constexpr float PlatformWidth  = 400.f;
	static constexpr float PlatformHeight = 16.f;

    Platform() = default;
    ~Platform() = default;

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

    const sf::FloatRect getBounds() const { return m_body.getGlobalBounds(); }
    bool isOffscreen() const { return m_position.x + m_body.getSize().x < -100.f; }

private:
	static constexpr float speed = 250.f;

    sf::RectangleShape m_body;
    float m_speed = 0.f;
};
