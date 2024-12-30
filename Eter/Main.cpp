#include "Game.h"
#include "iostream";


int main()
{
	Game& game = Game::get_Instance();
	game.startGame(Game::GameType::Training);


}