#include "Player.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>

Player::Player(Biome &biome)
    : m_biome{biome}
{
}

bool Player::changeTexture(const std::string &texturePath)
{
    const sf::Texture* pTexture = ResourceManager::getOrLoadTexture(texturePath);
    if (pTexture == nullptr)
        return false;
    m_pSprite->setTexture(*pTexture, true);
    return true;
}

void Player::setShield(bool hasShield)
{
    m_hasShield = hasShield;
}

bool Player::hasShield() const
{
    return m_hasShield;
}

void Player::upgrade()
{
    if (m_hasSword)
    {
        float newRadius = m_sword.getRadius() * 1.1f;
        m_sword.setRadius(newRadius);
        m_sword.setOrigin({newRadius, newRadius * 1.1f});
    }
    else
    {
        m_hasSword = true;
    }
}

bool Player::init()
{
    const sf::Texture* pTexture1 = ResourceManager::getOrLoadTexture("player.png");
    if (pTexture1 == nullptr)
        return false;

    const sf::Texture* pTexture2 = ResourceManager::getOrLoadTexture("player_behelit.png");
    if (pTexture2 == nullptr)
        return false;

    m_pSprite = std::make_unique<sf::Sprite>(*pTexture1);
    if (!m_pSprite)
        return false;

    if (!changeTexture("player.png"))
        return false;

    m_rotation = sf::degrees(0);
    sf::FloatRect localBounds = m_pSprite->getLocalBounds();
    m_pSprite->setOrigin({localBounds.size.x / 2.0f, localBounds.size.y / 2.0f});
    m_pSprite->setPosition(m_position);
    m_pSprite->setScale(sf::Vector2f(3.0f, 3.0f));
    m_collisionRadius = collisionRadius;

    m_groundY = m_biome.getGroundY();
    m_jumpBaseY = m_groundY;

    m_shieldShape.setRadius(collisionRadius * 1.1f);
    m_shieldShape.setOrigin({collisionRadius * 1.1f, collisionRadius * 1.1f});
    m_shieldShape.setFillColor(sf::Color(55, 100, 255, 80));

    m_sword.setRadius(collisionRadius * 1.5f);
    m_sword.setFillColor(Color::Black);
    m_sword.scale({0.3f, 0.6f});
    m_sword.setRadius(m_sword.getRadius() * 1.1f);
    float finalRadius = m_sword.getRadius();
    m_sword.setOrigin({finalRadius, finalRadius * 1.4f});

    return true;
}

void Player::update(float dt)
{
    constexpr float eps = 0.5f;
    const float groundY = m_groundY;
    m_isOnGround = (m_position.y >= groundY - eps);

    if (m_isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        m_isJumping = true;
        m_isOnGround = false;
        m_jumpBaseY = groundY;
    }

    if (m_prevAttack && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        m_prevAttack = false;
        m_attacking = false;
        m_sword.rotate(sf::degrees(-90.f));
    }

    if (!m_prevAttack && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        m_attacking = true;
        m_prevAttack = true;
        m_sword.rotate(sf::degrees(90.f));
    }

    const float apexY = m_jumpBaseY - jumpHeight;

    if (m_isJumping)
    {
        m_position.y -= jumpSpeed * dt;
        if (m_position.y <= apexY)
        {
            m_position.y = apexY;
            m_isJumping = false;
        }
    }
    else if (!m_isOnGround)
    {
        m_position.y += jumpSpeed * dt;
        if (m_position.y >= groundY)
        {
            m_position.y = groundY;
            m_isOnGround = true;
        }
    }

    if (m_hasShield)
        m_shieldShape.setPosition(m_position);
    if (m_hasSword)
        m_sword.setPosition({m_position.x + 20.f, m_position.y - 10.f});
}

void Player::render(sf::RenderTarget& target) const
{
    if (m_hasShield)
        target.draw(m_shieldShape);
    if (m_hasSword)
        target.draw(m_sword);
    m_pSprite->setRotation(m_rotation);
    m_pSprite->setPosition(m_position);
    target.draw(*m_pSprite);
}
