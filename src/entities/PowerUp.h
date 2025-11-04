#pragma once

#include "Entity.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "utils/Colors.h"

class PowerUp final : public Entity
{
public:
    enum class Type
	{
		Shield,
		Bomb
	};

    PowerUp() = default;
    ~PowerUp() = default;
    explicit PowerUp(Type type);

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

    void setType(Type t);
    Type getType() const;

    sf::FloatRect getBounds() const;

	static constexpr float radius = 24.0f;
private:
    void applyType();

    static constexpr float speed = 200.0f;

    Type m_type = Type::Shield;
    sf::CircleShape m_shape;
};
