#pragma once

#include "Entity.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Material final : public Entity
{
public:
    enum class Type { Wood, Iron };

    Material() = default;
    explicit Material(Type type);

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

    void setType(Type t);
    Type getType() const;

    sf::FloatRect getBounds() const;
    bool isOffscreen() const;

    static constexpr float radius = 15.0f;
private:
    void applyType();

    static constexpr float speed = 200.0f;

    Type m_type = Type::Wood;
    sf::CircleShape m_shape;
};
