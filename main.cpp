#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <math.h>
#include <map>

#define PRICEPLAY 1
#define SIZESPRITES 7
#define DURATIONANIMATION 0.08
#define VELOCITY 0.035
#define RADIUS 25

using namespace std;

struct Player
{
	vector<sf::Texture> frames;
	int size,currentFrame,currentWaypoint;
	float v,spriteV;
	sf::Sprite x;
};


sf::Vector2f normalize(sf::Vector2f vec)
{
	
	float length = sqrt((vec.x * vec.x) + ( vec.y * vec.y) );
    float newX   = vec.x / length;
    float newY   = vec.y / length;
    return sf::Vector2f(newX,newY);
}

string raceResults(vector<int> results, map<int, string> playerMap)
{
	cout << "###Results of the race###"<< endl << endl;
	string auxWinner = "";
	for(int i=0;i<6;i++)
		auxWinner += to_string(i+1) + "# place: " + playerMap[ results[i] ]+"\n";

	return auxWinner;
}

int main()
{	
    //macOS just won't agree if you try to create a window or handle events in a thread other than the main one.
    //Windows doesn't like windows that are bigger than the desktop. This includes windows created with 
    //VideoMode::getDesktopMode(): with the window decorations (borders and titlebar) added, you end up with a window which is slightly bigger than the desktop.
    
	sf::Clock programClock,playClock; 
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Farm Racing");
    //window.setVerticalSyncEnabled(true);  //application will run same frequency of monitor's refresh rate
         	
	sf::Texture textureFinalMenu;
    if(!textureFinalMenu.loadFromFile("resource/finalMenu.png") )
	{
		cout << "Error opening menuFinal image" << endl; 
		return -1;
	}
	
	sf::Texture textureBackgroundImage;
    if(!textureBackgroundImage.loadFromFile("resource/background.png") )
	{
		cout << "Error opening background image" << endl; 
		return -1;
	}
	
	//Background image
	sf::Vector2u textureBackgroundSize;
	sf::Vector2u windowSize; 
  
	textureBackgroundImage.setSmooth(true);
	textureBackgroundSize = textureBackgroundImage.getSize();
	windowSize = window.getSize(); 
	float ScaleX = (float) windowSize.x / textureBackgroundSize.x;
    float ScaleY = (float) windowSize.y / textureBackgroundSize.y;   

	//final UI menu
    sf::Sprite spriteBackgroundImage(textureBackgroundImage);
    spriteBackgroundImage.setScale(ScaleX, ScaleY); 
    
    textureBackgroundSize = textureFinalMenu.getSize();
    ScaleX = (float) windowSize.x / textureBackgroundSize.x;
    ScaleY = (float) windowSize.y / textureBackgroundSize.y;   
    sf::Sprite spriteFinalMenu(textureFinalMenu);
    spriteFinalMenu.setScale(ScaleX/2, ScaleY/2); 
    spriteFinalMenu.setPosition(windowSize.x / 2 - textureBackgroundSize.x - textureBackgroundSize.x/4, windowSize.y / 2 - textureBackgroundSize.y/2); 
    
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
	
	//Setting the UI in pixels, not points
	sf::Text textStart,	textCreditsIn, textCreditsOut, textCredits, textCoin,textWinners,text1,text2,text3;
	textStart.setFont(font); 
	textStart.setString("PLAY: " + to_string(plays));
	textStart.setCharacterSize(75); 
	textStart.setFillColor(sf::Color::White);
	textStart.setStyle(sf::Text::Bold);
	textStart.setPosition(windowSize.x/2 - textStart.getLocalBounds().width/2 - 40, windowSize.y/2 - 100 );
	
	textCreditsIn = textStart;
	textCreditsIn.setString("B TO INSERT CREDITS");
	textCreditsIn.setCharacterSize(40);
	textCreditsIn.setStyle(sf::Text::Regular);
	textCreditsIn.setPosition(textCreditsIn.getPosition()+ sf::Vector2f(-50,90));
	
	textCreditsOut = textStart;
	textCreditsOut.setString("R TO RETRIEVE CREDITS");
	textCreditsOut.setCharacterSize(40);
	textCreditsOut.setStyle(sf::Text::Regular);
	textCreditsOut.setPosition(textCreditsOut.getPosition()+ sf::Vector2f(-75,160));

	textCredits = textStart;
	textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
	textCredits.setCharacterSize(40);
	textCredits.setStyle(sf::Text::Regular);	
	textCredits.setPosition(textCredits.getPosition()+ sf::Vector2f(-125,125));
	
	textCoin = textStart;
	textCoin.setString("INSERT COIN");
	textCoin.setStyle(sf::Text::Regular);
	textCoin.setPosition(textCoin.getPosition()+ sf::Vector2f(-75,225));

	textWinners = textStart;
	textWinners.setCharacterSize(55);
	textWinners.setPosition(windowSize.x / 2 - textureBackgroundSize.x + 150, windowSize.y / 2 - textureBackgroundSize.y/2 + 60);
	
	//https://github.com/skyrpex/RichText
	//this texts are used for coloring the 1 2 and 3 place with colors
	text1 = textWinners;
	text1.setString("1#");
	//https://www.schemecolor.com/gold-silver-and-bronze-color-palette.php
	text1.setFillColor(sf::Color(218,165,32,255));
	text2 = text1;
	text2.setString("2#");
	text2.setPosition(textWinners.getPosition() + sf::Vector2f(0,63));
	text2.setFillColor(sf::Color(117,117,117,255));
	text3 = textWinners;
	text3.setString("3#");
	text3.setPosition(textWinners.getPosition()+ sf::Vector2f(0,126));
	text3.setFillColor(sf::Color(205,127,50,255));
		
	//defining the map (done previously using mouse click event to find coordinates	
	vector < sf::Vector2f> waypointList; 
	waypointList.push_back( sf::Vector2f( 46,303) );
	waypointList.push_back( sf::Vector2f( 175,284) );
	waypointList.push_back( sf::Vector2f( 421,320) );
	waypointList.push_back( sf::Vector2f( 588,333) );
	waypointList.push_back( sf::Vector2f( 741,316) );
	waypointList.push_back( sf::Vector2f( 961,288) );
	waypointList.push_back( sf::Vector2f( 1134,304) );
	waypointList.push_back( sf::Vector2f( 1287,310) );
	waypointList.push_back( sf::Vector2f( 1457,325) );
	waypointList.push_back( sf::Vector2f( 1587,310) );
	waypointList.push_back( sf::Vector2f( 1757,298) );
	waypointList.push_back( sf::Vector2f( 1844,297) );
	
	//used to show winners in a prettier way
	map<int, string> playerMap = {
    { 0, "cat" },
    { 1, "dog" },
    { 2, "dragon" },
    { 3, "pony" },
    { 4, "ram" },
    { 5, "sheep" },
    { 6, "tiger" }
	};

	//used to assign a y axis variation of waypoint to a playing character
	map<int, int> playerPosition;

	Player spriteList[SIZESPRITES];	
	sf::Texture spriteTextures;
	sf::Sprite spriteCharacter;
	
	vector<int> placeRace;
	
	//In here we fill the struct by reading all images and assigning them to a vector of textures and assigning the first of these to a sprite
	for(int i=1;i<=SIZESPRITES;i++)
	{
		string fileSpriteNameBeggining = "resource/";
		//j is the number of frames (6 and 4 in dragon case), they are -- to later on use %
		int j=6;
		fileSpriteNameBeggining += playerMap[i-1];
		if( i == 3)
			j=4;
		
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
		spriteList[i-1].currentFrame = 0;
		spriteList[i-1].x = spriteCharacter;
		spriteList[i-1].currentWaypoint = 1;
		spriteList[i-1].spriteV = 0;
	}
	
	
	//game loop
    while (window.isOpen())
    {
        sf::Event event;
        int playerOut;
        sf::Time elapsedPlay;
        while (window.pollEvent(event))
			{
            if (event.type == sf::Event::Closed)
                window.close();
            
            //this event helped in finding positions to create waypoints
            if( event.type == sf::Event::MouseButtonPressed)
            {
				cout << "Button " << event.mouseButton.button << " @ "
                     << sf::Mouse::getPosition(window).x << ", "
                     << sf::Mouse::getPosition(window).y << endl;
			}
			
			//if we are at the final menu (size==6) lets reset the placeRace to play again
			if (event.key.code == sf::Keyboard::Enter && placeRace.size() == 6)
			{
				placeRace.clear();
				for(int i=0;i<=6;i++)
					spriteList[i].currentWaypoint = 1;			
				
				playerPosition.clear();	
				continue;
			}
			
			//disabling initial menu during a play
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
					elapsedPlay = playClock.restart();			
					plays++;
					textStart.setString("PLAY: " + to_string(plays));
					textCredits.setString( to_string(creditsIn) + " CREDITS IN / " + to_string(creditsOut) + " CREDITS OUT");
			
					//player that is not playing this round
					random_device dev;
					mt19937 rng(dev());
					uniform_int_distribution<mt19937::result_type> dist6(0,6); // distribution in range [0, 6]
					playerOut = dist6(rng);
					cout << playerOut << endl;	
					
					//setting up player position so that is not a hole when a player is left out
					int playerOutAux[] = {0,100,200,300,400,500};
					int playerOutIndexAux = 0;
					for(int i=0;i<=6;i++)
					{
						if( i == playerOut)
							continue;
							
						playerPosition.insert({i,playerOutAux[playerOutIndexAux]});
						spriteList[i].x.setPosition( waypointList[0].x , waypointList[0].y + playerPosition[i]);
						playerOutIndexAux++;
								
						//https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
						random_device dev;
						mt19937 rng(dev());
						uniform_int_distribution<mt19937::result_type> dist6(0,10); // distribution in range [0, 10]
						float velS = 0.3 + dist6(rng) * 0.01;
						cout << velS << endl;
						
						spriteList[i].v = velS;
					}
					
					flagPlay = true;
				}
			}
        }

        window.clear();

		window.draw(spriteBackgroundImage);
		
		//if we are doing a play don't show initial menu
		if(!flagPlay && placeRace.size() < 6)
		{
			window.draw(textStart);
			window.draw(textCreditsIn);
			window.draw(textCreditsOut);
			window.draw(textCredits);
			window.draw(textCoin);
		}
		
		//if all of the playing characters finished the race, show end menu and calculate the winners
		if( placeRace.size() == 6)
		{
			elapsedPlay = playClock.getElapsedTime();
			cout << elapsedPlay.asSeconds() << endl;
			
			string auxWinner = raceResults(placeRace,playerMap);
			cout << auxWinner << endl;
			textWinners.setString(auxWinner);
			flagPlay = false;
			
			window.draw(spriteFinalMenu);
			window.draw(textWinners);
			window.draw(text1);
			window.draw(text2);
			window.draw(text3);
		}
		
		//if we pressed enter
		if(flagPlay)
		{	
			//change sprites of the playing characters based on play
			for(int i=0;i<=6;i++)
			{
				if( i == playerOut)
					continue;
				
				//velocity is in the order of magnitude [0.3-0.4] while sprite change speed looks ok at [0.03-0.04]	
				spriteList[i].spriteV += spriteList[i].v*0.1;
				int currentFrameAux = (spriteList[i].currentFrame + int(spriteList[i].spriteV)) % spriteList[i].size;
				if( spriteList[i].currentFrame != currentFrameAux)
				{
					spriteList[i].currentFrame = currentFrameAux;
					spriteList[i].x.setTexture( spriteList[i].frames[ spriteList[i].currentFrame ] );
					spriteList[i].spriteV = 0;
				}
			}
		
			//for any playing character we see the distance to his next waypoint, if its close we make him go to the next one
			for(int i=0;i<=6;i++)
			{				
				if( spriteList[i].currentWaypoint >=  waypointList.size() || i == playerOut)
					continue;
					
				double distance = sqrt( 
				(spriteList[i].x.getPosition().x - waypointList[ spriteList[i].currentWaypoint ].x )*(spriteList[i].x.getPosition().x - waypointList[ spriteList[i].currentWaypoint ].x) +
				(spriteList[i].x.getPosition().y - (waypointList[ spriteList[i].currentWaypoint ].y + playerPosition[i]) )*(spriteList[i].x.getPosition().y - (waypointList[ spriteList[i].currentWaypoint ].y + playerPosition[i]))  ); 
				if( distance < RADIUS)
				{
					spriteList[i].currentWaypoint++;	
					//if a character finished a race put him in placeRace list
					if( spriteList[i].currentWaypoint >= waypointList.size() )
					{
						placeRace.push_back(i);
						continue;
					}
				}
				//we need a unit vector so we can have small movement
				sf::Vector2f direction = normalize( waypointList[ spriteList[i].currentWaypoint ] - spriteList[i].x.getPosition() + sf::Vector2f(0,playerPosition[i]) );
				
				//cout << distance << endl;
				//cout << spriteList[i].v * direction.x << " " << spriteList[i].v * direction.y << endl;
				//cout <<  spriteList[i].x.getPosition().x + VELOCITY * direction.x << " " <<  spriteList[i].x.getPosition().y + VELOCITY * direction.y ;
				spriteList[i].x.move( spriteList[i].v * direction.x, spriteList[i].v  * direction.y );

				spriteList[i].x.setScale(sf::Vector2f(2,2)); 
				window.draw(spriteList[i].x);			
			}
		}

		
        window.display();
    }


	sf::Time elapsedProgram = programClock.getElapsedTime();
	cout << elapsedProgram.asSeconds() << endl;
    return 0;
}
