#include "Game.h"
#include "MainWindow.h"
#include "SecondaryWindow.h"

Game Game::m_current_Instance;

// În Game.cpp - Înlocuiește destructorul cu această versiune mai sigură:

Game::~Game() {
    qDebug() << "Game destructor called - starting cleanup";

    try {
        if (currentGameWindow) {
            qDebug() << "Cleaning up currentGameWindow";

            // Disconnect all signals first
            currentGameWindow->blockSignals(true);
            currentGameWindow->disconnect();

            // Hide the window
            currentGameWindow->hide();

            // Schedule for deletion
            currentGameWindow->deleteLater();
            currentGameWindow = nullptr;

            qDebug() << "currentGameWindow cleanup completed";
        }

        // Reset other members
        player1RoundsWon = 0;
        player2RoundsWon = 0;
        m_round_Counter = 0;
        playerMoveCompleted = false;

        qDebug() << "Game destructor completed successfully";

    }
    catch (const std::exception& e) {
        qDebug() << "Exception in Game destructor:" << e.what();
    }
    catch (...) {
        qDebug() << "Unknown exception in Game destructor";
    }
}

// Fixed showMainMenu method
void Game::showMainMenu() {
    qDebug() << "Returning to main menu...";

    // Close current game window if it exists
    if (currentGameWindow) {
        qDebug() << "Closing current game window";
        // Disconnect all signals to prevent issues during destruction
        currentGameWindow->disconnect();
        currentGameWindow->close();
        currentGameWindow = nullptr;
    }

    // Reset game state
    m_round_Counter = 0;
    player1RoundsWon = 0;
    player2RoundsWon = 0;
    playerMoveCompleted = false;

    // Use QTimer to create main window in next event loop cycle
    QTimer::singleShot(100, []() {
        MainWindow* mainWindow = new MainWindow(QDir::currentPath() + QDir::separator() + "eter.png");
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow->setObjectName("MainWindow");
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();
        qDebug() << "MainWindow created and shown";
        });
}

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
	return m_gameBoard; 
}

const Board& Game::getBoard() const {
	return m_gameBoard;
}





//void Game::startTraining() {
//    m_gameBoard = Board(1);
//    this->m_round_Counter = 1;
//    int16_t maxRounds = 3; // Best of 3 (primul care câștigă 2 runde)
//    int16_t roundsToWin = 2; // Trebuie 2 runde câștigate pentru a câștiga meciul
//    std::vector<SimpleCard> PastCards;
//    std::optional<std::pair<bool, bool>> canPlayIllusion;
//    int16_t player1RoundsWon = 0;
//    int16_t player2RoundsWon = 0;
//
//    if (m_illusionsEnabled) {
//        canPlayIllusion = std::make_pair(true, true);
//    }
//    else {
//        canPlayIllusion = std::nullopt;
//    }
//
//    auto* trainingWindow = new SecondaryWindow("Training", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), "", "", "", "", false, false);
//    trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
//    connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
//
//    // Simple connection - no complex lambda
//    connect(trainingWindow, &SecondaryWindow::returnToMainMenu, this, &Game::showMainMenu);
//
//    trainingWindow->show();
//
//    // BUCLA MECIULUI - Continuă până când cineva câștigă destule runde
//    while (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin && m_round_Counter <= maxRounds) {
//        qDebug() << "=== Începe Runda " << m_round_Counter << " ===";
//        qDebug() << "Scorul curent - Jucătorul 1:" << player1RoundsWon << ", Jucătorul 2:" << player2RoundsWon;
//
//        // Resetare pentru runda nouă
//        player1 = Player("Jucătorul 1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(4, Color::Red) }, PastCards);
//        player2 = Player("Jucătorul 2", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue) }, PastCards);
//
//        m_gameBoard.initializeForDynamicPlay(3);
//        currentPlayer = Color::Red;
//        playerMoveCompleted = false;
//
//        trainingWindow->setBoard(m_gameBoard, 3);
//        trainingWindow->setPlayer1Cards(player1.getVector());
//        trainingWindow->setPlayer2Cards(player2.getVector());
//        trainingWindow->setCurrentPlayer(currentPlayer);
//        trainingWindow->resetView();
//        trainingWindow->updateBoardView();
//
//        // Actualizează afișajul informațiilor despre meci
//        trainingWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);
//
//        // BUCLA RUNDEI - Joacă până când această rundă este decisă
//        bool roundInProgress = true;
//        QString roundWinnerName;
//
//        while (roundInProgress) {
//            QCoreApplication::processEvents();
//
//            if (playerMoveCompleted) {
//                // Verifică condițiile de câștig ÎNAINTE de schimbarea jucătorilor
//                Board::State winState = m_gameBoard.checkWin(false, 3);
//
//                if (winState == Board::State::RedWin) {
//                    qDebug() << "Jucătorul 1 (Roșu) câștigă runda " << m_round_Counter << "!";
//                    player1RoundsWon++;
//                    roundWinnerName = "Jucătorul 1";
//                    roundInProgress = false;
//                    break;
//                }
//                else if (winState == Board::State::BlueWin) {
//                    qDebug() << "Jucătorul 2 (Albastru) câștigă runda " << m_round_Counter << "!";
//                    player2RoundsWon++;
//                    roundWinnerName = "Jucătorul 2";
//                    roundInProgress = false;
//                    break;
//                }
//
//                // Schimbă jucătorii dacă runda continuă
//                if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
//                    trainingWindow->setCurrentPlayer(Color::Blue);
//                    currentPlayer = Color::Blue;
//                    qDebug() << "Rândul jucătorului 2.";
//                }
//                else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
//                    trainingWindow->setCurrentPlayer(Color::Red);
//                    currentPlayer = Color::Red;
//                    qDebug() << "Rândul jucătorului 1.";
//                }
//
//                playerMoveCompleted = false;
//            }
//
//            // Verifică dacă ambii jucători au rămas fără cărți (numărarea punctelor)
//            if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
//                qDebug() << "Ambii jucători au rămas fără cărți - se verifică punctele...";
//                auto state = m_gameBoard.checkWin(true, 3);
//
//                if (state == Board::State::RedWin) {
//                    qDebug() << "Jucătorul 1 câștigă runda " << m_round_Counter << " prin puncte.";
//                    player1RoundsWon++;
//                    roundWinnerName = "Jucătorul 1";
//                }
//                else if (state == Board::State::BlueWin) {
//                    qDebug() << "Jucătorul 2 câștigă runda " << m_round_Counter << " prin puncte.";
//                    player2RoundsWon++;
//                    roundWinnerName = "Jucătorul 2";
//                }
//                else if (state == Board::State::Draw) {
//                    qDebug() << "Runda " << m_round_Counter << " este egalitate.";
//                    roundWinnerName = "Egalitate";
//                    // În caz de egalitate, niciun jucător nu primește punct
//                }
//
//                roundInProgress = false;
//            }
//        }
//
//        // Afișează câștigătorul rundei dacă există unul
//        if (!roundWinnerName.isEmpty() && roundWinnerName != "Egalitate") {
//            trainingWindow->showRoundWinner(roundWinnerName, m_round_Counter);
//        }
//        else if (roundWinnerName == "Egalitate") {
//            QMessageBox::information(trainingWindow, "Rezultatul rundei",
//                QString("Runda %1 s-a terminat la egalitate!").arg(m_round_Counter));
//        }
//
//        incrementRoundCounter();
//
//        // Verifică dacă meciul a fost câștigat
//        if (player1RoundsWon >= roundsToWin) {
//            trainingWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
//            return; // Simply return from the method
//        }
//        else if (player2RoundsWon >= roundsToWin) {
//            trainingWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
//            return; // Simply return from the method
//        }
//        else if (m_round_Counter > maxRounds) {
//            // S-a atins numărul maxim de runde, determină câștigătorul prin rundele câștigate
//            if (player1RoundsWon > player2RoundsWon) {
//                trainingWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
//            }
//            else if (player2RoundsWon > player1RoundsWon) {
//                trainingWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
//            }
//            else {
//                trainingWindow->showWinner("Meciul s-a terminat la egalitate!");
//            }
//            return; // Simply return from the method
//        }
//
//        // Pauză scurtă între runde dacă meciul continuă
//        if (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin) {
//            QEventLoop loop;
//            QTimer::singleShot(2000, &loop, &QEventLoop::quit);
//            loop.exec();
//        }
//    }
//
//    qDebug() << "Training method finished normally";
//}


void Game::startTraining() {
    qDebug() << "=== DEBUG: startTraining called ===";

    try {
        qDebug() << "DEBUG: Setting up game state...";
        m_round_Counter = 1;
        player1RoundsWon = 0;
        player2RoundsWon = 0;
        roundsToWin = 2;
        maxRounds = 3;

        qDebug() << "DEBUG: Creating SecondaryWindow...";
        currentGameWindow = new SecondaryWindow("Training",
            QDir::currentPath() + QDir::separator() + "eter.png",
            &Game::get_Instance(), "", "", "", "", false, false);

        if (!currentGameWindow) {
            qDebug() << "ERROR: Failed to create currentGameWindow!";
            return;
        }

        qDebug() << "DEBUG: Setting window attributes...";
        currentGameWindow->setAttribute(Qt::WA_DeleteOnClose);

        qDebug() << "DEBUG: Connecting signals...";
        connect(currentGameWindow, &SecondaryWindow::boardClicked,
            this, &Game::handleBoardClick);
        connect(currentGameWindow, &SecondaryWindow::returnToMainMenu,
            this, &Game::showMainMenu);
        connect(currentGameWindow, &QObject::destroyed, this, [this]() {
            qDebug() << "SecondaryWindow destroyed, clearing pointer";
            currentGameWindow = nullptr;
            });

        qDebug() << "DEBUG: Showing window...";
        currentGameWindow->show();

        qDebug() << "DEBUG: Starting new round...";
        startNewRound();

        qDebug() << "=== DEBUG: startTraining completed successfully ===";

    }
    catch (const std::exception& e) {
        qDebug() << "EXCEPTION in startTraining:" << e.what();
    }
    catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in startTraining";
    }
}

void Game::startNewRound() {
    if (!currentGameWindow) return;

    qDebug() << "=== Starting Round " << m_round_Counter << " ===";
    qDebug() << "Score - Player 1:" << player1RoundsWon << ", Player 2:" << player2RoundsWon;

    // Reset pentru runda nouă
    std::vector<SimpleCard> PastCards;
    player1 = Player("Jucătorul 1", {
        SimpleCard(1, Color::Red), SimpleCard(1, Color::Red),
        SimpleCard(2, Color::Red), SimpleCard(2, Color::Red),
        SimpleCard(3, Color::Red), SimpleCard(3, Color::Red), SimpleCard(4, Color::Red)
        }, PastCards);

    player2 = Player("Jucătorul 2", {
        SimpleCard(1, Color::Blue), SimpleCard(1, Color::Blue),
        SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue),
        SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue), SimpleCard(4, Color::Blue)
        }, PastCards);

    m_gameBoard.initializeForDynamicPlay(3);
    currentPlayer = Color::Red;
    playerMoveCompleted = false;

    currentGameWindow->setBoard(m_gameBoard, 3);
    currentGameWindow->setPlayer1Cards(player1.getVector());
    currentGameWindow->setPlayer2Cards(player2.getVector());
    currentGameWindow->setCurrentPlayer(currentPlayer);
    currentGameWindow->resetView();
    currentGameWindow->updateBoardView();
    currentGameWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);
}

void Game::checkRoundEnd() {
    if (!currentGameWindow) return;

    // Check win conditions
    Board::State winState = m_gameBoard.checkWin(false, 3);
    QString roundWinnerName;

    if (winState == Board::State::RedWin) {
        player1RoundsWon++;
        roundWinnerName = "Jucătorul 1";
    }
    else if (winState == Board::State::BlueWin) {
        player2RoundsWon++;
        roundWinnerName = "Jucătorul 2";
    }
    else if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
        // Count points if both players are out of cards
        auto state = m_gameBoard.checkWin(true, 3);
        if (state == Board::State::RedWin) {
            player1RoundsWon++;
            roundWinnerName = "Jucătorul 1";
        }
        else if (state == Board::State::BlueWin) {
            player2RoundsWon++;
            roundWinnerName = "Jucătorul 2";
        }
        else {
            roundWinnerName = "Egalitate";
        }
    }

    // If round ended
    if (!roundWinnerName.isEmpty()) {
        if (roundWinnerName != "Egalitate") {
            currentGameWindow->showRoundWinner(roundWinnerName, m_round_Counter);
        }

        incrementRoundCounter();

        // Check if match ended
        if (player1RoundsWon >= roundsToWin) {
            currentGameWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            return;
        }
        else if (player2RoundsWon >= roundsToWin) {
            currentGameWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            return;
        }
        else if (m_round_Counter > maxRounds) {
            if (player1RoundsWon > player2RoundsWon) {
                currentGameWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            }
            else if (player2RoundsWon > player1RoundsWon) {
                currentGameWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            }
            else {
                currentGameWindow->showWinner("Meciul s-a terminat la egalitate!");
            }
            return;
        }

        // Start next round after delay
        QTimer::singleShot(2000, this, &Game::startNewRound);
    }
}

// Fixed handleBoardClick - simplified version
// În Game.cpp - Versiunea corectată a handleBoardClick:

// În Game.cpp - modifică funcția handleBoardClick pentru extinderea asimetrică:

// În Game.cpp - modifică funcția handleBoardClick pentru extinderea asimetrică:

void Game::handleBoardClick(int row, int col, int player) {
    qDebug() << "Processing move at (" << row << ", " << col << ") for player "
        << (currentPlayer == Color::Red ? "1 (Red)" : "2 (Blue)");

    if (!currentGameWindow) {
        qDebug() << "No active game window!";
        return;
    }

    if (!m_gameBoard.canBePlaced(row, col)) {
        qDebug() << "Position is not valid for placement.";
        return;
    }

    Player& currentPlayerRef = (currentPlayer == Color::Red) ? player1 : player2;

    if (currentPlayerRef.numberofValidCards() <= 0) {
        qDebug() << "Current player has no more cards!";
        checkRoundEnd();
        return;
    }

    // Check if there's a selected card
    SimpleCard cardToPlay;
    bool foundCard = false;

    if (hasSelectedCard()) {
        for (const auto& card : currentPlayerRef.getVector()) {
            if (card.getValue() == selectedCard.getValue() &&
                card.getColor() == selectedCard.getColor() &&
                (card.getColor() == Color::Red || card.getColor() == Color::Blue)) {
                cardToPlay = card;
                foundCard = true;
                qDebug() << "Using selected card: Value=" << cardToPlay.getValue()
                    << ", Color=" << (cardToPlay.getColor() == Color::Red ? "Red" : "Blue");
                break;
            }
        }

        if (!foundCard) {
            qDebug() << "Selected card is not valid for current player!";
            return;
        }
    }
    else {
        qDebug() << "No card selected! Player must select a card first.";
        return;
    }

    // Check that the card can be placed
    if (!m_gameBoard.canBePushed(cardToPlay, { row, col })) {
        qDebug() << "Card cannot be pushed to this position - value too low";
        return;
    }

    qDebug() << "Board size before placement: "
        << m_gameBoard.getRowSize() << "x" << m_gameBoard.getColumnSize();

    // Place the card
    m_gameBoard.pushCard(cardToPlay, { row, col });
    qDebug() << "Card placed: Player" << (currentPlayer == Color::Red ? "1" : "2")
        << " Value:" << cardToPlay.getValue();

    // Clear selection IMMEDIATELY after placing the card
    clearSelectedCard();
    if (currentGameWindow) {
        currentGameWindow->clearCardSelection();
    }

    // NEW SMART BOARD MANAGEMENT LOGIC
    qDebug() << "Applying smart board management...";
    m_gameBoard.smartBoardManagement(4); // Maximum 4x4 for training

    qDebug() << "Board size after smart management: "
        << m_gameBoard.getRowSize() << "x" << m_gameBoard.getColumnSize();

    // Mark card as used
    currentPlayerRef.makeCardInvalid(cardToPlay);
    currentPlayerRef.getPastVector().push_back(cardToPlay);

    // Update interface - NOW with selection already reset
    currentGameWindow->setPlayer1Cards(player1.getVector());
    currentGameWindow->setPlayer2Cards(player2.getVector());
    currentGameWindow->updateBoardView();

    // Debug info
    m_gameBoard.print();
    auto [finalRows, finalCols] = m_gameBoard.getActualBoardBounds();
    qDebug() << "Move completed. Board size: "
        << m_gameBoard.getRowSize() << "x" << m_gameBoard.getColumnSize()
        << " (actual occupied: " << finalRows << "x" << finalCols << ")";

    // Check end of round
    Board::State winState = m_gameBoard.checkWin(false, 3);
    if (winState != Board::State::None) {
        checkRoundEnd();
        return;
    }

    // Check if both players have cards left
    if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
        checkRoundEnd();
        return;
    }

    // Switch player
    currentPlayer = (currentPlayer == Color::Red) ? Color::Blue : Color::Red;
    currentGameWindow->setCurrentPlayer(currentPlayer);

    qDebug() << "Switched to player " << (currentPlayer == Color::Red ? "1 (Red)" : "2 (Blue)");
}

// Elimină funcția cleanupEmptyBorders veche, deoarece acum este integrată în asymmetricExpansion

// Elimină funcția cleanupEmptyBorders veche, deoarece acum este integrată în asymmetricExpansion

void Game::cleanupEmptyBorders() {
    qDebug() << "Cleaning up borders...";

    // Eliminăm rândurile goale de la margini
    while (m_gameBoard.getRowSize() > 3) {
        if (m_gameBoard.isFirstRowEmpty() && !m_gameBoard.isLastRowEmpty()) {
            m_gameBoard.removeRow(0);
            qDebug() << "Removed first row";
        }
        else if (m_gameBoard.isLastRowEmpty() && !m_gameBoard.isFirstRowEmpty()) {
            m_gameBoard.removeRow(m_gameBoard.getRowSize() - 1);
            qDebug() << "Removed last row";
        }
        else {
            break;
        }
    }

    // Eliminăm coloanele goale de la margini
    while (m_gameBoard.getColumnSize() > 3) {
        if (m_gameBoard.isFirstColumnEmpty() && !m_gameBoard.isLastColumnEmpty()) {
            m_gameBoard.removeColumn(0);
            qDebug() << "Removed first column";
        }
        else if (m_gameBoard.isLastColumnEmpty() && !m_gameBoard.isFirstColumnEmpty()) {
            m_gameBoard.removeColumn(m_gameBoard.getColumnSize() - 1);
            qDebug() << "Removed last column";
        }
        else {
            break;
        }
    }

    qDebug() << "Cleanup finished. Final size: "
        << m_gameBoard.getRowSize() << "x" << m_gameBoard.getColumnSize();
}

void Game::startMageDuel() {
    m_gameBoard = Board(1);
    this->m_round_Counter = 1;
    int16_t maxRounds = 5; // Best of 5 (primul care câștigă 3 runde)
    int16_t roundsToWin = 3; // Trebuie 3 runde câștigate pentru a câștiga meciul
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

    // Atribuie vrăjitorii o singură dată pentru întregul meci
    player1 = Player("Jucătorul 1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(3, Color::Red),SimpleCard(4, Color::Red),SimpleCard(5, Color::Red) }, PastCards, true);
    player2 = Player("Jucătorul 2", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue),SimpleCard(5, Color::Blue) }, PastCards, true);

    auto mage1 = player1.getMageAssignment();
    auto mage2 = player2.getMageAssignment();
    while ((mage1 == mage2) || (mage1 % 2 == 0 && mage2 == mage1 + 1) || (mage1 % 2 == 1 && mage2 == mage1 - 1)) {
        player2.reasignMage();
        mage2 = player2.getMageAssignment();
    }

    auto* mageWindow = new SecondaryWindow("Mage Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), QString::fromStdString(player1.getMage()), QString::fromStdString(player2.getMage()), "", "", true, false);
    mageWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(mageWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
    connect(mageWindow, &SecondaryWindow::returnToMainMenu, this, &Game::showMainMenu);

    mageWindow->show();

    qDebug() << "Vrăjitorul Jucătorului 1:" << player1.getMage();
    qDebug() << "Vrăjitorul Jucătorului 2:" << player2.getMage();

    // BUCLA MECIULUI - Continuă până când cineva câștigă destule runde
    while (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin && m_round_Counter <= maxRounds) {
        qDebug() << "=== Începe Runda " << m_round_Counter << " ===";
        qDebug() << "Scorul curent - Jucătorul 1:" << player1RoundsWon << ", Jucătorul 2:" << player2RoundsWon;

        // Resetează cărțile pentru runda nouă (dar păstrează vrăjitorii)
        player1.ResetVector();
        player2.ResetVector();

        m_gameBoard.initializeForDynamicPlay(4);
        currentPlayer = Color::Red;
        playerMoveCompleted = false;

        mageWindow->setBoard(m_gameBoard, 4);
        mageWindow->setPlayer1Cards(player1.getVector());
        mageWindow->setPlayer2Cards(player2.getVector());
        mageWindow->setCurrentPlayer(currentPlayer);
        mageWindow->resetView();
        mageWindow->updateBoardView();

        // Actualizează afișajul informațiilor despre meci
        mageWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);

        // BUCLA RUNDEI - Joacă până când această rundă este decisă
        bool roundInProgress = true;
        QString roundWinnerName;

        while (roundInProgress) {
            QCoreApplication::processEvents();

            if (playerMoveCompleted) {
                // Verifică condițiile de câștig ÎNAINTE de schimbarea jucătorilor
                Board::State winState = m_gameBoard.checkWin(false, 4);

                if (winState == Board::State::RedWin) {
                    qDebug() << "Jucătorul 1 (Roșu) câștigă runda " << m_round_Counter << "!";
                    player1RoundsWon++;
                    roundWinnerName = "Jucătorul 1";
                    roundInProgress = false;
                    break;
                }
                else if (winState == Board::State::BlueWin) {
                    qDebug() << "Jucătorul 2 (Albastru) câștigă runda " << m_round_Counter << "!";
                    player2RoundsWon++;
                    roundWinnerName = "Jucătorul 2";
                    roundInProgress = false;
                    break;
                }

                // Schimbă jucătorii dacă runda continuă
                if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
                    mageWindow->setCurrentPlayer(Color::Blue);
                    currentPlayer = Color::Blue;
                    qDebug() << "Rândul jucătorului 2.";
                }
                else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
                    mageWindow->setCurrentPlayer(Color::Red);
                    currentPlayer = Color::Red;
                    qDebug() << "Rândul jucătorului 1.";
                }

                playerMoveCompleted = false;
            }

            // Verifică dacă ambii jucători au rămas fără cărți (numărarea punctelor)
            if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
                qDebug() << "Ambii jucători au rămas fără cărți - se verifică punctele...";
                auto state = m_gameBoard.checkWin(true, 4);

                if (state == Board::State::RedWin) {
                    qDebug() << "Jucătorul 1 câștigă runda " << m_round_Counter << " prin puncte.";
                    player1RoundsWon++;
                    roundWinnerName = "Jucătorul 1";
                }
                else if (state == Board::State::BlueWin) {
                    qDebug() << "Jucătorul 2 câștigă runda " << m_round_Counter << " prin puncte.";
                    player2RoundsWon++;
                    roundWinnerName = "Jucătorul 2";
                }
                else if (state == Board::State::Draw) {
                    qDebug() << "Runda " << m_round_Counter << " este egalitate.";
                    roundWinnerName = "Egalitate";
                }

                roundInProgress = false;
            }
        }

        // Afișează câștigătorul rundei dacă există unul
        if (!roundWinnerName.isEmpty() && roundWinnerName != "Egalitate") {
            mageWindow->showRoundWinner(roundWinnerName, m_round_Counter);
        }
        else if (roundWinnerName == "Egalitate") {
            QMessageBox::information(mageWindow, "Rezultatul rundei",
                QString("Runda %1 s-a terminat la egalitate!").arg(m_round_Counter));
        }

        incrementRoundCounter();

        // Verifică dacă meciul a fost câștigat
        if (player1RoundsWon >= roundsToWin) {
            mageWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            break;
        }
        else if (player2RoundsWon >= roundsToWin) {
            mageWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            break;
        }
        else if (m_round_Counter > maxRounds) {
            // S-a atins numărul maxim de runde, determină câștigătorul prin rundele câștigate
            if (player1RoundsWon > player2RoundsWon) {
                mageWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            }
            else if (player2RoundsWon > player1RoundsWon) {
                mageWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            }
            else {
                mageWindow->showWinner("Meciul s-a terminat la egalitate!");
            }
            break;
        }

        // Pauză scurtă între runde dacă meciul continuă
        if (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin) {
            QEventLoop loop;
            QTimer::singleShot(2000, &loop, &QEventLoop::quit);
            loop.exec();
        }
    }
}

void Game::startPowerDuel() {
    m_gameBoard = Board(1);
    this->m_round_Counter = 1;
    int16_t maxRounds = 5; // Best of 5 (primul care câștigă 3 runde)
    int16_t roundsToWin = 3; // Trebuie 3 runde câștigate pentru a câștiga meciul
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

    // Atribuie puterile o singură dată pentru întregul meci
    player1 = Player("Jucătorul 1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(3, Color::Red),SimpleCard(4, Color::Red),SimpleCard(5, Color::Red) }, PastCards, true);
    player2 = Player("Jucătorul 2", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue),SimpleCard(5, Color::Blue) }, PastCards, true);

    player1.assignPower();
    player2.assignPower();

    while (player1.getPower() == player2.getPower()) {
        player2.reassignPower();
    }

    auto* powerWindow = new SecondaryWindow("Power Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), "", "", QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString()), QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString()), false, true);
    powerWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(powerWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
    connect(powerWindow, &SecondaryWindow::returnToMainMenu, this, &Game::showMainMenu);

    powerWindow->show();

    qDebug() << "Puterea Jucătorului 1:" << fromPowerToQString(player1.getPower());
    qDebug() << "Puterea Jucătorului 2:" << fromPowerToQString(player2.getPower());

    // BUCLA MECIULUI - Continuă până când cineva câștigă destule runde
    while (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin && m_round_Counter <= maxRounds) {
        qDebug() << "=== Începe Runda " << m_round_Counter << " ===";
        qDebug() << "Scorul curent - Jucătorul 1:" << player1RoundsWon << ", Jucătorul 2:" << player2RoundsWon;

        // Resetează cărțile pentru runda nouă (dar păstrează puterile)
        player1.ResetVector();
        player2.ResetVector();

        m_gameBoard.initializeForDynamicPlay(4);
        currentPlayer = Color::Red;
        playerMoveCompleted = false;

        powerWindow->setBoard(m_gameBoard, 4);
        powerWindow->setPlayer1Cards(player1.getVector());
        powerWindow->setPlayer2Cards(player2.getVector());
        powerWindow->setCurrentPlayer(currentPlayer);
        powerWindow->resetView();
        powerWindow->updateBoardView();

        // Actualizează afișajul informațiilor despre meci
        powerWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);

        // BUCLA RUNDEI - Joacă până când această rundă este decisă
        bool roundInProgress = true;
        QString roundWinnerName;

        while (roundInProgress) {
            QCoreApplication::processEvents();

            if (playerMoveCompleted) {
                // Verifică condițiile de câștig ÎNAINTE de schimbarea jucătorilor
                Board::State winState = m_gameBoard.checkWin(false, 4);

                if (winState == Board::State::RedWin) {
                    qDebug() << "Jucătorul 1 (Roșu) câștigă runda " << m_round_Counter << "!";
                    player1RoundsWon++;
                    roundWinnerName = "Jucătorul 1";
                    roundInProgress = false;
                    break;
                }
                else if (winState == Board::State::BlueWin) {
                    qDebug() << "Jucătorul 2 (Albastru) câștigă runda " << m_round_Counter << "!";
                    player2RoundsWon++;
                    roundWinnerName = "Jucătorul 2";
                    roundInProgress = false;
                    break;
                }

                // Schimbă jucătorii dacă runda continuă
                if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
                    powerWindow->setCurrentPlayer(Color::Blue);
                    currentPlayer = Color::Blue;
                    qDebug() << "Rândul jucătorului 2.";
                }
                else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
                    powerWindow->setCurrentPlayer(Color::Red);
                    currentPlayer = Color::Red;
                    qDebug() << "Rândul jucătorului 1.";
                }

                playerMoveCompleted = false;
            }

            // Verifică dacă ambii jucători au rămas fără cărți (numărarea punctelor)
            if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
                qDebug() << "Ambii jucători au rămas fără cărți - se verifică punctele...";
                auto state = m_gameBoard.checkWin(true, 4);

                if (state == Board::State::RedWin) {
                    qDebug() << "Jucătorul 1 câștigă runda " << m_round_Counter << " prin puncte.";
                    player1RoundsWon++;
                    roundWinnerName = "Jucătorul 1";
                }
                else if (state == Board::State::BlueWin) {
                    qDebug() << "Jucătorul 2 câștigă runda " << m_round_Counter << " prin puncte.";
                    player2RoundsWon++;
                    roundWinnerName = "Jucătorul 2";
                }
                else if (state == Board::State::Draw) {
                    qDebug() << "Runda " << m_round_Counter << " este egalitate.";
                    roundWinnerName = "Egalitate";
                }

                roundInProgress = false;
            }
        }

        // Afișează câștigătorul rundei dacă există unul
        if (!roundWinnerName.isEmpty() && roundWinnerName != "Egalitate") {
            powerWindow->showRoundWinner(roundWinnerName, m_round_Counter);
        }
        else if (roundWinnerName == "Egalitate") {
            QMessageBox::information(powerWindow, "Rezultatul rundei",
                QString("Runda %1 s-a terminat la egalitate!").arg(m_round_Counter));
        }

        incrementRoundCounter();

        // Verifică dacă meciul a fost câștigat
        if (player1RoundsWon >= roundsToWin) {
            powerWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            break;
        }
        else if (player2RoundsWon >= roundsToWin) {
            powerWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            break;
        }
        else if (m_round_Counter > maxRounds) {
            // S-a atins numărul maxim de runde, determină câștigătorul prin rundele câștigate
            if (player1RoundsWon > player2RoundsWon) {
                powerWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            }
            else if (player2RoundsWon > player1RoundsWon) {
                powerWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            }
            else {
                powerWindow->showWinner("Meciul s-a terminat la egalitate!");
            }
            break;
        }

        // Pauză scurtă între runde dacă meciul continuă
        if (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin) {
            QEventLoop loop;
            QTimer::singleShot(2000, &loop, &QEventLoop::quit);
            loop.exec();
        }
    }
}

void Game::startTournament()
{
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;

}
void Game::startMageDuelAndPower() {
    m_gameBoard = Board(1);
    this->m_round_Counter = 1;
    int16_t maxRounds = 3; // Best of 3 (primul care câștigă 2 runde)
    int16_t roundsToWin = 2; // Trebuie 2 runde câștigate pentru a câștiga meciul
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

    // Atribuie atât vrăjitorii cât și puterile o singură dată pentru întregul meci
    player1 = Player("Jucătorul 1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(3, Color::Red),SimpleCard(4, Color::Red),SimpleCard(5, Color::Red) }, PastCards, true);
    player2 = Player("Jucătorul 2", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue),SimpleCard(5, Color::Blue) }, PastCards, true);

    // Atribuie vrăjitorii distincți
    auto mage1 = player1.getMageAssignment();
    auto mage2 = player2.getMageAssignment();
    while ((mage1 == mage2) || (mage1 % 2 == 0 && mage2 == mage1 + 1) || (mage1 % 2 == 1 && mage2 == mage1 - 1)) {
        player2.reasignMage();
        mage2 = player2.getMageAssignment();
    }

    // Atribuie puteri distincte
    player1.assignPower();
    player2.assignPower();

    while (player1.getPower() == player2.getPower()) {
        player2.reassignPower();
    }

    auto* combinedWindow = new SecondaryWindow("Power & Mage Duel", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), QString::fromStdString(player1.getMage()), QString::fromStdString(player2.getMage()), QString::fromStdString(fromPowerToQString(player1.getPower()).toStdString()), QString::fromStdString(fromPowerToQString(player2.getPower()).toStdString()), true, true);
    combinedWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(combinedWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
    connect(combinedWindow, &SecondaryWindow::returnToMainMenu, this, &Game::showMainMenu);

    combinedWindow->show();

    qDebug() << "Vrăjitorul Jucătorului 1:" << player1.getMage();
    qDebug() << "Vrăjitorul Jucătorului 2:" << player2.getMage();
    qDebug() << "Puterea Jucătorului 1:" << fromPowerToQString(player1.getPower());
    qDebug() << "Puterea Jucătorului 2:" << fromPowerToQString(player2.getPower());

    // BUCLA MECIULUI - Continuă până când cineva câștigă destule runde
    while (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin && m_round_Counter <= maxRounds) {
        qDebug() << "=== Începe Runda " << m_round_Counter << " ===";
        qDebug() << "Scorul curent - Jucătorul 1:" << player1RoundsWon << ", Jucătorul 2:" << player2RoundsWon;

        // Resetează cărțile pentru runda nouă (dar păstrează vrăjitorii și puterile)
        player1.ResetVector();
        player2.ResetVector();

        m_gameBoard.initializeForDynamicPlay(4);
        currentPlayer = Color::Red;
        playerMoveCompleted = false;

        combinedWindow->setBoard(m_gameBoard, 4);
        combinedWindow->setPlayer1Cards(player1.getVector());
        combinedWindow->setPlayer2Cards(player2.getVector());
        combinedWindow->setCurrentPlayer(currentPlayer);
        combinedWindow->resetView();
        combinedWindow->updateBoardView();

        // Actualizează afișajul informațiilor despre meci
        combinedWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);

        // BUCLA RUNDEI - Joacă până când această rundă este decisă
        bool roundInProgress = true;
        QString roundWinnerName;

        while (roundInProgress) {
            QCoreApplication::processEvents();

            if (playerMoveCompleted) {
                // Verifică condițiile de câștig ÎNAINTE de schimbarea jucătorilor
                Board::State winState = m_gameBoard.checkWin(false, 4);

                if (winState == Board::State::RedWin) {
                    qDebug() << "Jucătorul 1 (Roșu) câștigă runda " << m_round_Counter << "!";
                    player1RoundsWon++;
                    roundWinnerName = "Jucătorul 1";
                    roundInProgress = false;
                    break;
                }
                else if (winState == Board::State::BlueWin) {
                    qDebug() << "Jucătorul 2 (Albastru) câștigă runda " << m_round_Counter << "!";
                    player2RoundsWon++;
                    roundWinnerName = "Jucătorul 2";
                    roundInProgress = false;
                    break;
                }

                // Schimbă jucătorii dacă runda continuă
                if (currentPlayer == Color::Red && player1.numberofValidCards() > 0) {
                    combinedWindow->setCurrentPlayer(Color::Blue);
                    currentPlayer = Color::Blue;
                    qDebug() << "Rândul jucătorului 2.";
                }
                else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
                    combinedWindow->setCurrentPlayer(Color::Red);
                    currentPlayer = Color::Red;
                    qDebug() << "Rândul jucătorului 1.";
                }

                playerMoveCompleted = false;
            }

            // Verifică dacă ambii jucători au rămas fără cărți (numărarea punctelor)
            if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
                qDebug() << "Ambii jucători au rămas fără cărți - se verifică punctele...";
                auto state = m_gameBoard.checkWin(true, 4);

                if (state == Board::State::RedWin) {
                    qDebug() << "Jucătorul 1 câștigă runda " << m_round_Counter << " prin puncte.";
                    player1RoundsWon++;
                    roundWinnerName = "Jucătorul 1";
                }
                else if (state == Board::State::BlueWin) {
                    qDebug() << "Jucătorul 2 câștigă runda " << m_round_Counter << " prin puncte.";
                    player2RoundsWon++;
                    roundWinnerName = "Jucătorul 2";
                }
                else if (state == Board::State::Draw) {
                    qDebug() << "Runda " << m_round_Counter << " este egalitate.";
                    roundWinnerName = "Egalitate";
                }

                roundInProgress = false;
            }
        }

        // Afișează câștigătorul rundei dacă există unul
        if (!roundWinnerName.isEmpty() && roundWinnerName != "Egalitate") {
            combinedWindow->showRoundWinner(roundWinnerName, m_round_Counter);
        }
        else if (roundWinnerName == "Egalitate") {
            QMessageBox::information(combinedWindow, "Rezultatul rundei",
                QString("Runda %1 s-a terminat la egalitate!").arg(m_round_Counter));
        }

        incrementRoundCounter();

        // Verifică dacă meciul a fost câștigat
        if (player1RoundsWon >= roundsToWin) {
            combinedWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            break;
        }
        else if (player2RoundsWon >= roundsToWin) {
            combinedWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            break;
        }
        else if (m_round_Counter > maxRounds) {
            // S-a atins numărul maxim de runde, determină câștigătorul prin rundele câștigate
            if (player1RoundsWon > player2RoundsWon) {
                combinedWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            }
            else if (player2RoundsWon > player1RoundsWon) {
                combinedWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            }
            else {
                combinedWindow->showWinner("Meciul s-a terminat la egalitate!");
            }
            break;
        }

        // Pauză scurtă între runde dacă meciul continuă
        if (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin) {
            QEventLoop loop;
            QTimer::singleShot(2000, &loop, &QEventLoop::quit);
            loop.exec();
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


//void Game::handleBoardClick(int row, int col) {
//	qDebug() << "Clicked on board at (" << row << ", " << col << ")";
//
//	if (!m_gameBoard.canBePlaced(row, col)) {
//		qDebug() << "Position is not valid for placement.";
//		return;
//	}
//
//	if (currentPlayer == Color::Red) {
//		SimpleCard selectedCard = player1.chooseCard();
//		m_gameBoard.pushCard(selectedCard, { row, col });
//		qDebug() << "Player 1 placed card at (" << row << ", " << col << ")";
//		currentPlayer = Color::Blue; 
//	}
//	else if (currentPlayer == Color::Blue) {
//		SimpleCard selectedCard = player2.chooseCard();
//		m_gameBoard.pushCard(selectedCard, { row, col });
//		qDebug() << "Player 2 placed card at (" << row << ", " << col << ")";
//		currentPlayer = Color::Red;
//	}
//
//	playerMoveCompleted = true; 
//}


