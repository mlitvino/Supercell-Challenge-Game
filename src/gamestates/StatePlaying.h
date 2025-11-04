#pragma once

#include "IState.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Spawner.h"
#include "Biome.h"
#include "entities/Portal.h"
#include "entities/PowerUp.h"
#include "entities/Material.h"

#include <SFML/Audio/Music.hpp>

class StatePlaying : public IState
{
public:
    StatePlaying(StateStack& stateStack);
    ~StatePlaying();

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;
    void changeBiome(Biome::Type type);

private:
    StateStack& m_stateStack;

    std::unique_ptr<Player> m_pPlayer;
    std::unique_ptr<Spawner> m_spawner;
    std::unique_ptr<Biome> m_biome;

    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<Platform>> m_platforms;
    std::vector<std::unique_ptr<Portal>> m_portals;
    std::vector<std::unique_ptr<PowerUp>> m_powerUps;
    std::vector<std::unique_ptr<Material>> m_materials;

    bool m_hasPauseKeyBeenReleased = true;
    float m_prevPlayerY = 0.f;
    bool m_dropThroughActive = false;
    float m_dropThroughTimer = 0.f;

    sf::Music m_homelandMusic;
    sf::Music m_abyssMusic;
    sf::Music* m_currentMusic = nullptr;

    void updateCollisions(float dt);
    void updatePlatformCollision(const sf::Vector2f& playerPos);

    template<typename Container>
    void removeOffscreenObjects(Container& container)
    {
        container.erase(
            std::remove_if(container.begin(), container.end(),
                [](const auto& obj) { return obj->isOffscreen(); }),
            container.end()
        );
    }
};
