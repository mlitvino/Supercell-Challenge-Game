#include "ResourceManager.h"
#include "gamestates/StateStack.h"
#include "gamestates/IState.h"
#include "gamestates/StateMenu.h"
#include <memory>
#include <stack>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "utils/constants.h"

int main(int argc, char* argv[])
{
    (void)argc;

    // ResourceManager must be instantiated here -- DO NOT CHANGE
    ResourceManager::init(argv[0]);

    sf::RenderWindow window(sf::VideoMode({constants::kWinWidth, constants::kWinHeight}), "Runner");
    window.setPosition({500, 200});
    window.setKeyRepeatEnabled(false);

    StateStack gamestates;
    if (!gamestates.push<StateMenu>())
        return -1;

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();

        IState* pState = gamestates.getCurrentState();
        if (!pState) return -1;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
                return 0;
            }
        }

        pState->update(elapsedTime.asSeconds());
        window.clear(sf::Color::Blue);
        pState->render(window);
        window.display();

        gamestates.performDeferredPops();
    }

    return 0;
}
