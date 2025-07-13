#include "Game.h"


Game Game::m_current_Instance;
bool Game::s_forceStop = false;





GameType Game::getCurrentGameType() const {
	return m_currentGameType;
}

int Game::getRoundCounter() const {
	return m_round_Counter;
}



Color Game::getCurrentPlayerColor() const {
	return currentPlayer;
}

int Game::getPlayer1Score() const {
	return player1RoundsWon;
}

int Game::getPlayer2Score() const {
	return player2RoundsWon;
}
int Game::getBoardMaxSize() const {
	return m_boardMaxSize;
}

bool Game::isTimerEnabled() const {
	return m_timerEnabled;
}

bool Game::isPlayer1MageUsed() const {
	return m_player1MageUsed;
}

bool Game::isPlayer2MageUsed() const {
	return m_player2MageUsed;
}

bool Game::isPlayer1PowerUsed() const {
	return m_player1PowerUsed;
}

bool Game::isPlayer2PowerUsed() const {
	return m_player2PowerUsed;
}

QString Game::getUserEmail() const
{
	return m_userEmail;
}

QString Game::getUserPassword() const
{
	return m_userPassword;
}

void Game::setUserCredentials(const QString& email, const QString& password) {
	m_userEmail = email;
	m_userPassword = password;
}

bool Game::isPlayer1IllusionUsed() const
{
	return m_player1IllusionUsed;
}

bool Game::isPlayer2IllusionUsed() const
{
	return m_player2IllusionUsed;
}

void Game::setPlayer1IllusionUsed(bool used)
{
	m_player1IllusionUsed = used;
}

void Game::setPlayer2IllusionUsed(bool used)
{
	m_player2IllusionUsed = used;
}

void Game::startLoadedGame()
{
	QString windowTitle;
	QString mage1Name = "";
	QString mage2Name = "";
	QString power1Name = "";
	QString power2Name = "";
	bool hasMages = false;
	bool hasPowers = false;
	if (m_round_Counter == 1)
	{
		m_player1IllusionUsed = false;
		m_player2IllusionUsed = false;
	}

	switch (m_currentGameType) {
	case GameType::Training:
		windowTitle = "Training - Loaded Game";
		break;
	case GameType::MageDuel:
		windowTitle = "Mage Duel - Loaded Game";
		hasMages = true;
		mage1Name = QString::fromStdString(player1.getMage());
		mage2Name = QString::fromStdString(player2.getMage());
		break;
	case GameType::Power:
		windowTitle = "Power Duel - Loaded Game";
		hasPowers = true;
		power1Name = QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString());
		power2Name = QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString());
		break;
	case GameType::MageDuelAndPower:
		windowTitle = "Mage & Power Duel - Loaded Game";
		hasMages = true;
		hasPowers = true;
		mage1Name = QString::fromStdString(player1.getMage());
		mage2Name = QString::fromStdString(player2.getMage());
		power1Name = QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString());
		power2Name = QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString());
		break;
	default:
		windowTitle = "Loaded Game";
		break;
	}

	auto* gameWindow = new SecondaryWindow(
		windowTitle,
		QDir::currentPath() + QDir::separator() + "eter.png",
		this,
		mage1Name,
		mage2Name,
		power1Name,
		power2Name,
		hasMages,
		hasPowers
	);

	gameWindow->setAttribute(Qt::WA_DeleteOnClose);
	gameWindow->setBoard(m_gameBoard, m_boardMaxSize);
	gameWindow->setPlayer1Cards(player1.getVector());
	gameWindow->setPlayer2Cards(player2.getVector());
	gameWindow->setCurrentPlayer(currentPlayer);

	connect(gameWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
	gameWindow->show();


	int16_t maxRounds;
	int16_t winCondition;

	switch (m_currentGameType) {
	case GameType::Training:
		maxRounds = 3;
		winCondition = 2;
		break;
	case GameType::MageDuel:
	case GameType::Power:
		maxRounds = 5;
		winCondition = 3;
		break;
	case GameType::MageDuelAndPower:
		maxRounds = 3;
		winCondition = 2;
		break;
	default:
		maxRounds = 3;
		winCondition = 2;
		break;
	}
	while (m_round_Counter <= maxRounds) {
		if (s_forceStop) return;

		playerMoveCompleted = false;
		gameWindow->setCurrentPlayer(currentPlayer);

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					gameWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 1's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					gameWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 2's turn.";
				}
				playerMoveCompleted = false;
			}

	
			int16_t boardSizeForWin = (m_currentGameType == GameType::Training) ? 3 : 4;
			if (m_gameBoard.checkWin(false, boardSizeForWin) == Board::State::Win) {
				if (currentPlayer == Color::Red) {
					qDebug() << "Player 2 wins!";
					player1RoundsWon++;
				}
				else {
					qDebug() << "Player 1 wins!";
					player2RoundsWon++;
				}

				player1.ResetVector();
				player2.ResetVector();
				gameWindow->setPlayer1Cards(player1.getVector());
				gameWindow->setPlayer2Cards(player2.getVector());
				m_gameBoard.resizeBoard(1);
				gameWindow->resetView();
				incrementRoundCounter();
				gameWindow->updateBoardView();
				currentPlayer = Color::Red;
				roundInProgress = false;
			}

			if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
				auto state = m_gameBoard.checkWin(true, boardSizeForWin);
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
				currentPlayer = Color::Red;
				roundInProgress = false;
			}
		}

		if (player1RoundsWon >= winCondition) {
			gameWindow->showWinner("Player 2");
			gameWindow->hide();        
			gameWindow->deleteLater();
			emit gameEnded();
			break;
		}

		if (player2RoundsWon >= winCondition) {
			gameWindow->showWinner("Player 1");
			gameWindow->hide();        
			gameWindow->deleteLater();
			emit gameEnded();
			break;
		}
	}
}

Game& Game::get_Instance()
{
	return m_current_Instance;
}
void Game::forceStop() {
	s_forceStop = true;
	QApplication::quit();
}
GameType Game::stringToGameType(std::string_view word)
{
	return stringToEnum<GameType>(std::string(word));
}

std::string_view Game::gameTypeToString(GameType gameType) const
{

	static std::string result = enumToString(gameType);
	return result;
}

Board& Game::getBoard() {
	return m_gameBoard; 
}

const Board& Game::getBoard() const {
	return m_gameBoard;
}





void Game::startTraining() {
	m_currentGameType = GameType::Training;
	m_boardMaxSize = 3;
	m_gameBoard = Board(1);
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;
	std::vector<SimpleCard> PastCards;
	std::optional<std::pair<bool, bool>> canPlayIllusion;
	player1RoundsWon = 0;
	player2RoundsWon = 0;
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;


	if (m_illusionsEnabled) {
		canPlayIllusion = std::make_pair(true, true);
	}
	else {
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(4, Color::Red) }, PastCards);
	player2 = Player("Name1", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue) }, PastCards);


	auto* trainingWindow = new SecondaryWindow("Training", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(),"","", "", "",false,false);


	trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
	trainingWindow->setBoard(m_gameBoard ,3);
	trainingWindow->setPlayer1Cards(player1.getVector());
	trainingWindow->setPlayer2Cards(player2.getVector());

	connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);

	trainingWindow->show();

	

	while (m_round_Counter <= maxRounds) {
		if (s_forceStop) return;
		PastCards.clear();
		currentPlayer = Color::Red; 
		playerMoveCompleted = false;

		trainingWindow->setCurrentPlayer(currentPlayer);

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents(); 

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
				playerMoveCompleted = false; 
			}
			else {
				continue;
			}

	
			if (m_gameBoard.checkWin() == Board::State::Win) {
				if (currentPlayer == Color::Red) {
					qDebug() << "Player 2 wins!";
					player1RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				else {
					qDebug() << "Player 1 wins!";
					player2RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				trainingWindow->updateBoardView();
				roundInProgress = false;
			}


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


		

		if (player1RoundsWon == 2) {
			trainingWindow->showWinner("Player 2");
			emit gameEnded();
			break;
		}

		if (player2RoundsWon == 2) {
			trainingWindow->showWinner("Player 1");
			emit gameEnded();
			break;
		}

	}
}



void Game::startMageDuel()
{
	m_currentGameType = GameType::MageDuel;
	m_boardMaxSize = 4;
	m_gameBoard = Board(1);
	this->m_round_Counter = 1;
	int16_t maxRounds = 5;
	std::vector<SimpleCard> PastCards;
	std::optional<std::pair<bool, bool>> canPlayIllusion;
	player1RoundsWon = 0;
	player2RoundsWon = 0;
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;

	if (m_illusionsEnabled) {
		canPlayIllusion = std::make_pair(true, true);
	}
	else {
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(3, Color::Red),SimpleCard(4, Color::Red),SimpleCard(5, Color::Red) }, PastCards,true);
	player2 = Player("Name1", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue),SimpleCard(5, Color::Blue) }, PastCards,true);

	auto mage1 = player1.getMageAssignment();
	auto mage2 = player2.getMageAssignment();
	while ((mage1 == mage2 )|| (mage1 % 2 == 0 && mage2 == mage1 + 1) || (mage1 % 2 == 1 && mage2 == mage1 - 1))
	{
		player2.reasignMage();
		mage2 = player2.getMageAssignment();
	}
	auto* trainingWindow = new SecondaryWindow("Mage Duel", QDir::currentPath() + QDir::separator() + "eter.png",  &Game::get_Instance(), QString::fromStdString(player1.getMage()), QString::fromStdString(player2.getMage()), "", "", true,false);


	trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
	trainingWindow->setBoard(m_gameBoard, 4);
	trainingWindow->setPlayer1Cards(player1.getVector());
	trainingWindow->setPlayer2Cards(player2.getVector());

	connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);

	trainingWindow->show();


	qDebug() << "Player1Mage" << player1.getMage();
	qDebug() << "Player2Mage" << player2.getMage();
	while (m_round_Counter <= maxRounds) {
		if (s_forceStop) return;
		PastCards.clear();
		currentPlayer = Color::Red; 
		playerMoveCompleted = false;

		trainingWindow->setCurrentPlayer(currentPlayer);

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents(); 

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
				playerMoveCompleted = false; 
			}
			else {
				continue; 
			}

			
			if (m_gameBoard.checkWin(false, 4) == Board::State::Win) {
				if (currentPlayer == Color::Red) {
					qDebug() << "Player 2 wins!";
					player1RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				else {
					qDebug() << "Player 1 wins!";
					player2RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				trainingWindow->updateBoardView();
				roundInProgress = false;
			}

	
			if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
				auto state = m_gameBoard.checkWin(true,4);
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




		if (player1RoundsWon == 3) {
			trainingWindow->showWinner("Player 2");
			break;
		}

		if (player2RoundsWon == 3) {
			trainingWindow->showWinner("Player 1");
			break;
		}

	}
}

void Game::startPowerDuel() {
	m_currentGameType = GameType::Power;
	m_boardMaxSize = 4;
	m_gameBoard = Board(1);
	this->m_round_Counter = 1;
	int16_t maxRounds = 5;
	std::vector<SimpleCard> PastCards;
	std::optional<std::pair<bool, bool>> canPlayIllusion;
	player1RoundsWon = 0;
	player2RoundsWon = 0;
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;

	if (m_illusionsEnabled) {
		canPlayIllusion = std::make_pair(true, true);
	}
	else {
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(3, Color::Red),SimpleCard(4, Color::Red),SimpleCard(5, Color::Red) }, PastCards, true);
	player2 = Player("Name1", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue),SimpleCard(5, Color::Blue) }, PastCards, true);

	player1.assignPower();
	player2.assignPower();


	while (player1.getPower() == player2.getPower()) {
		player2.reassignPower();
	}
	auto* trainingWindow = new SecondaryWindow("Power Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), "", "", QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString()), QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString()), false, true);


	trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
	trainingWindow->setBoard(m_gameBoard, 4);
	trainingWindow->setPlayer1Cards(player1.getVector());
	trainingWindow->setPlayer2Cards(player2.getVector());

	connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);

	trainingWindow->show();


	qDebug() << "Player1Mage" << player1.getMage();
	qDebug() << "Player2Mage" << player2.getMage();
	while (m_round_Counter <= maxRounds) {
		if (s_forceStop) return;
		PastCards.clear();
		currentPlayer = Color::Red; 
		playerMoveCompleted = false;
	
		trainingWindow->setCurrentPlayer(currentPlayer);

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

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
				playerMoveCompleted = false; 
			}
			else {
				continue; 
			}

	
			if (m_gameBoard.checkWin(false, 4) == Board::State::Win) {
				if (currentPlayer == Color::Red) {
					qDebug() << "Player 2 wins!";
					player1RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				else {
					qDebug() << "Player 1 wins!";
					player2RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				trainingWindow->updateBoardView();
				roundInProgress = false;
			}

			if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
				auto state = m_gameBoard.checkWin(true, 4);
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




		if (player1RoundsWon == 3) {
			trainingWindow->showWinner("Player 2");
			break;
		}

		if (player2RoundsWon == 3) {
			trainingWindow->showWinner("Player 1");
			break;
		}

	}
}

void Game::startTournament()
{
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;

}
void Game::startMageDuelAndPower()
{
	m_currentGameType = GameType::MageDuelAndPower;
	m_boardMaxSize = 4;
	m_gameBoard = Board(1);
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;
	std::vector<SimpleCard> PastCards;
	std::optional<std::pair<bool, bool>> canPlayIllusion;
	player1RoundsWon = 0;
	player2RoundsWon = 0;
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;

	if (m_illusionsEnabled) {
		canPlayIllusion = std::make_pair(true, true);
	}
	else {
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(3, Color::Red),SimpleCard(4, Color::Red),SimpleCard(5, Color::Red) }, PastCards, true);
	player2 = Player("Name1", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue),SimpleCard(5, Color::Blue) }, PastCards, true);

	auto mage1 = player1.getMageAssignment();
	auto mage2 = player2.getMageAssignment();
	while ((mage1 == mage2) || (mage1 % 2 == 0 && mage2 == mage1 + 1) || (mage1 % 2 == 1 && mage2 == mage1 - 1))
	{
		player2.reasignMage();
		mage2 = player2.getMageAssignment();
	}
	player1.assignPower();
	player2.assignPower();


	while (player1.getPower() == player2.getPower()) {
		player2.reassignPower();
	}
	auto* trainingWindow = new SecondaryWindow("Power & Mage Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(),  QString::fromStdString(player1.getMage()), QString::fromStdString(player2.getMage()), QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString()), QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString()), true, true);


	trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
	trainingWindow->setBoard(m_gameBoard, 4);
	trainingWindow->setPlayer1Cards(player1.getVector());
	trainingWindow->setPlayer2Cards(player2.getVector());

	connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);

	trainingWindow->show();


	qDebug() << "Player1Mage" << player1.getMage();
	qDebug() << "Player2Mage" << player2.getMage();
	while (m_round_Counter <= maxRounds) {
		if (s_forceStop) return;
		PastCards.clear();
		currentPlayer = Color::Red; 
		playerMoveCompleted = false;

		trainingWindow->setCurrentPlayer(currentPlayer);

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents(); 

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
				playerMoveCompleted = false; 
			}
			else {
				continue; 
			}

	
			if (m_gameBoard.checkWin(false, 4) == Board::State::Win) {
				if (currentPlayer == Color::Red) {
					qDebug() << "Player 2 wins!";
					player1RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				else {
					qDebug() << "Player 1 wins!";
					player2RoundsWon++;
					player1.ResetVector();
					player2.ResetVector();
					trainingWindow->setPlayer1Cards(player1.getVector());
					trainingWindow->setPlayer2Cards(player2.getVector());
					m_gameBoard.resizeBoard(1);
					trainingWindow->resetView();
					incrementRoundCounter();
				}
				trainingWindow->updateBoardView();
				roundInProgress = false;
			}

		
			if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
				auto state = m_gameBoard.checkWin(true, 4);
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

	


		if (player1RoundsWon == 2) {
			trainingWindow->showWinner("Player 2");
			break;
		}

		if (player2RoundsWon == 2) {
			trainingWindow->showWinner("Player 1");
			break;
		}

	}
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
			break; 
		}
		else
		{
			std::cout << "Unknown command. Please type 'explosions', 'no explosions', or 'start'.\n";
		}
	}
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
	case (GameType::MageDuelAndPower):
		startMageDuelAndPower();
		break;
	default:
		break;
	}

}

void Game::setExplosionsEnabled(bool enabled)
{

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
	this->m_round_Counter++; 
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;
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
		currentPlayer = Color::Blue; 
	}
	else if (currentPlayer == Color::Blue) {
		SimpleCard selectedCard = player2.chooseCard();
		m_gameBoard.pushCard(selectedCard, { row, col });
		qDebug() << "Player 2 placed card at (" << row << ", " << col << ")";
		currentPlayer = Color::Red;
	}

	playerMoveCompleted = true; 
}


