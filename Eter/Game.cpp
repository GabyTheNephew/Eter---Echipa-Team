#include "Game.h"


Game Game::m_current_Instance;
bool Game::s_forceStop = false;

#pragma region Getters
QString Game::getExplosionPreviewGrid() const
{
	if (!m_explosion)
	{
		return "No explosion preview available.";
	}

	int16_t gridSize = m_boardMaxSize;

	std::vector<std::vector<QString>> grid(gridSize, std::vector<QString>(gridSize, "  -  "));

	for (const auto& [x, y, action] : m_explosion->getPositions())
	{
		if (x >= 0 && x < gridSize && y >= 0 && y < gridSize)
		{
			switch (action)
			{
			case ActionType::explode:
				grid[x][y] = "EXP";
				break;
			case ActionType::giveBack:
				grid[x][y] = "GIVE";
				break;
			case ActionType::hole:
				grid[x][y] = "HOLE";
				break;
			}
		}
	}

	QString result;
	for (int16_t i = 0; i < gridSize; i++)
	{
		for (int16_t j = 0; j < gridSize; j++)
		{
			result += grid[i][j];

			if (j < gridSize - 1) {
				result += " | ";
			}
		}

		result += "\n";
		if (i < gridSize - 1) {
			result += QString("-").repeated(gridSize * 7 - 1) + "\n";
		}
	}

	result += "\n\nLegend:\n";
	result += "EXP  = Remove from game\n";
	result += "GIVE = Return to hand\n";
	result += "HOLE = Create hole\n";
	result += "-    = No effect\n";

	return result;
}

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
bool Game::isPlayer1IllusionUsed() const
{
	return m_player1IllusionUsed;
}

bool Game::isPlayer2IllusionUsed() const
{
	return m_player2IllusionUsed;
}
int Game::getTimerDuration() const {
	return timerDuration;
}


int Game::getPlayer1RemainingTime() const {
	return player1RemainingTime;
}

int Game::getPlayer2RemainingTime() const {
	return player2RemainingTime;
}

Game& Game::get_Instance()
{
	return m_current_Instance;
}
Board& Game::getBoard() {
	return m_gameBoard;
}

const Board& Game::getBoard() const {
	return m_gameBoard;
}

bool Game::areExplosionsEnabled() const
{
	return m_explosionsEnabled;
}

void Game::setIllusionsEnabled(bool enabled)
{
	m_illusionsEnabled = enabled;
}

bool Game::areIllusionsEnabled() const
{
	return m_illusionsEnabled;
}

bool Game::getPlayerMoveCompleted() const
{
	return playerMoveCompleted;
}


bool Game::checkPlayExplosion(Board& m_board)const
{
	if (!areExplosionsEnabled() || m_explosionActivated) {
		return false;
	}

	int16_t completedRowsColumnsOrDiagonals = 0;
	int16_t minSize = (m_currentGameType == GameType::Training) ? 3 : 4;
	if (m_board.getRowSize() < minSize || m_board.getColumnSize() < minSize)
	{
		qDebug() << "Board too small for explosions:" << m_board.getRowSize() << "x" << m_board.getColumnSize() << "< required" << minSize << "x" << minSize;
		return false;
	}
	for (int16_t i = 0; i < m_board.getRowSize(); i++)
	{
		if (m_board.checkRow(i))
		{
			completedRowsColumnsOrDiagonals++;
		}
	}

	for (int16_t i = 0; i < m_board.getColumnSize(); i++)
	{
		if (m_board.checkColumn(i))
		{
			completedRowsColumnsOrDiagonals++;
		}
	}

	bool mainDiagonalComplete = true;
	bool secondaryDiagonalComplete = true;

	if (m_board.getColumnSize() == m_board.getRowSize())
	{
		for (int16_t i = 0; i < m_board.getRowSize(); i++)
		{
			if (m_board[{i, i}].empty() || m_board[{i, i}].back().getColor() == Color::Hole)
			{
				mainDiagonalComplete = false;
				break;
			}
		}

		for (int16_t i = 0; i < m_board.getRowSize(); i++)
		{
			if (m_board[{i, m_board.getRowSize() - 1 - i}].empty() || m_board[{i, m_board.getRowSize() - 1 - i}].back().getColor() == Color::Hole)
			{
				secondaryDiagonalComplete = false;
				break;
			}
		}

		if (mainDiagonalComplete)
		{
			completedRowsColumnsOrDiagonals++;
		}
		if (secondaryDiagonalComplete)
		{
			completedRowsColumnsOrDiagonals++;
		}
	}


	return completedRowsColumnsOrDiagonals >= 2;
}

bool Game::canActivateExplosion() const
{
	if (!areExplosionsEnabled()) {
		return false;
	}

	if (m_explosionActivated)
	{
		return false;
	}

	return checkPlayExplosion(const_cast<Board&>(m_gameBoard));
}
Player& Game::getCurrentPlayer() {
	return (currentPlayer == Color::Red) ? player1 : player2;
}




#pragma endregion

#pragma region Setters
void Game::endCurrentRound() {

	player1.ResetVector();
	player2.ResetVector();
	m_gameBoard.resizeBoard(1);
	incrementRoundCounter();
	currentPlayer = Color::Red;


	resetTimers();


	int16_t winCondition;
	switch (m_currentGameType) {
	case GameType::Training:
		winCondition = 2;
		break;
	case GameType::MageDuel:
	case GameType::Power:
		winCondition = 3;
		break;
	case GameType::MageDuelAndPower:
		winCondition = 2;
		break;
	default:
		winCondition = 2;
		break;
	}

	if (player1RoundsWon >= winCondition || player2RoundsWon >= winCondition) {
		emit gameEnded();
	}
}
void Game::resetMageFlags()
{
	m_player1MageUsed = false;
	m_player2MageUsed = false;
	m_player1PowerUsed = false;
	m_player2PowerUsed = false;
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;
}
void Game::setPlayer1MageUsed(bool used) {
	m_player1MageUsed = used;
}

void Game::setPlayer2MageUsed(bool used) {
	m_player2MageUsed = used;
}

void Game::setPlayer1PowerUsed(bool used) {
	m_player1PowerUsed = used;
}

void Game::setPlayer2PowerUsed(bool used) {
	m_player2PowerUsed = used;
}


void Game::setUserCredentials(const QString& email, const QString& password) {
	m_userEmail = email;
	m_userPassword = password;
}

void Game::setPlayer1IllusionUsed(bool used)
{
	m_player1IllusionUsed = used;
}

void Game::setPlayer2IllusionUsed(bool used)
{
	m_player2IllusionUsed = used;
}

void Game::forceStop() {
	s_forceStop = true;
	QApplication::quit();
}

void Game::setExplosionsEnabled(bool enabled)
{

	m_explosionsEnabled = enabled;

	if (!enabled)
	{
		m_explosion.reset();
		m_explosionActivated = false;
	}
}

void Game::setPlayerMoveCompleted(bool completed)
{
	if (m_restrictionRemainingTurns > 0)
	{
		m_restrictionRemainingTurns--;
		if (m_restrictionRemainingTurns == 0)
		{
			m_restrictedPositions.clear();
		}
	}
	playerMoveCompleted = completed;
}


#pragma endregion

#pragma region Explosions
void Game::handleExplosionActivation()
{
	if (!canActivateExplosion())
	{
		return;
	}
	Color playerWhoTriggered = currentPlayer;

	QMessageBox::StandardButton reply = QMessageBox::question(
		nullptr,
		"Explosion Avalible",
		QString("Player %1,you can activate an explosion!").arg(playerWhoTriggered == Color::Red ? "1" : "2"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No
	);

	if (reply == QMessageBox::Yes) {
		activateExplosion();
	}
}
void Game::showExplosionRotationDialog()
{
	if (!m_explosion) {
		m_explosion = std::make_unique<Explosion>();
	}
	bool confirmed = false;

	while (!confirmed) {
		QString previewText = getExplosionPreviewGrid();

		QMessageBox previewBox;
		previewBox.setWindowTitle("Explosion Preview");
		previewBox.setText("Current explosion effects:");
		previewBox.setDetailedText(previewText);

		QPushButton* rotateLeftBtn = previewBox.addButton("Rotate 90° Left", QMessageBox::ActionRole);
		QPushButton* rotateRightBtn = previewBox.addButton("Rotate 90° Right", QMessageBox::ActionRole);
		QPushButton* rotate180Btn = previewBox.addButton("Rotate 180°", QMessageBox::ActionRole);
		QPushButton* confirmBtn = previewBox.addButton("Confirm Explosion", QMessageBox::AcceptRole);
		QPushButton* cancelBtn = previewBox.addButton("Cancel", QMessageBox::RejectRole);

		previewBox.exec();

		if (previewBox.clickedButton() == rotateLeftBtn) {
			m_explosion->rotationLeft(m_boardMaxSize);
		}
		else if (previewBox.clickedButton() == rotateRightBtn) {
			m_explosion->rotationRight(m_boardMaxSize);
		}
		else if (previewBox.clickedButton() == rotate180Btn) {
			m_explosion->rotationDown(m_boardMaxSize);
		}
		else if (previewBox.clickedButton() == confirmBtn) {
			applyExplosionEffects(*m_explosion);
			m_explosionActivated = true;
			QMessageBox::information(nullptr, "Explosion!", "The explosion has been activated!");
			confirmed = true;
		}
		else {

			confirmed = true;
		}
	}
}
void Game::applyExplosionEffects(const Explosion& explosion)
{
	if (wouldCreateIsolatedCards(explosion)) {
		QMessageBox::information(nullptr, "Invalid Explosion",
			"Some effects were ignored to prevent isolated cards.");
		return;
	}


	for (const auto& [x, y, action] : explosion.getPositions())
	{
		if (x < 0 || x >= m_gameBoard.getRowSize() || y < 0 || y >= m_gameBoard.getColumnSize())
		{
			continue;
		}

		switch (action)
		{
		case ActionType::explode:
		{
			if (!m_gameBoard[{x, y}].empty()) {
				m_gameBoard[{x, y}].clear();
			}
			break;
		}

		case ActionType::giveBack:
		{
			if (!m_gameBoard[{x, y}].empty())
			{
				SimpleCard topCard = m_gameBoard[{x, y}].back();

				if (topCard.getColor() == Color::Red || topCard.getColor() == Color::usedRed)
				{
					SimpleCard returnCard(topCard.getValue(), Color::Red);
					player1.makeCardValid(returnCard);
					player1.addRestrictedCard(returnCard);
				}
				else if (topCard.getColor() == Color::Blue || topCard.getColor() == Color::usedBlue)
				{
					SimpleCard returnCard(topCard.getValue(), Color::Blue);
					player2.makeCardValid(returnCard);
					player2.addRestrictedCard(returnCard);
				}

				m_gameBoard.popCard({ x, y });
			}
			break;
		}
		case ActionType::hole:
		{
			if (!m_gameBoard[{x, y}].empty())
			{
				m_gameBoard[{x, y}].clear();
			}
			m_gameBoard.pushCard(SimpleCard(0, Color::Hole), { x, y });
			break;
		}
		}
	}
}
bool Game::wouldCreateIsolatedCards(const Explosion& explosion) const
{
	Board test_board = m_gameBoard;

	for (const auto& [x, y, action] : explosion.getPositions())
	{
		if (x < 0 || x >= test_board.getRowSize() || y < 0 || y >= test_board.getColumnSize())
		{
			continue;
		}

		switch (action)
		{
		case ActionType::explode:
		{
			if (!test_board[{x, y}].empty())
			{
				test_board[{x, y}].clear();
			}
			break;
		}
		case ActionType::giveBack:
		{
			if (!test_board[{x, y}].empty())
			{
				test_board[{x, y}].clear();
			}
			break;
		}
		case ActionType::hole:
		{
			if (!test_board[{x, y}].empty())
			{
				test_board[{x, y}].clear();
			}
			test_board.pushCard(SimpleCard(0, Color::Hole), { x, y });
			break;
		}
		}
	}

	return !areCardsConnected(test_board);
}
bool Game::wouldMageCreateIsolatedCards(const Board& testBoard) const
{
	return !areCardsConnected(testBoard);
}
bool Game::areCardsConnected(const Board& test_board) const
{
	std::vector<std::pair<int16_t, int16_t>> validCards;

	for (int16_t i = 0; i < test_board.getRowSize(); ++i) {
		for (int16_t j = 0; j < test_board.getColumnSize(); ++j) {
			if (!test_board[{i, j}].empty() &&
				test_board[{i, j}].back().getColor() != Color::Hole) {
				validCards.push_back({ i, j });
			}
		}
	}


	if (validCards.size() <= 1)
	{
		return true;
	}

	for (const auto& [x, y] : validCards)
	{
		bool hasNeighbours = false;

		std::vector<std::pair<int16_t, int16_t>> directions = {
			{-1, 0}, {1, 0}, {0, -1}, {0, 1},
			{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
		};

		for (const auto& [dx, dy] : directions)
		{
			int16_t newX = x + dx;
			int16_t newY = y + dy;

			if (newX >= 0 && newX < test_board.getRowSize() && newY >= 0 && newY < test_board.getColumnSize() && !test_board[{newX, newY}].empty() && test_board[{newX, newY}].back().getColor() != Color::Hole)
			{
				hasNeighbours = true;
				break;
			}


		}
		if (!hasNeighbours)
		{
			return false;
		}

	}
	return true;
}

#pragma endregion

#pragma region GameModes
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
		m_explosionActivated = false;
		m_explosion.reset();
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
		if (m_timerEnabled) {
			startPlayerTimer();
		}

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					gameWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 2's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					gameWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 1's turn.";
				}

				if (canActivateExplosion()) {
					handleExplosionActivation();
					gameWindow->updateBoardView();
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
	m_explosionActivated = false;
	m_explosion.reset();


	if (m_illusionsEnabled) {
		canPlayIllusion = std::make_pair(true, true);
	}
	else {
		canPlayIllusion = std::nullopt;
	}

	player1 = Player("Name1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(4, Color::Red) }, PastCards);
	player2 = Player("Name1", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue) }, PastCards);


	auto* trainingWindow = new SecondaryWindow("Training", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), "", "", "", "", false, false);


	trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
	trainingWindow->setBoard(m_gameBoard, 3);
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
		if (m_timerEnabled) {
			startPlayerTimer();
		}

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 2's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 1's turn.";
				}

				if (canActivateExplosion()) {
					handleExplosionActivation();
					trainingWindow->updateBoardView();
				}
				playerMoveCompleted = false;
			}
			else
			{
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
	m_explosionActivated = false;
	m_explosion.reset();

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
	auto* trainingWindow = new SecondaryWindow("Mage Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), QString::fromStdString(player1.getMage()), QString::fromStdString(player2.getMage()), "", "", true, false);


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
		if (m_timerEnabled) {
			startPlayerTimer();
		}

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 2's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 1's turn.";
				}
				if (canActivateExplosion()) {
					handleExplosionActivation();
					trainingWindow->updateBoardView();
				}
				playerMoveCompleted = false;
			}
			else
			{
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
	m_explosionActivated = false;
	m_explosion.reset();

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
		if (m_timerEnabled) {
			startPlayerTimer();
		}

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 2's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 1's turn.";
				}

				if (canActivateExplosion()) {
					handleExplosionActivation();
					trainingWindow->updateBoardView();
				}
				playerMoveCompleted = false;
			}
			else
			{
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
	m_explosionActivated = false;
	m_explosion.reset();

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
	auto* trainingWindow = new SecondaryWindow("Power & Mage Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), QString::fromStdString(player1.getMage()), QString::fromStdString(player2.getMage()), QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString()), QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString()), true, true);


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
		if (m_timerEnabled) {
			startPlayerTimer();
		}

		bool roundInProgress = true;
		while (roundInProgress) {
			if (s_forceStop) return;
			QCoreApplication::processEvents();

			if (playerMoveCompleted) {
				if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Blue);
					currentPlayer = Color::Blue;
					qDebug() << "Player 2's turn.";
				}
				else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
					trainingWindow->setCurrentPlayer(Color::Red);
					currentPlayer = Color::Red;
					qDebug() << "Player 1's turn.";
				}
				if (canActivateExplosion()) {
					handleExplosionActivation();
					trainingWindow->updateBoardView();
				}

				playerMoveCompleted = false;
			}
			else
			{
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



#pragma endregion

#pragma region Timer
void Game::setTimerDuration(int seconds) {
	timerDuration = seconds;
	resetTimers();
}


void Game::startPlayerTimer() {
	if (!m_timerEnabled) return;

	stopPlayerTimer();

	if (currentPlayer == Color::Red) {
		player1Timer->start(1000);
		timerActive = true;
	}
	else {
		player2Timer->start(1000);
		timerActive = true;
	}
}

void Game::stopPlayerTimer() {
	player1Timer->stop();
	player2Timer->stop();
	timerActive = false;
}


void Game::resetTimers() {
	player1RemainingTime = timerDuration;
	player2RemainingTime = timerDuration;
	stopPlayerTimer();
}

void Game::onPlayerTimerTimeout() {
	if (currentPlayer == Color::Red) {
		player1RemainingTime--;
		if (player1RemainingTime <= 0) {
			QMessageBox::information(nullptr, "Time's Up!", "Player 1's time expired! Player 2 wins this round!");
			player2RoundsWon++;
			endCurrentRound();
			return;
		}
	}
	else {
		player2RemainingTime--;
		if (player2RemainingTime <= 0) {
			QMessageBox::information(nullptr, "Time's Up!", "Player 2's time expired! Player 1 wins this round!");
			player1RoundsWon++;
			endCurrentRound();
			return;
		}
	}
}

#pragma endregion

#pragma region Extras
GameType Game::stringToGameType(std::string_view word)
{
	return stringToEnum<GameType>(std::string(word));
}

std::string_view Game::gameTypeToString(GameType gameType) const
{

	static std::string result = enumToString(gameType);
	return result;
}



void Game::incrementRoundCounter()
{
	this->m_round_Counter++;
	m_player1IllusionUsed = false;
	m_player2IllusionUsed = false;

	player1.clearRestrictedCards();
	player2.clearRestrictedCards();
	m_explosionActivated = false;
	m_explosion.reset();

	resetTimers();
}


void Game::activateExplosion()
{
	if (!m_explosion) {
		m_explosion = std::make_unique<Explosion>();
		m_explosion->vectorInstantiation(m_boardMaxSize);
	}

	showExplosionRotationDialog();
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

	if (m_timerEnabled) {
		stopPlayerTimer();
	}
}



#pragma endregion


