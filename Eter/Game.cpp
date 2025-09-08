#include "Game.h"
// IMPORTANT: Include complete definition BEFORE any smart pointer usage
#include "MainWindow.h"
#include <QTimer>
Game Game::m_current_Instance;

// În Game.cpp - Înlocuiește destructorul cu această versiune mai sigură:

Game::~Game() {
    qDebug() << "Game destructor called - starting cleanup";

    try {
        if (currentGameWindow) {
            qDebug() << "Cleaning up currentGameWindow";

            // Disconnect all signals to prevent issues
            currentGameWindow->disconnect();

            // Close the window gracefully
            currentGameWindow->close();

            // Set parent to nullptr to prevent Qt from managing it
            currentGameWindow->setParent(nullptr);

            // Now safe to reset the smart pointer
            currentGameWindow.reset();

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

Game& Game::get_Instance()
{
	return m_current_Instance;
}

Game::GameType Game::stringToGameType(std::string_view word)
{
    if (word == "Training")
        return GameType::Training;
    else if (word == "MageDuel")
        return GameType::MageDuel;
    else if (word == "Power")
        return GameType::Power;
    else if (word == "Tournament")
        return GameType::Tournament;
    else if (word == "MageDuelAndPower")
        return GameType::MageDuelAndPower;

    // Add default case to avoid warning
    return GameType::Training;
}

std::string_view Game::gameTypeToString(GameType gameType) const
{
    if (gameType == GameType::Training)
        return "Training";
    else if (gameType == GameType::MageDuel)
        return "MageDuel";
    else if (gameType == GameType::Power)
        return "Power";
    else if (gameType == GameType::Tournament)
        return "Tournament";
    else if (gameType == GameType::MageDuelAndPower)
        return "MageDuelAndPower";

    // Add default case
    return "Training";
}

Board& Game::getBoard() {
	return m_gameBoard; 
}

const Board& Game::getBoard() const {
	return m_gameBoard;
}

void Game::showMainMenu() {
    qDebug() << "Returning to main menu...";

    // Prevent multiple calls
    static bool returningToMenu = false;
    if (returningToMenu) {
        qDebug() << "Already returning to menu, ignoring duplicate call";
        return;
    }
    returningToMenu = true;

    if (currentGameWindow) {
        qDebug() << "Closing current game window";

        // Disconnect signals first to prevent cascading calls
        currentGameWindow->disconnect();

        // Hide the window immediately to prevent flash
        currentGameWindow->hide();

        // Close the window
        currentGameWindow->close();

        // Remove from Qt's parent-child system
        currentGameWindow->setParent(nullptr);

        // Reset the smart pointer - custom deleter will handle it safely
        currentGameWindow.reset();
    }

    // Reset game state
    m_round_Counter = 0;
    player1RoundsWon = 0;
    player2RoundsWon = 0;
    playerMoveCompleted = false;

    // Create main window using Qt's standard approach with proper preparation
    QTimer::singleShot(100, [this]() {
        // Reset the flag before creating the window
        static bool returningToMenu = false;
        returningToMenu = false;

        // Create MainWindow but prepare it fully before showing
        MainWindow* mainWindow = new MainWindow(QDir::currentPath() + QDir::separator() + "eter.png");
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow->setObjectName("MainWindow");

        // Prepare the window completely to prevent flashing
        mainWindow->adjustSize();
        mainWindow->setAttribute(Qt::WA_DontShowOnScreen, false);

        // Show everything at once
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();

        qDebug() << "MainWindow created and shown";
        });
}

void Game::startTraining() {
    qDebug() << "=== DEBUG: startTraining called ===";

    try {
        m_round_Counter = 1;
        player1RoundsWon = 0;
        player2RoundsWon = 0;
        roundsToWin = 2;
        maxRounds = 3;

        // Create window without parent - smart pointer will manage it
        auto* window = new SecondaryWindow("Training",
            QDir::currentPath() + QDir::separator() + "eter.png",
            &Game::get_Instance(), "", "", "", "", false, false);

        // Hide window initially to prevent flash during setup
        window->hide();

        // Transfer ownership to smart pointer with custom deleter
        currentGameWindow.reset(window);

        if (!currentGameWindow) {
            qDebug() << "ERROR: Failed to create currentGameWindow!";
            return;
        }

        // Setup connections while window is hidden
        connect(currentGameWindow.get(), &SecondaryWindow::boardClicked,
            this, &Game::handleBoardClick);
        connect(currentGameWindow.get(), &SecondaryWindow::returnToMainMenu,
            this, &Game::showMainMenu);

        // Prepare the game state
        startNewRound();

        // Show the window only after everything is ready
        currentGameWindow->show();

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

    bool hasAnyCards = m_gameBoard.hasAnyCardModern();
    auto [redCount, blueCount] = m_gameBoard.getCardCountByColorModern();
    auto totalCards = m_gameBoard.getTotalCardsModern();
    auto emptyPositions = m_gameBoard.getEmptyPositionsModern();

    currentGameWindow->setBoard(m_gameBoard, 3);
    currentGameWindow->setPlayer1Cards(player1.getVector());
    currentGameWindow->setPlayer2Cards(player2.getVector());
    currentGameWindow->setCurrentPlayer(currentPlayer);
    currentGameWindow->resetView();
    currentGameWindow->updateBoardView();
    currentGameWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);

    // Force UI update
    currentGameWindow->update();
    currentGameWindow->repaint();
}
void Game::checkRoundEnd() {
    if (!currentGameWindow) return;

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
        // FOLOSEȘTE direct funcțiile moderne pentru calculul punctelor
        int16_t redSum = m_gameBoard.sumPointsModern(Color::Red);
        int16_t blueSum = m_gameBoard.sumPointsModern(Color::Blue);

        if (redSum > blueSum) {
            player1RoundsWon++;
            roundWinnerName = "Jucătorul 1";
        }
        else if (blueSum > redSum) {
            player2RoundsWon++;
            roundWinnerName = "Jucátorul 2";
        }
        else {
            roundWinnerName = "Egalitate";
        }
    }

    if (!roundWinnerName.isEmpty()) {
        if (roundWinnerName != "Egalitate") {
            currentGameWindow->showRoundWinner(roundWinnerName, m_round_Counter);
        }

        incrementRoundCounter();

        if (player1RoundsWon >= roundsToWin) {
            currentGameWindow->showWinner("Jucátorul 1 - Câştigátorul meciului!");
            return;
        }
        else if (player2RoundsWon >= roundsToWin) {
            currentGameWindow->showWinner("Jucátorul 2 - Câştigátorul meciului!");
            return;
        }
        else if (m_round_Counter > maxRounds) {
            if (player1RoundsWon > player2RoundsWon) {
                currentGameWindow->showWinner("Jucátorul 1 - Câştigátorul meciului!");
            }
            else if (player2RoundsWon > player1RoundsWon) {
                currentGameWindow->showWinner("Jucátorul 2 - Câştigátorul meciului!");
            }
            else {
                currentGameWindow->showWinner("Meciul s-a terminat la egalitate!");
            }
            return;
        }

        QTimer::singleShot(2000, this, &Game::startNewRound);
    }
}
void Game::handleBoardClick(int row, int col, int player) {
    qDebug() << "=== BOARD CLICK DEBUG START ===";
    qDebug() << "Processing move at (" << row << ", " << col << ") for player "
        << (currentPlayer == Color::Red ? "1 (Red)" : "2 (Blue)");

    if (!currentGameWindow) {
        qDebug() << "No active game window!";
        return;
    }

    // Prevent multiple simultaneous moves
    static bool processingMove = false;
    if (processingMove) {
        qDebug() << "Move already being processed, ignoring duplicate call";
        return;
    }
    processingMove = true;

    // Block signals during processing to prevent cascading updates
    currentGameWindow->blockSignals(true);

    if (!m_gameBoard.canBePlaced(row, col)) {
        qDebug() << "Position is not valid for placement.";
        currentGameWindow->blockSignals(false);
        processingMove = false;
        return;
    }

    // FOLOSEȘTE pentru logica de sfârșit de joc
    auto emptyPositions = m_gameBoard.getEmptyPositionsModern();
    if (emptyPositions.empty()) {
        qDebug() << "No empty positions left - triggering end game";
        currentGameWindow->blockSignals(false);
        processingMove = false;

        QTimer::singleShot(100, this, [this]() {
            checkRoundEnd();
            });
        return;
    }

    Player& currentPlayerRef = (currentPlayer == Color::Red) ? player1 : player2;

    if (currentPlayerRef.numberofValidCards() <= 0) {
        qDebug() << "Current player has no more cards!";
        currentGameWindow->blockSignals(false);
        processingMove = false;

        // Use timer to prevent immediate UI conflicts
        QTimer::singleShot(100, this, [this]() {
            checkRoundEnd();
            });
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
                break;
            }
        }

        if (!foundCard) {
            qDebug() << "Selected card is not valid for current player!";
            currentGameWindow->blockSignals(false);
            processingMove = false;
            return;
        }
    }
    else {
        qDebug() << "No card selected! Player must select a card first.";
        currentGameWindow->blockSignals(false);
        processingMove = false;
        return;
    }

    if (!m_gameBoard.canBePushed(cardToPlay, { row, col })) {
        qDebug() << "Card cannot be pushed to this position - value too low";
        currentGameWindow->blockSignals(false);
        processingMove = false;
        return;
    }

    qDebug() << "Placing card with value:" << cardToPlay.getValue();

    // Place the card
    m_gameBoard.pushCard(cardToPlay, { row, col });

    // Clear selection
    clearSelectedCard();
    currentGameWindow->clearCardSelection();

    // Board expansion logic
    QString windowTitle = currentGameWindow->windowTitle();
    int maxBoardSize = 4;
    int targetWinSize = 3;

    if (windowTitle == "Training") {
        maxBoardSize = 4;
        targetWinSize = 3;
    }
    else if (windowTitle == "Mage Duel") {
        maxBoardSize = 5;
        targetWinSize = 4;
    }
    else if (windowTitle == "Power Duel") {
        maxBoardSize = 5;
        targetWinSize = 4;
    }
    else if (windowTitle == "Power & Mage Duel") {
        maxBoardSize = 5;
        targetWinSize = 4;
    }

    // Do board management
    try {
        qDebug() << "Starting board management...";
        m_gameBoard.smartBoardManagement(maxBoardSize);
        qDebug() << "Board management completed";

        // Mark card as used
        currentPlayerRef.makeCardInvalid(cardToPlay);
        currentPlayerRef.getPastVector().push_back(cardToPlay);

    }
    catch (const std::exception& e) {
        qDebug() << "Exception during board management: " << e.what();
        currentGameWindow->blockSignals(false);
        processingMove = false;
        return;
    }

    // Re-enable signals
    currentGameWindow->blockSignals(false);

    // Batch update interface using timer to prevent conflicts
    QTimer::singleShot(50, this, [this, targetWinSize, windowTitle]() {
        if (!currentGameWindow) return;

        qDebug() << "Starting UI update batch...";

        // Single batch update
        currentGameWindow->setPlayer1Cards(player1.getVector());
        currentGameWindow->setPlayer2Cards(player2.getVector());
        currentGameWindow->updateBoardView();

        qDebug() << "UI update batch completed";

        // Check end of round
        Board::State winState = m_gameBoard.checkWin(false, targetWinSize);

        if (winState != Board::State::None) {
            qDebug() << "Win condition detected, ending round";
            if (windowTitle == "Training") {
                checkRoundEnd();
            }
            else if (windowTitle == "Mage Duel") {
                checkMageDuelRoundEnd();
            }
            else if (windowTitle == "Power Duel") {
                checkPowerDuelRoundEnd();
            }
            else if (windowTitle == "Power & Mage Duel") {
                checkCombinedRoundEnd();
            }
            return;
        }

        // Check if both players are out of cards
        if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
            qDebug() << "Both players out of cards, ending round";
            if (windowTitle == "Training") {
                checkRoundEnd();
            }
            else if (windowTitle == "Mage Duel") {
                checkMageDuelRoundEnd();
            }
            else if (windowTitle == "Power Duel") {
                checkPowerDuelRoundEnd();
            }
            else if (windowTitle == "Power & Mage Duel") {
                checkCombinedRoundEnd();
            }
            return;
        }

        // Switch player
        currentPlayer = (currentPlayer == Color::Red) ? Color::Blue : Color::Red;
        currentGameWindow->setCurrentPlayer(currentPlayer);

        qDebug() << "Player switched to" << (currentPlayer == Color::Red ? "Red" : "Blue");
        });

    // Reset the processing flag
    processingMove = false;
    qDebug() << "=== BOARD CLICK DEBUG END ===";
}
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
    qDebug() << "=== DEBUG: startMageDuel called ===";

    try {
        m_round_Counter = 1;
        player1RoundsWon = 0;
        player2RoundsWon = 0;
        roundsToWin = 3;
        maxRounds = 5;

        auto* window3 = new SecondaryWindow("Mage Duel",
            QDir::currentPath() + QDir::separator() + "eter.png",
            &Game::get_Instance(), "", "", "", "", true, false);

        // Hide initially to prevent flash
        window3->hide();

        // Transfer ownership to smart pointer with custom deleter
        currentGameWindow.reset(window3);

        if (!currentGameWindow) {
            qDebug() << "ERROR: Failed to create currentGameWindow!";
            return;
        }

        // Setup connections while hidden
        connect(currentGameWindow.get(), &SecondaryWindow::boardClicked,
            this, &Game::handleBoardClick);
        connect(currentGameWindow.get(), &SecondaryWindow::returnToMainMenu,
            this, &Game::showMainMenu);

        // Setup game state
        std::vector<SimpleCard> PastCards;
        player1 = Player("Jucătorul 1", {
            SimpleCard(1, Color::Red), SimpleCard(1, Color::Red),
            SimpleCard(2, Color::Red), SimpleCard(2, Color::Red), SimpleCard(2, Color::Red),
            SimpleCard(3, Color::Red), SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),
            SimpleCard(4, Color::Red), SimpleCard(5, Color::Red)
            }, PastCards, true);

        player2 = Player("Jucătorul 2", {
            SimpleCard(1, Color::Blue), SimpleCard(1, Color::Blue),
            SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue),
            SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),
            SimpleCard(4, Color::Blue), SimpleCard(5, Color::Blue)
            }, PastCards, true);

        auto mage1 = player1.getMageAssignment();
        auto mage2 = player2.getMageAssignment();
        while ((mage1 == mage2) || (mage1 % 2 == 0 && mage2 == mage1 + 1) || (mage1 % 2 == 1 && mage2 == mage1 - 1)) {
            player2.reasignMage();
            mage2 = player2.getMageAssignment();
        }

        currentGameWindow->setMagesCompact(QString::fromStdString(player1.getMage()),
            QString::fromStdString(player2.getMage()));

        // Start the round while window is still hidden
        startNewMageDuelRound();

        // Show only after everything is ready
        currentGameWindow->show();

    }
    catch (const std::exception& e) {
        qDebug() << "EXCEPTION in startMageDuel:" << e.what();
    }
    catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in startMageDuel";
    }
}
void Game::startNewMageDuelRound() {
    if (!currentGameWindow) return;

    qDebug() << "=== Starting Mage Duel Round " << m_round_Counter << " ===";

    player1.ResetVector();
    player2.ResetVector();

    m_gameBoard.initializeForDynamicPlay(2);
    currentPlayer = Color::Red;
    playerMoveCompleted = false;

    auto emptyPositions = m_gameBoard.getEmptyPositionsModern();

    currentGameWindow->setBoard(m_gameBoard, 5);
    currentGameWindow->setPlayer1Cards(player1.getVector());
    currentGameWindow->setPlayer2Cards(player2.getVector());
    currentGameWindow->setCurrentPlayer(currentPlayer);
    currentGameWindow->resetView();
    currentGameWindow->updateBoardView();
    currentGameWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);
}
void Game::checkMageDuelRoundEnd() {
    if (!currentGameWindow) return;

    // Check win conditions for 4x4 board (need 4 in a row for mage duel)
    Board::State winState = m_gameBoard.checkWin(false, 4);
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
        auto state = m_gameBoard.checkWin(true, 4);
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
        QTimer::singleShot(2000, this, &Game::startNewMageDuelRound);
    }
}

void Game::startPowerDuel() {
    qDebug() << "=== DEBUG: startPowerDuel called ===";

    try {
        m_round_Counter = 1;
        player1RoundsWon = 0;
        player2RoundsWon = 0;
        roundsToWin = 3;
        maxRounds = 5;

        std::vector<SimpleCard> PastCards;

        player1 = Player("Jucătorul 1", {
            SimpleCard(1, Color::Red), SimpleCard(1, Color::Red),
            SimpleCard(2, Color::Red), SimpleCard(2, Color::Red), SimpleCard(2, Color::Red),
            SimpleCard(3, Color::Red), SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),
            SimpleCard(4, Color::Red), SimpleCard(5, Color::Red)
            }, PastCards, true);

        player2 = Player("Jucătorul 2", {
            SimpleCard(1, Color::Blue), SimpleCard(1, Color::Blue),
            SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue),
            SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),
            SimpleCard(4, Color::Blue), SimpleCard(5, Color::Blue)
            }, PastCards, true);

        player1.assignPower();
        player2.assignPower();

        while (player1.getPower() == player2.getPower()) {
            player2.reassignPower();
        }

        QString power1Name = getPowerDisplayName(player1.getPower());
        QString power2Name = getPowerDisplayName(player2.getPower());

        auto* window = new SecondaryWindow("Power Duel",
            QDir::currentPath() + QDir::separator() + "eter.png",
            &Game::get_Instance(), "", "",
            power1Name, power2Name,
            false, true);

        // Hide initially
        window->hide();

        // Transfer ownership to smart pointer with custom deleter
        currentGameWindow.reset(window);

        if (!currentGameWindow) {
            qDebug() << "ERROR: Failed to create currentGameWindow!";
            return;
        }

        // Setup connections while hidden
        connect(currentGameWindow.get(), &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
        connect(currentGameWindow.get(), &SecondaryWindow::returnToMainMenu, this, &Game::showMainMenu);

        // Start round while hidden
        startNewPowerDuelRound();

        // Show only when ready
        currentGameWindow->show();

    }
    catch (const std::exception& e) {
        qDebug() << "EXCEPTION in startPowerDuel:" << e.what();
    }
    catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in startPowerDuel";
    }
}


void Game::startNewPowerDuelRound() {
    if (!currentGameWindow) return;

    qDebug() << "=== Starting Power Duel Round " << m_round_Counter << " ===";
    qDebug() << "Score - Player 1:" << player1RoundsWon << ", Player 2:" << player2RoundsWon;

    // Reset cards for new round (but keep powers)
    player1.ResetVector();
    player2.ResetVector();

    // Initialize board for power duel (4x4 final size)
    m_gameBoard.initializeForDynamicPlay(5);  // Can expand to 5x5, will fix to 4x4
    currentPlayer = Color::Red;
    playerMoveCompleted = false;

    auto emptyPositions = m_gameBoard.getEmptyPositionsModern();

    // Set the board with max size 5 for Power Duel (will fix to 4x4 final target)
    currentGameWindow->setBoard(m_gameBoard, 5);  // Max expansion size
    currentGameWindow->setPlayer1Cards(player1.getVector());
    currentGameWindow->setPlayer2Cards(player2.getVector());
    currentGameWindow->setCurrentPlayer(currentPlayer);
    currentGameWindow->resetView();
    currentGameWindow->updateBoardView();
    currentGameWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);
}
void Game::checkPowerDuelRoundEnd() {
    if (!currentGameWindow) return;

    // Check win conditions for 4x4 board (need 4 in a row for power duel)
    Board::State winState = m_gameBoard.checkWin(false, 4);
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
        auto state = m_gameBoard.checkWin(true, 4);
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
        QTimer::singleShot(2000, this, &Game::startNewPowerDuelRound);
    }
}
QString Game::getPowerDisplayName(Power power) const {
    switch (power) {
    case Power::PowerEarthquake:
        return "PowerEarthquake";
    case Power::PowerTide:
        return "PowerTide";
    case Power::PowerStorm:
        return "PowerStorm";
    case Power::PowerSquall:
        return "PowerSquall";
    case Power::PowerGale:
        return "PowerGale";
    default:
        return "UnknownPower";
    }
}
void Game::startTournament()
{
	this->m_round_Counter = 1;
	int16_t maxRounds = 3;

}
void Game::startMageDuelAndPower() {
    qDebug() << "=== DEBUG: startMageDuelAndPower called ===";

    try {
        m_round_Counter = 1;
        player1RoundsWon = 0;
        player2RoundsWon = 0;
        roundsToWin = 2;
        maxRounds = 3;

        std::vector<SimpleCard> PastCards;

        player1 = Player("Jucătorul 1", {
            SimpleCard(1, Color::Red), SimpleCard(1, Color::Red),
            SimpleCard(2, Color::Red), SimpleCard(2, Color::Red), SimpleCard(2, Color::Red),
            SimpleCard(3, Color::Red), SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),
            SimpleCard(4, Color::Red), SimpleCard(5, Color::Red)
            }, PastCards, true);

        player2 = Player("Jucătorul 2", {
            SimpleCard(1, Color::Blue), SimpleCard(1, Color::Blue),
            SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue), SimpleCard(2, Color::Blue),
            SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),
            SimpleCard(4, Color::Blue), SimpleCard(5, Color::Blue)
            }, PastCards, true);

        // Assign distinct mages
        auto mage1 = player1.getMageAssignment();
        auto mage2 = player2.getMageAssignment();
        while ((mage1 == mage2) || (mage1 % 2 == 0 && mage2 == mage1 + 1) || (mage1 % 2 == 1 && mage2 == mage1 - 1)) {
            player2.reasignMage();
            mage2 = player2.getMageAssignment();
        }

        // Assign distinct powers
        player1.assignPower();
        player2.assignPower();
        while (player1.getPower() == player2.getPower()) {
            player2.reassignPower();
        }

        QString mage1Name = QString::fromStdString(player1.getMage());
        QString mage2Name = QString::fromStdString(player2.getMage());
        QString power1Name = getPowerDisplayName(player1.getPower());
        QString power2Name = getPowerDisplayName(player2.getPower());

        auto* window2 = new SecondaryWindow("Power & Mage Duel",
            QDir::currentPath() + QDir::separator() + "eter.png",
            &Game::get_Instance(),
            mage1Name, mage2Name,
            power1Name, power2Name,
            true, true);

        // Hide initially to prevent flash
        window2->hide();

        // Transfer ownership to smart pointer with custom deleter
        currentGameWindow.reset(window2);

        if (!currentGameWindow) {
            qDebug() << "ERROR: Failed to create currentGameWindow!";
            return;
        }

        // Setup connections while hidden
        connect(currentGameWindow.get(), &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
        connect(currentGameWindow.get(), &SecondaryWindow::returnToMainMenu, this, &Game::showMainMenu);

        // Start round while hidden
        startNewCombinedRound();

        // Show only when everything is ready
        currentGameWindow->show();

    }
    catch (const std::exception& e) {
        qDebug() << "EXCEPTION in startMageDuelAndPower:" << e.what();
    }
    catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in startMageDuelAndPower";
    }
}
void Game::startNewCombinedRound() {
    if (!currentGameWindow) return;

    qDebug() << "=== Starting Combined Duel Round " << m_round_Counter << " ===";
    qDebug() << "Score - Player 1:" << player1RoundsWon << ", Player 2:" << player2RoundsWon;

    // Reset cards for new round (but keep mages and powers)
    player1.ResetVector();
    player2.ResetVector();

    // Initialize board for combined duel (4x4 final size)
    m_gameBoard.initializeForDynamicPlay(5);  // Can expand to 5x5, will fix to 4x4
    currentPlayer = Color::Red;
    playerMoveCompleted = false;

    // Set the board with max size 5 for Combined Duel (will fix to 4x4 final target)
    currentGameWindow->setBoard(m_gameBoard, 5);  // Max expansion size
    currentGameWindow->setPlayer1Cards(player1.getVector());
    currentGameWindow->setPlayer2Cards(player2.getVector());
    currentGameWindow->setCurrentPlayer(currentPlayer);
    currentGameWindow->resetView();
    currentGameWindow->updateBoardView();
    currentGameWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);
}
void Game::checkCombinedRoundEnd() {
    if (!currentGameWindow) return;

    // Check win conditions for 4x4 board (need 4 in a row for combined duel)
    Board::State winState = m_gameBoard.checkWin(false, 4);
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
        auto state = m_gameBoard.checkWin(true, 4);
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
        QTimer::singleShot(2000, this, &Game::startNewCombinedRound);
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


