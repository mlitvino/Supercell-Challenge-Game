#include "Biome.h"

Biome::Biome(Type type)
	: m_gradientOverlay{sf::PrimitiveType::TriangleStrip, 4}
{
	m_ground.setSize({constants::kWinWidth, 256.0f});
	m_ground.setPosition({0.0f, 800.0f});

	setBiome(type);
	buildGradientOverlay();
}

void Biome::setBiome(Type type)
{
	m_type = type;
	switch (m_type)
	{
		case Type::Homeland:
		{
			m_backgroundColor = Color::Background;
			m_ground.setFillColor(Color::Green);
			m_gravity = 900.f;
			break;
		}
		case Type::Abyss:
		{
			m_backgroundColor = Color::Purple;
			m_ground.setFillColor(Color::Ground);
			m_gravity = 1400.f;
			break;
		}
		default:
			break;
	}
}

void Biome::buildGradientOverlay()
{
	const sf::Color left = sf::Color(0, 0, 0, 100);
	const sf::Color right = sf::Color(0, 0, 0, 255);

	m_gradientOverlay[0].position = {0.f, 0.f};
	m_gradientOverlay[0].color = left;

	m_gradientOverlay[1].position = {0.f, static_cast<float>(constants::kWinHeight)};
	m_gradientOverlay[1].color = left;

	m_gradientOverlay[2].position = {static_cast<float>(constants::kWinWidth), 0.f};
	m_gradientOverlay[2].color = right;

	m_gradientOverlay[3].position = {static_cast<float>(constants::kWinWidth), static_cast<float>(constants::kWinHeight)};
	m_gradientOverlay[3].color = right;
}

void Biome::renderBackground(sf::RenderTarget& target) const
{
	target.clear(m_backgroundColor);
	target.draw(m_ground);
}

void Biome::renderForeground(sf::RenderTarget& target) const
{
	if (m_type == Biome::Type::Abyss)
		target.draw(m_gradientOverlay);
}
