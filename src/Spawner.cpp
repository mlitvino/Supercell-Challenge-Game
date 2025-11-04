#include "Spawner.h"

Spawner::Spawner(Biome &biome,
                 std::vector<std::unique_ptr<Enemy>>& enemies,
                 std::vector<std::unique_ptr<Platform>>& platforms,
                 std::vector<std::unique_ptr<Portal>>& Portals,
                 std::vector<std::unique_ptr<PowerUp>>& powerUps,
                 std::vector<std::unique_ptr<Material>>& materials,
                 sf::Vector2f spawnPos)
    : m_biome(biome)
    , m_enemies(enemies)
    , m_platforms(platforms)
    , m_portals(Portals)
    , m_powerUps(powerUps)
    , m_materials(materials)
    , m_spawnPos(spawnPos)
    , m_rand(std::random_device{}())
{
    clock.restart();
    const std::size_t enemyIdx = toIndex(Type::Enemy);
    const std::size_t platformIdx = toIndex(Type::Platform);
    const std::size_t PortalIdx = toIndex(Type::Portal);
    const std::size_t powerUpIdx = toIndex(Type::PowerUp);
    const std::size_t materialIdx = toIndex(Type::Material);

    m_dist[enemyIdx] = std::uniform_real_distribution<float>(enemyIntrv.first, enemyIntrv.second);
    m_dist[platformIdx] = std::uniform_real_distribution<float>(platformIntrv.first, platformIntrv.second);
    m_dist[PortalIdx] = std::uniform_real_distribution<float>(portalIntrv.first, portalIntrv.second);
    m_dist[powerUpIdx] = std::uniform_real_distribution<float>(powerUpIntrv.first, powerUpIntrv.second);
    m_dist[materialIdx] = std::uniform_real_distribution<float>(materialIntrv.first, materialIntrv.second);

    for (int lane = 0; lane < laneCount; ++lane)
    {
        m_enemyTimers[lane] = m_dist[enemyIdx](m_rand);
        m_platformTimers[lane] = m_dist[platformIdx](m_rand);
    }
    m_PortalTimer = m_dist[PortalIdx](m_rand);
    m_powerupTimers = m_dist[powerUpIdx](m_rand);
    m_materialTimer = m_dist[materialIdx](m_rand);
}

void Spawner::update(float dt)
{
    float elapsedTime = clock.getElapsedTime().asSeconds();
    float diffucult = 1.0f + (elapsedTime / 30.0f) * 0.5f;

    const std::size_t enemyIdx = toIndex(Type::Enemy);
    const std::size_t platformIdx = toIndex(Type::Platform);
    const std::size_t PortalIdx = toIndex(Type::Portal);
    const std::size_t powerUpIdx = toIndex(Type::PowerUp);

    for (int lane = 0; lane < laneCount; ++lane)
    {
        float &enemyTimer = m_enemyTimers[lane];
        enemyTimer -= dt * diffucult;
        if (enemyTimer <= 0.0f )
        {
            enemyTimer = m_dist[enemyIdx](m_rand);
            const float platformY = m_spawnPos.y - lane * platformOffset;
            const float y = platformY - Enemy::collisionRadius;
            const sf::Vector2f start{m_spawnPos.x, y};
            spawnAt<Enemy>(m_enemies, start);
        }

        float &platTimer = m_platformTimers[lane];
        platTimer -= dt;
        if (platTimer <= 0.0f && lane != 0)
        {
            platTimer = m_dist[platformIdx](m_rand);
            const float y = m_spawnPos.y - lane * platformOffset;
            const sf::Vector2f start{m_spawnPos.x, y};
            spawnAt<Platform>(m_platforms, start);
        }
    }

    std::uniform_int_distribution<int> possibleLane(0, 3);
    m_powerupTimers -= dt;
    if (m_powerupTimers <= 0.0f)
    {
        m_powerupTimers = m_dist[powerUpIdx](m_rand);
        int lane = possibleLane(m_rand);
        const float y = m_spawnPos.y - lane * platformOffset - PowerUp::radius;
        const sf::Vector2f start{m_spawnPos.x, y};
        spawnAt<PowerUp>(m_powerUps, start);
        if (!m_powerUps.empty())
        {
            std::uniform_int_distribution<int> typeDist(0, 1);
            PowerUp::Type type = typeDist(m_rand) == 0 ? PowerUp::Type::Shield : PowerUp::Type::Bomb;
            m_powerUps.back()->setType(type);
        }
    }

    const std::size_t materialIdx = toIndex(Type::Material);
    m_materialTimer -= dt;
    if (m_materialTimer <= 0.0f)
    {
        m_materialTimer = m_dist[materialIdx](m_rand);
        int lane = possibleLane(m_rand);
        const float y = m_spawnPos.y - lane * platformOffset - Material::radius;
        const sf::Vector2f start{m_spawnPos.x, y};
        spawnAt<Material>(m_materials, start);
        if (!m_materials.empty())
        {
            std::uniform_int_distribution<int> typeDist(0, 1);
            Material::Type type = typeDist(m_rand) == 0 ? Material::Type::Wood : Material::Type::Iron;
            m_materials.back()->setType(type);
        }
    }


    m_PortalTimer -= dt;
    if (m_PortalTimer <= 0.0f)
    {
        bool isBiomeAbyss = m_biome.getTypeBiome() == Biome::Type::Abyss;
        Biome::Type dest = isBiomeAbyss ? Biome::Type::Homeland : Biome::Type::Abyss;
        m_PortalTimer = m_dist[PortalIdx](m_rand);
        const float platformY = m_spawnPos.y - possibleLane(m_rand) * platformOffset;
        const float y = platformY - Portal::Radius;
        const sf::Vector2f start{m_spawnPos.x, y};
        spawnAt<Portal>(m_portals, start);
        if (!m_portals.empty())
            m_portals.back()->setDestination(dest);
    }
}
