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

void Game::startTraining()
{
	
	this->m_round_Counter = 1;
	uint8_t maxRounds=3;
	while (m_round_Counter <= maxRounds)
	{
		m_gameBoard.resizeBoard(3);
		//vector
	}
	
}

void Game::startMageDuel()
{
	this->m_round_Counter = 1;
	uint8_t maxRounds = 4;
	//vector
	// eter
	// get a mage
	//best of 5
}

void Game::startPowerDuel()
{
	this->m_round_Counter = 1;
	uint8_t maxRounds = 4;
	//vector
	// eter
	// select 2 powers(!=)
	//best of 5
}

void Game::startTournament()
{
	this->m_round_Counter = 1;
	uint8_t maxRounds = 3;
	//vector
	//best of 5
}

void Game::startGame(GameType selectedGameType)
{	
	
	switch (selectedGameType)
	{
	case (GameType::Training):
		startTraining();
		break;
	case (GameType::MageDuel):
		startMageDuel();
		break;
	case (GameType::Power):
		startPowerDuel();
		break;
	case (GameType::Tournament):
		startTournament();
		break;
	default:
		break;
	}
	
}


void Game::incrementRoundCounter()
{
	this->m_round_Counter++;//we use it to increment the ammount of rounds farther int the game
}



Game::~Game()
{
	delete m_current_Instance;
}
