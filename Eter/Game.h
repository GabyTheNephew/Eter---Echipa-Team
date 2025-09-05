// SOLUȚIA pentru Game.h - Custom deleter pentru Qt objects
#pragma once
#include <string_view>
#include<cstdint>
#include "Board.h"
#include "Player.h"
#include "Explosion.h"
#include "Mages.h"
#include <string>
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDir>
#include "IntermediateMenu.h"
#include "BoardView.h"
#include <cstdlib>
#include <ctime>
#include "MainWindow.h"
#include <memory> // Added for smart pointers
#include "SecondaryWindow.h"

// Custom deleter for Qt objects to avoid double deletion
struct QObjectDeleter {
    void operator()(QObject* obj) const {
        if (obj && !obj->parent()) {
            // Only delete if object has no parent (not managed by Qt)
            obj->deleteLater();
        }
        // If object has parent, Qt will handle deletion automatically
    }
};

class Game :public QObject
{
    Q_OBJECT

private:
    SimpleCard selectedCard;

    // Using custom deleter to handle Qt's parent-child system properly
    std::unique_ptr<SecondaryWindow, QObjectDeleter> currentGameWindow;

    int16_t player1RoundsWon = 0;
    int16_t player2RoundsWon = 0;
    int16_t roundsToWin = 2;
    int16_t maxRounds = 3;

    int16_t m_round_Counter;
    Board m_gameBoard;
    static Game m_current_Instance;
    Player player1, player2;

    std::optional<Explosion> m_explosion;
    bool m_illusionsEnabled;
    Color currentPlayer;

    Game() : m_round_Counter{ 0 }, m_gameBoard{}, playerMoveCompleted{ false },
        currentPlayer{ Color::Red }, m_illusionsEnabled{ false } {
    }

    void startTraining();
    void startMageDuel();
    void startPowerDuel();
    void startTournament();
    void startMageDuelAndPower();
    void showExplosionMenu();

    bool playerMoveCompleted;
    QString getPowerDisplayName(Power power) const;

public:
    void startNewCombinedRound();
    void checkCombinedRoundEnd();

    void startNewPowerDuelRound();
    void checkPowerDuelRoundEnd();

    void startNewMageDuelRound();
    void checkMageDuelRoundEnd();

    void setSelectedCard(const SimpleCard& card) { selectedCard = card; }
    bool hasSelectedCard() const { return selectedCard.getValue() > 0; }
    void clearSelectedCard() { selectedCard = SimpleCard(); }

    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static Game& get_Instance();

    enum class GameType : int16_t
    {
        Training,
        MageDuel,
        Power,
        MageDuelAndPower,
        Tournament
    };

    GameType stringToGameType(std::string_view word);
    std::string_view gameTypeToString(GameType gameType) const;

    void incrementRoundCounter();
    bool checkPlayExplosion(Board& m_board);
    void startGame(GameType selectedGameType);

    void setExplosionsEnabled(bool enabled);
    bool areExplosionsEnabled() const;

    void setIllusionsEnabled(bool enabled);
    bool areIllusionsEnabled()const;

    bool getPlayerMoveCompleted() const { return playerMoveCompleted; }
    void setPlayerMoveCompleted(bool completed) { playerMoveCompleted = completed; }
    Player& getCurrentPlayer();

    Board& getBoard();
    const Board& getBoard() const;
    void showMainMenu();

    void startNewRound();
    void checkRoundEnd();
    void cleanupEmptyBorders();

    // Helper method to get raw pointer when needed for Qt connections
    SecondaryWindow* getCurrentGameWindow() const {
        return currentGameWindow.get();
    }

public slots:
    void handleBoardClick(int row, int col, int player);
signals:
    void playerActionComplete();
};