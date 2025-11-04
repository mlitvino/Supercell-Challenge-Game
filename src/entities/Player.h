#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Biome.h"

namespace sf { class Sprite; }

class Player final : public Entity
{
public:
    enum class State
    {
        Alive,
        Dead
    };

    Player(Biome &biome);
    ~Player() = default;

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;


    State getState() const { return m_state; };
    void setState(State state) { m_state = state; };

    void setGroundY(float y) { m_groundY = y; }
    float getGroundY() const { return m_groundY; }

    void setShield(bool hasShield);
    bool hasShield() const;

    void setSword(bool hasSword) { m_hasSword = hasSword; };
    bool hasSword() const { return m_hasSword; };

    void setAttack(bool Attack) { m_attacking = Attack; };
    bool isAttacking() const { return m_attacking; };

    const sf::CircleShape& getSword() { return m_sword; };

    bool changeTexture(const std::string &texturePath);

    void upgrade();
private:
    static constexpr float collisionRadius = 42.0f;
    static constexpr float jumpSpeed = 500.0f;
    static constexpr float jumpHeight = 220.0f;

    Biome &m_biome;
    bool m_isJumping = false;
    bool m_isOnGround = true;
    State m_state = State::Alive;

    float m_groundY = 0.f;
    float m_jumpBaseY = 0.f;
    bool m_hasShield = false;
    bool m_hasSword = false;
    bool m_attacking = false;
    bool m_prevAttack = false;
    float m_reloading = 0.5f;
    float m_reloadingTimer = 0.f;
    sf::CircleShape m_shieldShape;
    sf::CircleShape m_sword;
};
