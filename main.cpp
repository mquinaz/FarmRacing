#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<string>

#define PRICEPLAY 1

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
    
	sf::Font font;
	if (!font.loadFromFile("mk2.ttf"))
	{
		cout << "Error opening font file" << endl;
		return -1;
	}
	
	sf::Music music;
	if (!music.openFromFile("choroq.wav"))
	{
		cout << "Error opening music file" << endl;
		return -1;
	}
	//sound.setVolume(50.f);
	music.setLoop(true);
	music.play();
	
	int creditsIn = 0;
	int creditsOut = 0;
	int plays = 0;
	bool flagPlay = false;
	
	sf::Text textStart,	textCreditsIn, textCreditsOut, textCredits, textCoin;
	textStart.setFont(font); // font is a sf::Font
	textStart.setString("PLAY: " + to_string(plays));
	textStart.setCharacterSize(30); // in pixels, not points!
	textStart.setFillColor(sf::Color::White);
	textStart.setStyle(sf::Text::Bold);
	textStart.setPosition(100.f, 475.f);
	
	textCreditsIn = textStart;
	textCreditsIn.setString("B TO INSERT CREDITS");
	textCreditsIn.setCharacterSize(20);
	textCreditsIn.setStyle(sf::Text::Regular);
	textCreditsIn.setPosition(75.f, 525.f);
	
	textCreditsOut = textStart;
	textCreditsOut.setString("R TO RETRIEVE CREDITS");
	textCreditsOut.setCharacterSize(20);
	textCreditsOut.setStyle(sf::Text::Regular);
	textCreditsOut.setPosition(75.f, 550.f);

	textCredits = textStart;
	textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
	textCredits.setCharacterSize(20);
	textCredits.setStyle(sf::Text::Regular);	
	textCredits.setPosition(75.f, 575.f);
	
	textCoin = textStart;
	textCoin.setString("INSERT COIN");
	textCoin.setStyle(sf::Text::Regular);
	textCoin.setPosition(90.f, 615.f);
	    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if(flagPLay)
				break;
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::B)
				{
					std::cout << "the B key was pressed" << std::endl;
					creditsIn++;
					textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
								
				}
				if (event.key.code == sf::Keyboard::R)
				{
					cout << "the R key was pressed" << endl;
					if( creditsIn <=0)
					{
						cout << "Insufficient Credits to retrieve" << endl;
						continue;
					}
					creditsOut++;
					creditsIn--;
					textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");							
				}
				if (event.key.code == sf::Keyboard::Enter)
				{
					cout << "the Enter key was pressed" << endl;				
					if( creditsIn <= 0)
					{
						cout << "Insufficient Credits to start" << endl;
						continue;
					}
					creditsIn -= PRICEPLAY;
					flagPlay = true;				
				}
			}
        }

        sf::Time elapsedPlay = playClock.restart();
        //cout << elapsedPlay.asSeconds() << endl;

		if(flagPlay)
		{
			cout << "game" << endl;
			plays++;
			textStart.setString("PLAY: " + to_string(plays));
			flagPlay = false;	
			continue;
		}
        window.clear();
        //window.draw(shape);	
        window.draw(textStart);
		window.draw(textCreditsIn);
		window.draw(textCreditsOut);
		window.draw(textCredits);
		window.draw(textCoin);
        window.display();
    }


	sf::Time elapsedProgram = programClock.getElapsedTime();
	cout << elapsedProgram.asSeconds() << endl;
    return 0;
}



//g++ -c main.cpp
//g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

