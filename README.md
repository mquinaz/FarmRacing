# Farm Racing

C++ SFML 2D game

![Farm Racing](https://github.com/mquinaz/HorseRacing/blob/main/resource/game.png)

The idea based on kentucky derby/variations on the casino game. UI based on the following arcade game:

![Mortal Kombat2](https://github.com/mquinaz/HorseRacing/blob/main/resource/mortalKombatArcade.jpg)
## Resources taken from:

shorturl.at/iGORV

shorturl.at/puHZ4

shorturl.at/psGX7

shorturl.at/coOPZ

shorturl.at/zBQZ1

shorturl.at/efET4

https://br.freepik.com/vetores-premium/lucky-husky-game-sprites_1420548.htm

https://www.shutterstock.com/search/pigeon+sprite

https://www.istockphoto.com/pt/ilustra%C3%A7%C3%B5es/wooden-menu-boards-cartoons


Edited with Gimp for cropping and https://www2.lunapic.com/editor/ to remove background transparency of sprites.

## C++ game development challenge

For now the game only works at linux with:

### g++ -c main.cpp

### g++ main.o -o farmrace -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

or using CMake:

### cmake -B build

### cmake --build build

And moving the resources folder to the build folder
