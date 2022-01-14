#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <random>

#define PRICEPLAY 1
#define SIZESPRITES 7
#define DURATIONANIMATION 0.5
using namespace std;

struct Player
{
	vector<sf::Texture> frames;
	int size;
	sf::Sprite x;
};

struct Mapfarm
{
	 vector< pair <int,int> > coords;
};

int main()
{	
    //macOS just won't agree if you try to create a window or handle events in a thread other than the main one.
    //Windows doesn't like windows that are bigger than the desktop. This includes windows created with VideoMode::getDesktopMode(): with the window decorations (borders and titlebar) added, you end up with a window which is slightly bigger than the desktop.

    sf::Clock programClock,playClock; 
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Horse Racing");
    //window.setVerticalSyncEnabled(true);  //application will run same frequency of monitor's refresh rate
         	
	sf::Texture textureBackground_Image;
    if(!textureBackground_Image.loadFromFile("resource/background.png") )
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
	
	//Setting the UI
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
	
	//defining the map (done previously using mouse click event to find coordinates
	Mapfarm mapList;
	mapList.coords.push_back( make_pair( 30,395) );
	mapList.coords.push_back( make_pair( 30,501) );
	mapList.coords.push_back( make_pair( 30,623) );
	mapList.coords.push_back( make_pair( 30,708) );
	mapList.coords.push_back( make_pair( 30,769) );
	
	Player spriteList[SIZESPRITES];	
	sf::Texture spriteTextures;
	sf::Sprite spriteCharacter;
	//In here we fill the struct by reading all images and assigning them to a vector of textures and assigning the first of these to a sprite
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
		for(int k=0;k<j;k++)
		{
			string fileSpriteName = fileSpriteNameBeggining + to_string((k+1)) + ".png";
			cout << fileSpriteName << endl; 
			if (!spriteTextures.loadFromFile(fileSpriteName) )
			{
				cout << "Error loading file " + fileSpriteName << endl;
				return -1;			
			}
			spriteList[i-1].frames.push_back( spriteTextures );				
		}
		sf::Sprite spriteCharacter( spriteList[i-1].frames[0] );
		//spriteList[i-1].frames[i].setScale(ScaleX, ScaleY); 
		spriteList[i-1].x = spriteCharacter;
	}
	
	
	//game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if( event.type == sf::Event::MouseButtonPressed)
            {
				cout << "Button " << event.mouseButton.button << " @ "
                     << sf::Mouse::getPosition(window).x << ", "
                     << sf::Mouse::getPosition(window).y << endl;
			}
			
            if(flagPlay)
				break;
				
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::B)
				{
					std::cout << "the B key was pressed" << std::endl;
					creditsIn++;
					textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
					spriteList[4].x.setTexture(spriteList[4].frames[3]);
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
        window.clear();
        
		window.draw(spriteBackground_Image);
        window.draw(textStart);
		window.draw(textCreditsIn);
		window.draw(textCreditsOut);
		window.draw(textCredits);
		window.draw(textCoin);
		
		if(flagPlay)
		{	
			plays++;
			textStart.setString("PLAY: " + to_string(plays));
			textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
			
			//if possible improve this method - https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
			random_device dev;
			mt19937 rng(dev());
			uniform_int_distribution<mt19937::result_type> dist6(0,6); // distribution in range [0, 6]
			int playerOut = dist6(rng);
			cout << playerOut << endl;
			
			for(int i=0;i<=6;i++)
			{
				if( i == playerOut )
					continue;

				spriteList[i].x.move( mapList.coords[i].first , mapList.coords[i].second);
				window.draw(spriteList[0].x);			
			}
			
			//flagPlay = false;	
			continue;
		}
		
        window.display();
    }


	sf::Time elapsedProgram = programClock.getElapsedTime();
	cout << elapsedProgram.asSeconds() << endl;
    return 0;
}
