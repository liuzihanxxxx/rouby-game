#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    std::string imagePath = "..\\resources\\images\\rouby.png";
    sf::Sprite rouby;

    sf::RenderWindow window(sf::VideoMode(448, 448), "My Window");
    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) {
        std::cerr << "main.cpp: load image lose!";
        _exit(314159);
    }

    rouby.setTexture(texture);
    rouby.setScale(2.0f, 2.0f);

    const int topMargin = 50;
    int rows = 10;
    int cols = 10;
    int width = window.getSize().x;
    int height = window.getSize().y - topMargin;
    float cellWidth = width / float(cols);
    float cellHeight = (window.getSize().y - topMargin) / float(rows);
    float lineWidth = 3.0f;

    {
        sf::FloatRect spriteSize = rouby.getGlobalBounds();
        sf::Vector2f cellCenter = {width / 2.0f - spriteSize.width / 2.0f,
                                   topMargin + cellHeight / 2.0f - spriteSize.height / 2.0f};

        rouby.setPosition(cellCenter);
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        for (int i = 0; i <= cols; ++i) {
            sf::RectangleShape verticalLine(sf::Vector2f(lineWidth, height));
            verticalLine.setPosition(i * cellWidth - lineWidth / 2, topMargin);
            verticalLine.setFillColor(sf::Color::Black);
            window.draw(verticalLine);
        }

        for (int i = 0; i <= rows; ++i) {
            sf::RectangleShape horizontalLine(sf::Vector2f(width, lineWidth));
            horizontalLine.setPosition(0, topMargin + i * cellHeight - lineWidth / 2);
            horizontalLine.setFillColor(sf::Color::Black);
            window.draw(horizontalLine);
        }

        window.draw(rouby);
        window.display();
    }

    return 0;
}
