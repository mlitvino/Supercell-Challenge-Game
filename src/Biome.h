#pragma once

#include "gamestates/IState.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "utils/Colors.h"
#include "utils/constants.h"

class Biome {
public:
    enum class Type { Homeland, Abyss };

    Biome(Type type);

    void renderBackground(sf::RenderTarget& target) const;
    void renderForeground(sf::RenderTarget& target) const;

    float getGravity() const { return m_gravity; }
    void setGravity(float g) { m_gravity = g; }

    const sf::Color& getBackgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const sf::Color& c) { m_backgroundColor = c; }

    Type getTypeBiome() const { return m_type; }
    void setBiome(Type type);

    const sf::RectangleShape& getGround() const { return m_ground; }
	float getGroundY() const { return m_ground.getPosition().y; }

private:
    void buildGradientOverlay();

    sf::RectangleShape m_ground;
    sf::VertexArray m_gradientOverlay;
    float m_gravity;
    sf::Color m_backgroundColor;
    Type m_type;
};
