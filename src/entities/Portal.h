#pragma once

#include "Entity.h"
#include "utils/Colors.h"
#include "Biome.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Portal final : public Entity
{
public:
    Portal() = default;
    ~Portal() = default;

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

    const sf::FloatRect getBounds() const { return m_body.getGlobalBounds(); }
    bool isOffscreen() const { return m_position.x + Radius * 2.f < -100.f; }

    void setDestination(Biome::Type dest);
    Biome::Type getDestination() const { return m_destination; }

	static constexpr float Radius = 35.f;
private:
    static constexpr float Speed  = 250.f;

    sf::CircleShape m_body;
    float m_speed = Speed;
    Biome::Type m_destination = Biome::Type::Abyss;
};
