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
	int16_t maxRounds = 3;
	const int16_t matrixMaxSize = 3;
	std::vector<SimpleCard> PastCards;
	// bool canPlayIllusion;
	std::optional<std::pair<bool, bool>> canPlayIllusion;
	int16_t player1RoundsWon = 0;
	int16_t player2RoundsWon = 0;

	if (m_illusionsEnabled)
	{
		canPlayIllusion = std::make_pair(true, true);
	}
	else
	{
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1,Color::Red),SimpleCard(1,Color::Red),SimpleCard(2,Color::Red) ,SimpleCard(2,Color::Red) ,SimpleCard(3,Color::Red),SimpleCard(3,Color::Red),SimpleCard(4,Color::Red) }, PastCards);
	player2 = Player("Name2", { SimpleCard(1,Color::Blue),SimpleCard(1,Color::Blue),SimpleCard(2,Color::Blue) ,SimpleCard(2,Color::Blue) ,SimpleCard(3,Color::Blue),SimpleCard(3,Color::Blue),SimpleCard(4,Color::Blue) }, PastCards);



	while (m_round_Counter <= maxRounds)
	{
		PastCards.clear();
		m_gameBoard.resizeBoard(1);
		m_gameBoard.print();

		while (true)
		{
			if (player1.numberofValidCards() > 0)
			{
				std::cout << "Player 1's turn\n";

				//Explosion related
				if (checkPlayExplosion(m_gameBoard))
				{
					std::cout << "Do you want to play the explosion?\n";
					std::cout << m_explosion.value();

					std::string explosionInput;
					std::cin >> explosionInput;

					if (explosionInput == "yes")
					{
						std::cout << "PLAY THE EXPLOSION\n";
					}
					else
					{
						m_explosion.reset();
					}
				}

				SimpleCard chosenCard = player1.chooseCard();
				if (chosenCard.getValue() != 0)
				{
					player1.playCard(chosenCard, m_gameBoard, PastCards, canPlayIllusion);
				}
				m_gameBoard.print();

			}
			if (m_gameBoard.checkWin() == Board::State::Win)
			{
				std::cout << "Player 1 wins\n";
				player1RoundsWon++;
				break;
			}

			if (player2.numberofValidCards() > 0)
			{
				std::cout << "Player 2's turn\n";
				SimpleCard chosenCard = player2.chooseCard();
				if (chosenCard.getValue() != 0)
				{
					player2.playCard(chosenCard, m_gameBoard, PastCards, canPlayIllusion);
				}

				m_gameBoard.print();
			}
			if (m_gameBoard.checkWin() == Board::State::Win)
			{
				std::cout << "Player 2 wins\n";
				player2RoundsWon++;
				break;
			}
			if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0)
			{
				if (m_gameBoard.checkWin(true) == Board::State::RedWin)
				{
					std::cout << "Win by Player 1\n ";
					player1RoundsWon++;
					break;
				}
				if (m_gameBoard.checkWin(true) == Board::State::BlueWin)
				{
					std::cout << "Win by Player 2\n ";
					player2RoundsWon++;
					break;
				}
				if (m_gameBoard.checkWin() == Board::State::Draw)
				{
					std::cout << "Draw\n";
					player1RoundsWon++;
					player2RoundsWon++;
					break;
				}
			}
		}
		player1.ResetVector();
		player2.ResetVector();
		m_gameBoard.clear();
		incrementRoundCounter();

		if (player1RoundsWon == 2)
		{
			std::cout << "Player 1 won the game! Congratulations!\n";
			break;
		}
		if (player2RoundsWon == 2)
		{
			std::cout << "Player 2 won the game! Congratulations!\n";
			break;
		}
	}
	

}

void Game::startMageDuel()
{
	this->m_round_Counter = 1;
	int16_t maxRounds = 4;
	//vector
	// eter
	// get a mage
	//best of 5
}

void Game::startPowerDuel()
{
	this->m_round_Counter = 1;
	int16_t maxRounds = 4;
	//vector
	// eter
	// select 2 powers(!=)
	//best of 5
}

void Game::startTournament()
{
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;
	//vector
	//best of 5
}

void Game::showExplosionMenu()
{
	std::string input;

	
	std::cout << "=== Explosion Menu ===\n";
	std::cout << 
		"Type 'explosions' to enable explosions, 'no explosions' to disable them, 'illusions' to enable illusions, 'no illusions' to disable them or 'start' to begin the game:\n";

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
		else if (input == "illusions")
		{
			setIllusionsEnabled(true);
			std::cout << "Illusions enabled!\n";
		}
		else if (input == "no illusions")
		{
			setIllusionsEnabled(false);
			std::cout << "Illusions disabled!\n";
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
	showExplosionMenu();
	
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
	//m_explosionsEnabled = enabled;
	m_explosion.emplace(Explosion());
}

bool Game::areExplosionsEnabled() const
{
	return m_explosion.has_value();
}

void Game::setIllusionsEnabled(bool enabled)
{
	m_illusionsEnabled = enabled;
}

bool Game::areIllusionsEnabled() const
{
	return m_illusionsEnabled;
}


void Game::incrementRoundCounter()
{
	this->m_round_Counter++; //we use it to increment the ammount of rounds further into the game
}

bool Game::checkPlayExplosion(Board& m_board)
{
	if (!m_explosion.has_value())
	{
		return false;
	}

	int16_t count = 0;
	for (int16_t i = 0; i < m_board.getSize(); i++)
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


