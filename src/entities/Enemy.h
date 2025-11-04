#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class Sprite; }

class Enemy final : public Entity
{
public:
    Enemy() = default;
    ~Enemy() = default;

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

    static constexpr float collisionRadius = 24.0f;
private:
    static constexpr float speed = 250.0f;
};
