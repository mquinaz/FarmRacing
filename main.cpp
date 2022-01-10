#include <SFML/Graphics.hpp>
#include<iostream>
using namespace std;

int main()
{
	sf::Clock programClock,playClock; 
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Horse Racing");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        sf::Time elapsedPlay = playClock.restart();
        cout << elapsedPlay.asSeconds() << endl;
        window.draw(shape);
        window.display();
    }


	sf::Time elapsedProgram = programClock.getElapsedTime();
	cout << elapsedProgram.asSeconds() << endl;
    return 0;
}



//g++ -c main.cpp
//g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
