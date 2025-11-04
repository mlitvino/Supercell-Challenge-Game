#include "StatePlaying.h"
#include "StatePaused.h"
#include "StateStack.h"
#include "ResourceManager.h"
#include <memory>
#include <iostream>
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

StatePlaying::StatePlaying(StateStack& stateStack)
    : m_stateStack(stateStack)
{
}

StatePlaying::~StatePlaying()
{
    if (m_currentMusic)
        m_currentMusic->stop();
}

bool StatePlaying::init()
{
    if (!m_homelandMusic.openFromFile("assets/homeland.mp3"))
        return false;
    if (!m_abyssMusic.openFromFile("assets/abyss.mp3"))
        return false;

    m_homelandMusic.setLooping(true);
    m_abyssMusic.setLooping(true);

    m_biome = std::make_unique<Biome>(Biome::Type::Homeland);
    if (!m_biome)
        return false;

    float groundY = m_biome->getGroundY();

    m_pPlayer = std::make_unique<Player>(*m_biome.get());
    if (!m_pPlayer || !m_pPlayer->init())
        return false;

    m_pPlayer->setPosition(sf::Vector2f(200, groundY));
    m_prevPlayerY = groundY;

    const sf::Vector2f spawnPos{1100.0f, groundY};
    m_spawner = std::make_unique<Spawner>(*m_biome, m_enemies, m_platforms, m_portals, m_powerUps, m_materials, spawnPos);
    if (!m_spawner)
        return false;

    m_currentMusic = &m_homelandMusic;
    m_currentMusic->setVolume(50.f);
    m_currentMusic->play();

    return true;
}

void StatePlaying::update(float dt)
{
    bool isPauseKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    m_hasPauseKeyBeenReleased |= !isPauseKeyPressed;
    if (m_hasPauseKeyBeenReleased && isPauseKeyPressed)
    {
        m_hasPauseKeyBeenReleased = false;
        m_stateStack.push<StatePaused>();
    }

    m_spawner->update(dt);

    for (const std::unique_ptr<Platform>& pPlatform : m_platforms)
        pPlatform->update(dt);
    removeOffscreenObjects(m_platforms);

    for (const std::unique_ptr<Enemy>& pEnemy : m_enemies)
        pEnemy->update(dt);
    removeOffscreenObjects(m_enemies);

    for (const std::unique_ptr<Portal>& pPortal : m_portals)
        pPortal->update(dt);
    removeOffscreenObjects(m_portals);

    for (const std::unique_ptr<PowerUp>& pPowerUp : m_powerUps)
        pPowerUp->update(dt);
    removeOffscreenObjects(m_powerUps);

    for (const std::unique_ptr<Material>& pMaterial : m_materials)
        pMaterial->update(dt);
    removeOffscreenObjects(m_materials);

    m_pPlayer->update(dt);

    if (m_dropThroughActive)
    {
        m_dropThroughTimer -= dt;
        if (m_dropThroughTimer <= 0.f)
            m_dropThroughActive = false;
    }

    updateCollisions(dt);

    if (m_pPlayer->getState() == Player::State::Dead)
        m_stateStack.popDeferred();
}

void StatePlaying::updateCollisions(float dt)
{
    (void)dt;

    const sf::Vector2f playerPos = m_pPlayer->getPosition();
    const float playerRadius = m_pPlayer->getCollisionRadius();

    for (auto it = m_enemies.begin(), end = m_enemies.end(); it != end; ++it)
    {
        float distance = (playerPos - (*it)->getPosition()).lengthSquared();
        float minDistance = std::pow(playerRadius, 2.f);

        if (m_pPlayer->isAttacking() && distance < std::pow(m_pPlayer->getSword().getRadius(), 2.f))
        {
            m_enemies.erase(it);
            break;
        }

        if (distance < minDistance)
        {
            if (m_pPlayer->hasShield())
            {
                m_pPlayer->setShield(false);
                m_enemies.erase(it);
            }
            else
                m_pPlayer->setState(Player::State::Dead);
            break;
        }
    }

    updatePlatformCollision(playerPos);

    for (const std::unique_ptr<Portal>& portal : m_portals)
    {
        float distance = (playerPos - portal->getPosition()).lengthSquared();
        float minDistance = std::pow(playerRadius, 2.f);

        if (distance < minDistance)
        {
            Biome::Type nextBiome;
            if (m_biome->getTypeBiome() == Biome::Type::Abyss)
                nextBiome = Biome::Type::Homeland;
            else
                nextBiome = Biome::Type::Abyss;
            changeBiome(nextBiome);
            break;
        }
    }

    for (auto it = m_powerUps.begin(), end = m_powerUps.end(); it != end; ++it)
    {
        float distance = (playerPos - (*it)->getPosition()).lengthSquared();
        float minDistance = std::pow(playerRadius, 2.f);

        if (distance < minDistance)
        {
            if ((*it)->getType() == PowerUp::Type::Bomb)
                m_enemies.clear();
            else
                m_pPlayer->setShield(true);
			m_powerUps.erase(it);
            break;
        }
    }

    for (auto it = m_materials.begin(), end = m_materials.end(); it != end; ++it)
    {
        float distance = (playerPos - (*it)->getPosition()).lengthSquared();
        float minDistance = std::pow(playerRadius, 2.f);

        if (distance < minDistance)
        {
            m_pPlayer->upgrade();
            m_materials.erase(it);
            break;
        }
    }
}

void StatePlaying::updatePlatformCollision(const sf::Vector2f& playerPos)
{
    const float biomeGroundY = m_biome->getGroundY();
    float candidateGroundY = biomeGroundY;

    if (!m_dropThroughActive)
    {
        for (const std::unique_ptr<Platform>& pPlatform : m_platforms)
        {
            const sf::FloatRect playerBox = pPlatform->getBounds();
            const float left = playerBox.position.x;
            const float right = playerBox.position.x + playerBox.size.x;
            const float topY = playerBox.position.y;
            const float playerX = playerPos.x;

            const bool horizontallyOver = (playerX >= left && playerX <= right);
            const bool platformBelowOrAt = (topY >= playerPos.y - 0.1f);
            if (horizontallyOver && platformBelowOrAt)
            {
                if (topY < candidateGroundY)
                    candidateGroundY = topY;
            }
        }
    }

    bool dropRequested = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    if (dropRequested)
    {
        m_dropThroughActive = true;
        m_dropThroughTimer = 0.2f;
        candidateGroundY = biomeGroundY;
    }

    m_pPlayer->setGroundY(candidateGroundY);
    m_prevPlayerY = m_pPlayer->getPosition().y;
}

void StatePlaying::render(sf::RenderTarget& target) const
{
    m_biome->renderBackground(target);
    for (const std::unique_ptr<Platform>& pPlatform : m_platforms)
        pPlatform->render(target);
    for (const std::unique_ptr<Enemy>& pEnemy : m_enemies)
        pEnemy->render(target);
    for (const std::unique_ptr<Portal>& pPortal : m_portals)
        pPortal->render(target);
    for (const std::unique_ptr<PowerUp>& pPowerUp : m_powerUps)
        pPowerUp->render(target);
    for (const std::unique_ptr<Material>& material : m_materials)
        material->render(target);
    m_pPlayer->render(target);
    m_biome->renderForeground(target);
}

void StatePlaying::changeBiome(Biome::Type type)
{
    m_biome->setBiome(type);

    m_enemies.clear();
    m_platforms.clear();
    m_portals.clear();
    m_powerUps.clear();

    if (type == Biome::Type::Abyss)
    {
        m_pPlayer->changeTexture("player_behelit.png");
    }
    else
    {
        m_pPlayer->changeTexture("player.png");
    }

    const float groundY = m_biome->getGroundY();
    m_pPlayer->setGroundY(groundY);


    sf::Vector2f pos = m_pPlayer->getPosition();
    if (pos.y > groundY)
    {
        pos.y = groundY;
        m_pPlayer->setPosition(pos);
    }
    m_prevPlayerY = pos.y;

    m_dropThroughActive = false;
    m_dropThroughTimer = 0.f;
}
