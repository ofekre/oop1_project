#include "Menu.h"
#include "Resourcemanager.h"

void Menu::draw(sf::RenderWindow& window)
{
    sf::Font& font = ResourceManager::instance().getFont("arial");
    
    sf::Text title(font);
    title.setString("LODE RUNNER\n\nPress ENTER to Start\n\nPress ESC to Exit\n\ncreators:OFEK AND AVIA");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::Blue);
    title.setPosition({250.f, 50.f});
    window.draw(title);
}

void Menu::handleInput(sf::Event& event)
{
    // Menu navigation handled in GameController
}

int Menu::getSelectedOption() const
{
    return m_selectedIndex;
}
