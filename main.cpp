#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

#define PRICEPLAY 1
#define SIZESPRITES 7
using namespace std;

struct Player
{
	vector<sf::Texture> frames;
	int size;
	sf::Sprite *x;
};

int main()
{	
    //macOS just won't agree if you try to create a window or handle events in a thread other than the main one.
    //Windows doesn't like windows that are bigger than the desktop. This includes windows created with VideoMode::getDesktopMode(): with the window decorations (borders and titlebar) added, you end up with a window which is slightly bigger than the desktop.

    sf::Clock programClock,playClock; 
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Horse Racing");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    //window.setVerticalSyncEnabled(true);  //application will run same frequency of monitor's refresh rate
         	
	sf::Texture textureBackground_Image;
    if(!textureBackground_Image.loadFromFile("resource/possibleTexture.png") )
	{
		cout << "Error opening background image" << endl; 
		return -1;
	}
	
	sf::Vector2u textureBackgroundSize;
	sf::Vector2u windowSize; 
  
	textureBackground_Image.setSmooth(true);
	textureBackgroundSize = textureBackground_Image.getSize();
	windowSize = window.getSize(); 
	float ScaleX = (float) windowSize.x / textureBackgroundSize.x;
    float ScaleY = (float) windowSize.y / textureBackgroundSize.y;   

    sf::Sprite spriteBackground_Image(textureBackground_Image);
    spriteBackground_Image.setScale(ScaleX, ScaleY); 
    
    
	sf::Font font;
	if (!font.loadFromFile("resource/mk2.ttf"))
	{
		cout << "Error opening font file" << endl;
		return -1;
	}
	
	sf::Music music;
	if (!music.openFromFile("resource/fazendinha.wav"))
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
	
	
	Player spriteList[SIZESPRITES];	
	sf::Texture spriteTextures[SIZESPRITES][7];
    //spriteBackground_Image.setScale(ScaleX, ScaleY); 
	for(int i=1;i<=SIZESPRITES;i++)
	{
		string fileSpriteNameBeggining = "resource/";
		int j=6;
		if(i == 1)
			fileSpriteNameBeggining += "cat";
		if(i == 2)
			fileSpriteNameBeggining += "dog";			
		if(i == 3)
		{
			fileSpriteNameBeggining += "dragon";
			j = 4;
		}
		if(i == 4)
			fileSpriteNameBeggining += "pony";
		if(i == 5)
			fileSpriteNameBeggining += "ram";
		if(i == 6)
			fileSpriteNameBeggining += "sheep";
		if(i == 7)
			fileSpriteNameBeggining += "tiger";
		
		spriteList[i-1].size = j;
		vector<sf::Texture> auxFrames;
		for(int k=0;k<j;k++)
		{
			string fileSpriteName = fileSpriteNameBeggining + to_string((k+1)) + ".png";
			cout << fileSpriteName << endl; 
			if (!spriteTextures[i-1][k].loadFromFile(fileSpriteName) )
			{
				cout << "Error loading file " + fileSpriteName << endl;
				return -1;			
			}
			//auxFrames[k] = spriteTextures[i-1][k];				
		}

	}
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if(flagPlay)
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
			textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
			flagPlay = false;	
			continue;
		}
        window.clear();
        
		window.draw(spriteBackground_Image);
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

