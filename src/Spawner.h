#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <array>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <random>

#include "entities/Enemy.h"
#include "entities/Platform.h"
#include "entities/Portal.h"
#include "entities/PowerUp.h"
#include "entities/Material.h"
#include "Biome.h"

using minMaxInterval = std::pair<float, float>;

class Spawner
{
public:
    enum class Type
    {
        Enemy,
        PowerUp,
        Platform,
        Portal,
        Material,
        Count,
    };

    Spawner(Biome &biome,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        std::vector<std::unique_ptr<Platform>>& platforms,
        std::vector<std::unique_ptr<Portal>>& portals,
        std::vector<std::unique_ptr<PowerUp>>& powerUps,
        std::vector<std::unique_ptr<Material>>& materials,
        sf::Vector2f spawnPos);

    void update(float dt);

private:
    static constexpr int maxTypes = static_cast<int>(Type::Count);
    static constexpr int laneCount = 4;
    static constexpr float platformOffset = 150.f;

    static constexpr minMaxInterval enemyIntrv{2.f, 4.f};
    static constexpr minMaxInterval platformIntrv{1.5f, 3.5f};
    static constexpr minMaxInterval portalIntrv{1.0f, 2.0f};
    static constexpr minMaxInterval powerUpIntrv{8.0f, 16.0f};
    static constexpr minMaxInterval materialIntrv{8.0f, 16.0f};

    Biome &m_biome;
    sf::Clock clock;

    std::vector<std::unique_ptr<Enemy>>& m_enemies;
    std::vector<std::unique_ptr<Platform>>& m_platforms;
    std::vector<std::unique_ptr<Portal>>& m_portals;
    std::vector<std::unique_ptr<PowerUp>>& m_powerUps;
    std::vector<std::unique_ptr<Material>>& m_materials;

    std::array<std::uniform_real_distribution<float>, maxTypes> m_dist;

    std::array<float, laneCount> m_enemyTimers;
    std::array<float, laneCount> m_platformTimers;
    float m_powerupTimers;
    float m_materialTimer;
    float m_PortalTimer;

    sf::Vector2f m_spawnPos{};
    std::mt19937 m_rand{};

    std::size_t toIndex(Spawner::Type t) { return static_cast<std::size_t>(t); }

    template <typename EntityType, typename Container>
    void spawnAt(Container& out, const sf::Vector2f& startPos)
    {
        std::unique_ptr<EntityType> entity = std::make_unique<EntityType>();
        if (entity && entity->init())
        {
            entity->setPosition(startPos);
            out.push_back(std::move(entity));
        }
    }
};
