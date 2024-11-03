#include "Game.h"

Game* Game::m_current_Instance = nullptr;




Game* Game::get_Instance()
{
	if (m_current_Instance == nullptr)
	{
		m_current_Instance = new Game();
	}

	return m_current_Instance;
}

Game::GameType Game::stringToGameType(std::string_view cuvant)
{
	if (cuvant == "Training")
		return GameType::Training;
	else
		if (cuvant == "MageDuel")
			return GameType::MageDuel;
		else
			if (cuvant == "Power")
				return GameType::Power;
			else
				if (cuvant == "Tournament")
					return GameType::Tournament;
}

std::string_view Game::gameTypeToString(GameType gameType) const
{
	
	if (gameType == GameType::Training)
		return "Training";
	else
		if (gameType == GameType::MageDuel)
			return "MageDuel";
		else
			if (gameType == GameType::Power)
				return "Power";
			else
				if (gameType == GameType::Tournament)
					return "Tournament";
	
}

void Game::startGame()
{
	

	
}



uint8_t Game::getRoundCounter()
{
	return m_round_Counter;
}

void Game::setRoundCounter()
{
	this->m_round_Counter++;//we use it to increment the ammount of rounds farther int the game
}



Game::~Game()
{
	delete m_current_Instance;
}
