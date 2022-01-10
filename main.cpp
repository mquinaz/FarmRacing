#include <SFML/Graphics.hpp>
#include<iostream>
using namespace std;

int main()
{	
    //macOS just won't agree if you try to create a window or handle events in a thread other than the main one.
    //Windows doesn't like windows that are bigger than the desktop. This includes windows created with VideoMode::getDesktopMode(): with the window decorations (borders and titlebar) added, you end up with a window which is slightly bigger than the desktop.

    sf::Clock programClock,playClock; 
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Horse Racing");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    //window.setVerticalSyncEnabled(true);  //application will run same frequency of monitor's refresh rate
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
	if (event.type == sf::Event::KeyPressed)
	{
	    if (event.key.code == sf::Keyboard::B)
	    {
		std::cout << "the escape key was pressed" << std::endl;
		std::cout << "control:" << event.key.control << std::endl;
		std::cout << "alt:" << event.key.alt << std::endl;
		std::cout << "shift:" << event.key.shift << std::endl;
		std::cout << "system:" << event.key.system << std::endl;
	    }
	}
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
