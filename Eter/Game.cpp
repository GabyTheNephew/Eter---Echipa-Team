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
	return m_gameBoard; 
}

const Board& Game::getBoard() const {
	return m_gameBoard;
}





void Game::startTraining() {
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

    auto* trainingWindow = new SecondaryWindow("Training", QDir::currentPath() + QDir::separator() + "eter.png", &Game::get_Instance(), "", "", "", "", false, false);
    trainingWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(trainingWindow, &SecondaryWindow::boardClicked, this, &Game::handleBoardClick);
    trainingWindow->show();

    // BUCLA MECIULUI - Continuă până când cineva câștigă destule runde
    while (player1RoundsWon < roundsToWin && player2RoundsWon < roundsToWin && m_round_Counter <= maxRounds) {
        qDebug() << "=== Începe Runda " << m_round_Counter << " ===";
        qDebug() << "Scorul curent - Jucătorul 1:" << player1RoundsWon << ", Jucătorul 2:" << player2RoundsWon;

        // Resetare pentru runda nouă
        player1 = Player("Jucătorul 1", { SimpleCard(1, Color::Red),SimpleCard(1, Color::Red), SimpleCard(2, Color::Red),SimpleCard(2, Color::Red),SimpleCard(3, Color::Red), SimpleCard(3, Color::Red),SimpleCard(4, Color::Red) }, PastCards);
        player2 = Player("Jucătorul 2", { SimpleCard(1, Color::Blue),SimpleCard(1, Color::Blue), SimpleCard(2, Color::Blue),SimpleCard(2, Color::Blue),SimpleCard(3, Color::Blue), SimpleCard(3, Color::Blue),SimpleCard(4, Color::Blue) }, PastCards);

        m_gameBoard.initializeForDynamicPlay(3);
        currentPlayer = Color::Red;
        playerMoveCompleted = false;

        trainingWindow->setBoard(m_gameBoard, 3);
        trainingWindow->setPlayer1Cards(player1.getVector());
        trainingWindow->setPlayer2Cards(player2.getVector());
        trainingWindow->setCurrentPlayer(currentPlayer);
        trainingWindow->resetView();
        trainingWindow->updateBoardView();

        // Actualizează afișajul informațiilor despre meci
        trainingWindow->updateMatchInfo(m_round_Counter, player1RoundsWon, player2RoundsWon, roundsToWin);

        // BUCLA RUNDEI - Joacă până când această rundă este decisă
        bool roundInProgress = true;
        QString roundWinnerName;

        while (roundInProgress) {
            QCoreApplication::processEvents();

            if (playerMoveCompleted) {
                // Verifică condițiile de câștig ÎNAINTE de schimbarea jucătorilor
                Board::State winState = m_gameBoard.checkWin(false, 3);

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
                    trainingWindow->setCurrentPlayer(Color::Blue);
                    currentPlayer = Color::Blue;
                    qDebug() << "Rândul jucătorului 2.";
                }
                else if (currentPlayer == Color::Blue && player2.numberofValidCards() > 0) {
                    trainingWindow->setCurrentPlayer(Color::Red);
                    currentPlayer = Color::Red;
                    qDebug() << "Rândul jucătorului 1.";
                }

                playerMoveCompleted = false;
            }

            // Verifică dacă ambii jucători au rămas fără cărți (numărarea punctelor)
            if (player1.numberofValidCards() == 0 && player2.numberofValidCards() == 0) {
                qDebug() << "Ambii jucători au rămas fără cărți - se verifică punctele...";
                auto state = m_gameBoard.checkWin(true, 3);

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
                    // În caz de egalitate, niciun jucător nu primește punct
                }

                roundInProgress = false;
            }
        }

        // Afișează câștigătorul rundei dacă există unul
        if (!roundWinnerName.isEmpty() && roundWinnerName != "Egalitate") {
            trainingWindow->showRoundWinner(roundWinnerName, m_round_Counter);
        }
        else if (roundWinnerName == "Egalitate") {
            QMessageBox::information(trainingWindow, "Rezultatul rundei",
                QString("Runda %1 s-a terminat la egalitate!").arg(m_round_Counter));
        }

        incrementRoundCounter();

        // Verifică dacă meciul a fost câștigat
        if (player1RoundsWon >= roundsToWin) {
            trainingWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            break;
        }
        else if (player2RoundsWon >= roundsToWin) {
            trainingWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            break;
        }
        else if (m_round_Counter > maxRounds) {
            // S-a atins numărul maxim de runde, determină câștigătorul prin rundele câștigate
            if (player1RoundsWon > player2RoundsWon) {
                trainingWindow->showWinner("Jucătorul 1 - Câștigătorul meciului!");
            }
            else if (player2RoundsWon > player1RoundsWon) {
                trainingWindow->showWinner("Jucătorul 2 - Câștigătorul meciului!");
            }
            else {
                trainingWindow->showWinner("Meciul s-a terminat la egalitate!");
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


