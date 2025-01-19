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



class Game:public QObject
{
    Q_OBJECT

private:
    int16_t m_round_Counter;
    Board m_gameBoard;
    static Game m_current_Instance;
    Player player1, player2;
    //Explosion m_explosion;
    std::optional<Explosion> m_explosion;
    bool m_illusionsEnabled;
    Color currentPlayer;


    Game() : m_round_Counter{ 0 }, m_gameBoard{} {}

    // Methods for the different game types
    void startTraining();
    void startMageDuel();
    void startPowerDuel();
    void startTournament();
    void startMageDuelAndPower();
    void showExplosionMenu();

    bool playerMoveCompleted;

public:
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

    Board& getBoard(); // Metodă pentru a obține referința către tablă
    const Board& getBoard() const; // Supraincarcare pentru acces constant


public slots:
    void handleBoardClick(int row, int col);
signals:
    void playerActionComplete();
};
