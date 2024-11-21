#include "Game.h"


Game Game::m_current_Instance;


Game& Game::get_Instance()
{
	return m_current_Instance; 
}

Game::GameType Game::stringToGameType(std::string_view word)
{
	if (word == "Training")
		return GameType::Training;
	else
		if (word == "MageDuel")
			return GameType::MageDuel;
		else
			if (word == "Power")
				return GameType::Power;
			else
				if (word == "Tournament")
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
	m_gameBoard.resizeBoard(1);
	uint16_t chosen_card;
	player1 = Player("Name1", { SimpleCard(1,"red"),SimpleCard(1,"red"),SimpleCard(2,"red") ,SimpleCard(2,"red") ,SimpleCard(3,"red"),SimpleCard(3,"red"),SimpleCard(4,"red") });
	player2 = Player("Name2", { SimpleCard(1,"blue"),SimpleCard(1,"blue"),SimpleCard(2,"blue") ,SimpleCard(2,"blue") ,SimpleCard(3,"blue"),SimpleCard(3,"blue"),SimpleCard(4,"blue") });
	
	while (m_round_Counter <= maxRounds)
	{

		while (player1.numberofValidCards()!=0 || player1.numberofValidCards()!= 0||m_gameBoard.win()!="red"||m_gameBoard.win()!="blue")
		{
			
			player1.playCard(player1.chooseCard(), m_gameBoard);

			player2.playCard(player2.chooseCard(), m_gameBoard);
			
		}


		player1.ResetVector();
		player2.ResetVector();
		m_gameBoard.clear();
		incrementRoundCounter();
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

void Game::showMenu()
{
	std::string input;

	
	std::cout << "=== Explosion Menu ===\n";
	std::cout << "Type 'explosions' to enable explosions, 'no explosions' to disable them, or 'start' to begin the game:\n";

	while (true)
	{
		std::getline(std::cin, input);

		if (input == "explosions")
		{
			setExplosionsEnabled(true);
			std::cout << "Explosions enabled!\n";
		}
		else if (input == "no explosions")
		{
			setExplosionsEnabled(false);
			std::cout << "Explosions disabled!\n";
		}
		else if (input == "start")
		{
			std::cout << "Starting the game...\n";
			break; // Exit the loop to start the game
		}
		else
		{
			std::cout << "Unknown command. Please type 'explosions', 'no explosions', or 'start'.\n";
		}
	}
}

void Game::startGame(GameType selectedGameType)
{	
	showMenu();
	
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

void Game::setExplosionsEnabled(bool enabled)
{
	m_explosionsEnabled = enabled;
}

bool Game::areExplosionsEnabled() const
{
	return m_explosionsEnabled;
}


void Game::incrementRoundCounter()
{
	this->m_round_Counter++;//we use it to increment the ammount of rounds farther int the game
}

bool Game::checkPlayExplosion(Board& m_board)
{
	if (!m_explosionsEnabled)
	{
		return false;
	}

	uint8_t count = 0;
	for (int i = 0; i < m_board.getBoard().size(); i++)
	{
		if (m_board.checkColumn(i) == true)
		{
			count++;
		}if (m_board.checkRow(i) == true)
		{
			count++;
		}

		if (count >= 2)
		{
			return true;
		}
	}
	return false;
}


