#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
    void draw(sf::RenderWindow& window);
    void handleInput(sf::Event& event);
    int getSelectedOption() const;

private:
    int m_selectedIndex = 0;
};