#pragma once
#include <string_view>
#include<cstdint>
#include <QTimer>
#include <QMessageBox> 
#include "Board.h"
#include "Player.h"
#include "Explosion.h"
#include "Mages.h"
#include <string>
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDir>
#include "SecondaryWindow.h"
#include "IntermediateMenu.h"
#include "BoardView.h"
#include <cstdlib>
#include <ctime>
#include "GameTypes.h"
#include "EnumConversion.h"
#include <memory>




class Game:public QObject
{
    Q_OBJECT

   friend class GameSaver;

private:
#pragma region Game
    int16_t m_round_Counter;
    Board m_gameBoard;
    static Game m_current_Instance;
    GameType m_currentGameType;
    int m_boardMaxSize;
    QString m_userEmail;
    QString m_userPassword;


    std::unique_ptr<Explosion> m_explosion;
    bool m_explosionActivated = false;
    bool m_explosionsEnabled = false;
    bool m_illusionsEnabled;
#pragma endregion

#pragma region Player
    Player player1, player2;
    bool playerMoveCompleted;
    Color currentPlayer;
    int16_t player1RoundsWon;
    int16_t player2RoundsWon;
    bool m_player1MageUsed = false;
    bool m_player2MageUsed = false;
    bool m_player1PowerUsed = false;
    bool m_player2PowerUsed = false;
    bool m_player1IllusionUsed = false;
    bool m_player2IllusionUsed = false;
    QTimer* player1Timer;
    QTimer* player2Timer;
    int timerDuration;
    int player1RemainingTime;
    int player2RemainingTime;
    bool timerActive;
#pragma endregion

    static bool s_forceStop;

    Game() : m_round_Counter{ 0 }, m_gameBoard{}, timerDuration(90),
        player1RemainingTime(90), player2RemainingTime(90), timerActive(false) {
        player1Timer = new QTimer(this);
        player2Timer = new QTimer(this);

        connect(player1Timer, &QTimer::timeout, this, &Game::onPlayerTimerTimeout);
        connect(player2Timer, &QTimer::timeout, this, &Game::onPlayerTimerTimeout);
    }
#pragma region GameTypes  Methods
    void startTraining();
    void startMageDuel();
    void startPowerDuel();
    void startTournament();
    void startMageDuelAndPower();
#pragma endregion

    

public:
    bool m_timerEnabled = false;
    
#pragma region Game Methods
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    static Game& get_Instance();
    static void forceStop();
    GameType getCurrentGameType() const;
    int getRoundCounter() const;
    Color getCurrentPlayerColor()const;
    int getPlayer1Score() const;
    int getBoardMaxSize() const;
    int getPlayer2Score() const;
    bool isTimerEnabled() const;
    void incrementRoundCounter();
    bool checkPlayExplosion(Board& m_board)const;
    bool canActivateExplosion() const;
    void activateExplosion();
    void startGame(GameType selectedGameType);
    Board& getBoard();
    const Board& getBoard() const;
    void setExplosionsEnabled(bool enabled);
    bool areExplosionsEnabled() const;
    void setIllusionsEnabled(bool enabled);
    bool areIllusionsEnabled()const;
    void endCurrentRound();

    void handleExplosionActivation();
    void showExplosionRotationDialog();
    void applyExplosionEffects(const Explosion& explosion);

    bool wouldCreateIsolatedCards(const Explosion& explosion)const;
    bool wouldMageCreateIsolatedCards(const Board& testBoard) const;
    bool areCardsConnected(const Board& test_board)const;
    void resetMageFlags();

    QString getExplosionPreviewGrid() const;

#pragma endregion


#pragma region Extras/Helpers Methods
    GameType stringToGameType(std::string_view word);
    std::string_view gameTypeToString(GameType gameType) const;

    QString getUserEmail() const;
    QString getUserPassword() const;
    void setUserCredentials(const QString& email, const QString& password);
#pragma endregion

    

#pragma region PlayerElemnts Methods
    Player& getPlayer1() { return player1; }
    Player& getPlayer2() { return player2; }
    void setPlayer1MageUsed(bool used);
    void setPlayer2MageUsed(bool used);
    void setPlayer1PowerUsed(bool used);
    void setPlayer2PowerUsed(bool used);
    bool isPlayer1MageUsed() const;
    bool isPlayer2MageUsed() const;
    bool isPlayer1PowerUsed() const;
    bool isPlayer2PowerUsed() const;
    bool isPlayer1IllusionUsed() const;
    bool isPlayer2IllusionUsed() const;
    void setPlayer1IllusionUsed(bool used);
    void setPlayer2IllusionUsed(bool used);
    bool getPlayerMoveCompleted() const;
    void setPlayerMoveCompleted(bool completed);
    Player& getCurrentPlayer();

#pragma endregion

    
#pragma region PlayerTimer Methods
    int getPlayer1RemainingTime() const;
    int getPlayer2RemainingTime() const;
    void setTimerDuration(int seconds);
    int getTimerDuration() const;
    void startPlayerTimer();
    void stopPlayerTimer();
    void resetTimers();
#pragma endregion

    void startLoadedGame();

public slots:
    void handleBoardClick(int row, int col);
    void onPlayerTimerTimeout();
signals:
    void playerActionComplete();
    void gameEnded();
    void currentPlayerChanged(Color newPlayer);


};
