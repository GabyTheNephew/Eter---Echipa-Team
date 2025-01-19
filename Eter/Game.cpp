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
				else
					if (word == "MageDuelAndPower")
						return GameType::MageDuelAndPower;
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
				else
					if (gameType == GameType::MageDuelAndPower)
						return "MageDuelAndPower";
}

Board& Game::getBoard() {
	return m_gameBoard; // Returnează referința către m_gameBoard
}

const Board& Game::getBoard() const {
	return m_gameBoard; // Returnează referința constantă
}

void Game::startTraining() {
	m_gameBoard = Board(1);
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;
	std::vector<SimpleCard> PastCards;
	std::optional<std::pair<bool, bool>> canPlayIllusion;
	int16_t player1RoundsWon = 0;
	int16_t player2RoundsWon = 0;

	if (m_illusionsEnabled) {
		canPlayIllusion = std::make_pair(true, true);
	}
	else {
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1, Color::usedRed),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(4, Color::Red) }, PastCards);
	player2 = Player("Name1", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue) }, PastCards);


	auto* trainingWindow = new SecondaryWindow("Training", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance());


	trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
	trainingWindow->setBoard(m_gameBoard);
	trainingWindow->setPlayer1Cards(player1.getVector());
	trainingWindow->setPlayer2Cards(player2.getVector());

	connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);

	trainingWindow->show();

	currentPlayer = Color::Red; // Player 1 începe
	playerMoveCompleted = false;

	while (m_round_Counter <= maxRounds) {
		PastCards.clear();

		// Începem runda
		trainingWindow->setCurrentPlayer(currentPlayer);

		bool roundInProgress = true;
		while (roundInProgress) {
			QCoreApplication::processEvents(); // Procesează evenimentele interfeței

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 1's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 2's turn.";
				}
			}

			if (playerMoveCompleted) {
				playerMoveCompleted = false; // Resetăm starea pentru următoarea mutare
			}
			else {
				continue; // Așteptăm finalizarea mutării
			}

			// Verificăm câștigătorul
			if (m_gameBoard.checkWin() == Board::State::Win) {
				if (currentPlayer == Color::Red) {
					qDebug() << "Player 1 wins!";
					player1RoundsWon++;
				}
				else {
					qDebug() << "Player 2 wins!";
					player2RoundsWon++;
				}
				roundInProgress = false;
			}

			// Condiții de egalitate
			if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
				auto state = m_gameBoard.checkWin(true);
				if (state == Board::State::RedWin) {
					qDebug() << "Player 1 wins the round.";
					player1RoundsWon++;
				}
				else if (state == Board::State::BlueWin) {
					qDebug() << "Player 2 wins the round.";
					player2RoundsWon++;
				}
				else if (state == Board::State::Draw) {
					qDebug() << "Round is a draw.";
					player1RoundsWon++;
					player2RoundsWon++;
				}
				roundInProgress = false;
			}
		}

		// Resetăm pentru următoarea rundă
		player1.ResetVector();
		player2.ResetVector();
		m_gameBoard.clear();
		incrementRoundCounter();

		if (player1RoundsWon == 2) {
			qDebug() << "Player 1 won the game!";
			break;
		}

		if (player2RoundsWon == 2) {
			qDebug() << "Player 2 won the game!";
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
void Game::startMageDuelAndPower()
{

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
	//showExplosionMenu();
	// TODO: IntermediateMenu

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
	case (GameType::MageDuelAndPower):
		startMageDuelAndPower();
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

Player& Game::getCurrentPlayer() {
	return (currentPlayer == Color::Red) ? player1 : player2;
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


void Game::handleBoardClick(int row, int col) {
	qDebug() << "Clicked on board at (" << row << ", " << col << ")";

	if (!m_gameBoard.canBePlaced(row, col)) {
		qDebug() << "Position is not valid for placement.";
		return;
	}

	if (currentPlayer == Color::Red) {
		SimpleCard selectedCard = player1.chooseCard();
		m_gameBoard.pushCard(selectedCard, { row, col });
		qDebug() << "Player 1 placed card at (" << row << ", " << col << ")";
		currentPlayer = Color::Blue; // Trecem la Player 2
	}
	else if (currentPlayer == Color::Blue) {
		SimpleCard selectedCard = player2.chooseCard();
		m_gameBoard.pushCard(selectedCard, { row, col });
		qDebug() << "Player 2 placed card at (" << row << ", " << col << ")";
		currentPlayer = Color::Red; // Trecem la Player 1
	}

	playerMoveCompleted = true; // Mutarea curentă este completă
}


