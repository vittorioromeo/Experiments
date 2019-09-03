#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdio>
#include <vector>

struct Brick
{
    sf::Vector2i topLeft;
    sf::Vector2i bottomRight;

    int width() const
    {
        return bottomRight.x - topLeft.x;
    }

    int height() const
    {
        return bottomRight.y - topLeft.y;
    }
};

int main()
{
    // Brick bob{{200, 200}, {300, 250}};
    // Brick mike{{100, 400}, {200, 550}};
    std::vector<Brick> bricks;

    const int initialX = 300;
    const int initialY = 300;
    const int brickWidth = 60;
    const int brickHeight = 30;
    const int bricksPerRow = 6;
    const int bricksPerCol = 6;
    const int offset = 15;

    for(int iRow = 0; iRow < bricksPerRow; ++iRow)
    {
        for(int iCol = 0; iCol < bricksPerCol; ++iCol)
        {
            int left{initialX + (brickWidth + offset) * iRow};
            int top{initialY + (brickHeight + offset) * iCol};
            int right{left + brickWidth};
            int bottom{top + brickHeight};

            bricks.push_back(Brick{{left, top}, {right, bottom}});
        }
    }

    sf::RenderWindow window{sf::VideoMode{1024, 768}, "Diba is ugly",
        sf::Style::Default, sf::ContextSettings{}};

    window.setFramerateLimit(60);

    while(true)
    {
        sf::Event event;
        window.pollEvent(event);

        if(event.type == sf::Event::Closed)
        {
            break;
        }

        window.clear();

        // bob.topLeft.x += 1;
        // bob.bottomRight.x += 1;

        for(Brick& b : bricks)
        {
            sf::RectangleShape rs(sf::Vector2f(b.width(), b.height()));
            rs.setPosition(sf::Vector2f(b.topLeft));
            rs.setFillColor(sf::Color::Red);
            window.draw(rs);
        }

        window.display();
    }
}
