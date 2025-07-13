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
#include "SecondaryWindow.h"
#include "IntermediateMenu.h"
#include "BoardView.h"
#include <cstdlib>
#include <ctime>
#include "GameTypes.h"
#include "EnumConversion.h"




class Game:public QObject
{
    Q_OBJECT

   friend class GameSaver;

private:
    int16_t m_round_Counter;
    Board m_gameBoard;
    static Game m_current_Instance;
    GameType m_currentGameType;
    int m_boardMaxSize;
    

    std::optional<Explosion> m_explosion;
    bool m_illusionsEnabled;
    bool m_timerEnabled = false;
   

    Player player1, player2;
    Color currentPlayer;
    int16_t player1RoundsWon;
	int16_t player2RoundsWon;
    bool m_player1MageUsed = false;
    bool m_player2MageUsed = false;
    bool m_player1PowerUsed = false;
    bool m_player2PowerUsed = false;

    QString m_userEmail;
    QString m_userPassword;

    static bool s_forceStop;


    Game() : m_round_Counter{ 0 }, m_gameBoard{} {}

    void startTraining();
    void startMageDuel();
    void startPowerDuel();
    void startTournament();
    void startMageDuelAndPower();
    void showExplosionMenu();

    bool playerMoveCompleted;

    


public:
    Player& getPlayer1() { return player1; }
    Player& getPlayer2() { return player2; }
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
  
    static Game& get_Instance();
    static void forceStop();
    

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

    GameType getCurrentGameType() const;
    int getRoundCounter() const;
    Color getCurrentPlayerColor()const;
    int getPlayer1Score() const;
    int getBoardMaxSize() const;
    int getPlayer2Score() const;
    bool isTimerEnabled() const;
    bool isPlayer1MageUsed() const;
    bool isPlayer2MageUsed() const;
    bool isPlayer1PowerUsed() const;
    bool isPlayer2PowerUsed() const;
    QString getUserEmail() const;
	QString getUserPassword() const;
    void setUserCredentials(const QString& email, const QString& password);


public slots:
    void handleBoardClick(int row, int col);
signals:
    void playerActionComplete();
    void gameEnded();
    void currentPlayerChanged(Color newPlayer);


};
