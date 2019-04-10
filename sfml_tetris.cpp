#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <chrono>
#include <cstddef>
#include <string>
#include <thread>

namespace piece
{
    inline constexpr auto _ = false;
    inline constexpr auto O = true;

    // clang-format off
    inline constexpr std::array box{_, _, _, _,
                                    _, O, O, _,
                                    _, O, O, _,
                                    _, _, _, _};

    inline constexpr std::array line{_, O, _, _,
                                     _, O, _, _,
                                     _, O, _, _,
                                     _, O, _, _};

    inline constexpr std::array t{_, _, _, _,
                                  O, O, O, _,
                                  _, O, _, _,
                                  _, _, _, _};

    inline constexpr std::array l{_, _, _, _,
                                  _, O, O, _,
                                  _, O, _, _,
                                  _, O, _, _};

    inline constexpr std::array inv_l{_, _, _, _,
                                      _, O, O, _,
                                      _, _, O, _,
                                      _, _, O, _};
    // clang-format on
} // namespace piece

class Game
{
private:
    sf::RenderWindow _window;
    bool _running{true};

public:
    Game() : _window{sf::VideoMode{640, 480}, ""}
    {
        while(_running)
        {
            const auto tp0 = std::chrono::high_resolution_clock::now();

            _window.clear(sf::Color::Black);

            sf::Event event;
            while(_window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    _window.close();
                    break;
                }
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                _running = false;
            }

            /* stuff */

            const auto tp1 = std::chrono::high_resolution_clock::now();
            const auto elapsedTime = tp1 - tp0;

            const auto elapsedMs = std::chrono::duration_cast<
                std::chrono::duration<float, std::milli>>(elapsedTime)
                                       .count();

            const auto ftSeconds(elapsedMs / 1000.f);
            const auto fps(1.f / ftSeconds);

            _window.setTitle("FT: " + std::to_string(elapsedMs) +
                             "\tFPS: " + std::to_string(fps));

            _window.setActive();
            _window.draw(sf::Sprite{});
            _window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

private:
    void run()
    {
    }
};

int main()
{
    Game{};
}
